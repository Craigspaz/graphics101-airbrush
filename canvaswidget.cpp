#include "canvaswidget.h"

#include <iostream>
#include <cassert>

#include <QPainter>
#include <QMouseEvent>

using graphics101::Image;
using graphics101::ColorRGBA8;

namespace
{
QImage QImageFromImage( const Image& image )
{
    return QImage( reinterpret_cast< const uchar* >( image.data() ), image.width(), image.height(), QImage::Format_RGBA8888 );
}
}

CanvasWidget::CanvasWidget(QWidget *parent) : QWidget(parent)
{
    modified = false;

    image = Image();

    radius = 25;
    color = Qt::blue;
    color.setAlpha(64);
    shape = graphics101::Quadratic;
    flowRate = 66;
    flowRateTimerID = 0;
    painting = false;

    updateBrushImage();
}

void CanvasWidget::clear()
{
    image.fill( ColorRGBA8(255,255,255) );
    modified = false;
    update();
}

bool CanvasWidget::saveToPath( const QString& path )
{
    // Since we don't shrink the canvas when the user resizes the canvas smaller,
    // we might have extra canvas. Save only the visible part.
    const Image visible = Image( image ).resize( size().width(), size().height() );
    const bool success = visible.save( path.toStdString() );
    // After successfully saving, we no longer have unsaved changes.
    if( success ) modified = false;
    return success;
}

bool CanvasWidget::loadFromPath( const QString& path )
{
    Image loaded;
    const bool success = loaded.load( path.toStdString() );
    
    // After successfully loading, we no longer have unsaved changes.
    if( success ) {
        modified = false;
        
        // Always resize the loaded image to the current view size if bigger.
        image = loaded.resize(
            std::max( loaded.width(), size().width() ),
            std::max( loaded.height(), size().height() ),
            ColorRGBA8(255,255,255,255)
            );
        
        update();
    }
    
    return success;
}

QImage CanvasWidget::getBrushImage() const
{
    return QImageFromImage( airbrush_image );
}

void CanvasWidget::setRadius(int r)
{
    assert( r > 0 );

    radius = r;

    updateBrushImage();
}
void CanvasWidget::setFlowRate(int f)
{
    assert( f >= 0 );

    flowRate = f;
}
void CanvasWidget::setShape(const QString& str)
{
    shape = graphics101::AirBrushShapeFromString( str.toStdString() );

    updateBrushImage();
}
void CanvasWidget::setColor(const QColor& c)
{
    color = c;

    updateBrushImage();
}

void CanvasWidget::mousePressEvent(QMouseEvent *event)
{
    painting = true;
    // Save the position for the flow rate
    lastPos = event->pos();
    brushAt( lastPos );
    // If the flowRate is > 0, start a timer for that many milliseconds.
    if( flowRate > 0 ) {
        flowRateTimerID = startTimer( flowRate );
    }
}
void CanvasWidget::mouseMoveEvent(QMouseEvent *event)
{
    if( !painting ) return;
    // Save the position for the flow rate
    lastPos = event->pos();
    brushAt( lastPos );
}
void CanvasWidget::mouseReleaseEvent(QMouseEvent *event)
{
    painting = false;

    // Cancel the flow rate timer.
    if( flowRateTimerID != 0 ) killTimer( flowRateTimerID );
    flowRateTimerID = 0;
}
void CanvasWidget::paintEvent(QPaintEvent *event)
{
    // Create a drawing context.
    QPainter painter( this );
    // Redraw the "dirty" region.
    const QRect dirty = event->rect();
    painter.drawImage( dirty, QImageFromImage( image ), dirty );
}
void CanvasWidget::resizeEvent(QResizeEvent *event)
{
    // If either the width or height got bigger, make the image bigger.
    if( width() > image.width() || height() > image.height() ) {
        // To avoid resizing with every increased pixel, resize by CHUNK at a time.
        const int CHUNK = 128;
        const QSize newsize(
                    std::max( width() + CHUNK, image.width() ),
                    std::max( height() + CHUNK, image.height() )
                    );
        // Resize and fill newly visible areas with white.
        image.resize( newsize.width(), newsize.height(), ColorRGBA8( 255,255,255,255 ) );
        // Update to repaint everything.
        update();
    }
    // Bubble the resize event up.
    QWidget::resizeEvent( event );
}
void CanvasWidget::timerEvent(QTimerEvent *event)
{
    brushAt( lastPos );
}

void CanvasWidget::brushAt(const QPoint &point)
{
    // Step 1. Modify `image` pixels.
    const graphics101::Rect dirty = graphics101::paint_at( image, airbrush_image, point.x(), point.y() );

    // Step 2. Call update() to mark the painted part of the screen as dirty.
    //         Pass a tighter rectangle for better performance.
    update( QRect( dirty.x, dirty.y, dirty.width, dirty.height ) );
    modified = true;
}

void CanvasWidget::updateBrushImage()
{
    const ColorRGBA8 c( color.red(), color.green(), color.blue(), color.alpha() );
    graphics101::create_airbrush( airbrush_image, shape, radius, c );
    emit brushChanged( QImageFromImage( airbrush_image ) );
}
