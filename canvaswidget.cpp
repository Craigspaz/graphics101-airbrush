#include "canvaswidget.h"

#include <iostream>
#include <cassert>

#include <QPainter>
#include <QMouseEvent>

namespace
{
QImage resized( const QImage& img, const QSize& size )
{
    // We have nothing to do.
    // UPDATE: This function does a couple of other useful things,
    //         like format conversion and compositing with white.
    //         Therefore, always run it.
    // if( img.size() == size ) return img;

    // Create a blank, white image with the desired size.
    // Preserving the format is dangerous, because
    // the default constructor sets a weird format (maybe QImage::Format_Invalid).
    // QImage newimg( size, img.format() );
    QImage newimg( size, QImage::Format_RGB32 );
    newimg.fill( qRgb(255,255,255) );

    // Create a context around the new image and draw the old image into it.
    QPainter painter(&newimg);
    painter.drawImage( QPoint(0,0), img );
    return newimg;
}

}

CanvasWidget::CanvasWidget(QWidget *parent) : QWidget(parent)
{
    modified = false;

    image = QImage( 0, 0, QImage::Format_RGB32 );

    radius = 25;
    color = Qt::blue;
    color.setAlpha(64);
    shape = airbrush::Quadratic;
    flowRate = 66;
    flowRateTimerID = 0;
    painting = false;

    updateBrushImage();
}

void CanvasWidget::clear()
{
    image.fill( qRgb(255,255,255) );
    modified = false;
    update();
}

bool CanvasWidget::saveToPath( const QString& path )
{
    // Since we don't shrink the canvas when the user resizes the canvas smaller,
    // we might have extra canvas. Save only the visible part.
    const QImage visible = resized( image, size() );
    const bool success = visible.save( path );
    // After successfully saving, we no longer have unsaved changes.
    if( success ) modified = false;
    return success;
}

bool CanvasWidget::loadFromPath( const QString& path )
{
    QImage loaded;
    const bool success = loaded.load( path );
    
    // After successfully loading, we no longer have unsaved changes.
    if( success ) {
        modified = false;
        
        // Always resize the loaded image to the current view size if bigger.
        image = resized( loaded, loaded.size().expandedTo( size() ) );
        
        image = image.convertToFormat( QImage::Format_RGB32 );
        update();
    }
    
    return success;
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
    shape = airbrush::AirBrushShapeFromQString( str );

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
    painter.drawImage( dirty, image, dirty );
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
        image = resized( image, newsize );
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
    const QRect dirty = airbrush::paint_at( image, airbrush_image, point );

    // Step 2. Call update() to mark the painted part of the screen as dirty.
    //         Pass a tighter rectangle for better performance.
    update( dirty );
    modified = true;
}

void CanvasWidget::updateBrushImage()
{
    airbrush::create_airbrush( airbrush_image, shape, radius, color.rgba() );
    emit brushChanged( airbrush_image );
}
