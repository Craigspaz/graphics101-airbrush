#ifndef CANVASWIDGET_H
#define CANVASWIDGET_H

#include <QWidget>
#include <QImage>
#include <airbrush.h>

class CanvasWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CanvasWidget(QWidget *parent = nullptr);

    bool loadFromPath( const QString& path );
    bool saveToPath( const QString& path );

    bool isModified() const { return modified; }
    QColor getColor() const { return color; }
    const QImage& getBrushImage() const { return airbrush_image; }

signals:
    void brushChanged( const QImage& brushImage ) const;

public slots:
    void clear();
    void setRadius(int r);
    void setFlowRate(int f);
    void setShape(const QString& str);
    void setColor(const QColor& c);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void timerEvent(QTimerEvent *event) override;

private:
    void brushAt(const QPoint &point);
    void updateBrushImage();

    bool modified;

    QImage image;
    QImage airbrush_image;

    int radius;
    QColor color;
    int flowRate;
    airbrush::AirBrushShape shape;

    int flowRateTimerID;
    bool painting;
    QPoint lastPos;
};

#endif // CANVASWIDGET_H
