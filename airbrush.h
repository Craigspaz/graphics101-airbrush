#ifndef __airbrush__h__
#define __airbrush__h__

#include <QImage>
#include <QString>

namespace airbrush
{
    enum AirBrushShape {
        Constant = 0,
        Linear,
        Quadratic,
        Special,
        NumAirBrushShapes
    };
    QString QStringFromAirBrushShape( AirBrushShape type );
    AirBrushShape AirBrushShapeFromQString( const QString& str );

    /*
     * Given:
     *      an AirBrushShape `shape`
     *      a non-zero `radius`
     *      an RGBA `color`
     * saves an RGBA airbrush image with dimensions (1+2*radius)-by-(1+2*radius)
     * into output parameter `airbrush_image_out`.
     */
    void create_airbrush( QImage& airbrush_image_out, AirBrushShape shape, int radius, QRgb color );
    /*
     * Given:
     *     `image_to_modify` (input/output parameter): a QImage to paint into
     *     `airbrush_image`: a QImage with an RGBA airbrush image (guaranteed to be square and have odd dimensions).
     *     `center`: an integer x,y position in the image.
     * Modifies `image_to_modify` by spraying the airbrush into it.
     * Returns:
     *     QRect: the rectangle in the image that was modified.
     */
    QRect paint_at( QImage& image_to_modify,
                    const QImage& airbrush_image,
                    const QPoint& center
                    );
};

#endif /* __airbrush__h__ */
