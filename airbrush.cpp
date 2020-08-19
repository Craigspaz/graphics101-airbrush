#include "airbrush.h"

#include <cassert>
#include <cmath> // sqrt()
#include <algorithm> // std::min(), std::max()

// Put helper functions needed by this file only here in an anonymous namespace.
namespace {
    typedef double real;
    
    using graphics101::AirBrushShape;
    using graphics101::ColorRGBA8;
    
    /*
     * Falloff functions take a parameter `t`, guaranteed to be greater than or equal to zero,
     * and return a number between 0 and 1.
     * The return value should be 1 at the center and fall off to 0 when t >= 1.
     */
    real falloff_constant( real t )
    {
        // Your code goes here.

        assert( t >= 0. );

        return 1;
    }

    real falloff_linear( real t )
    {
        // Your code goes here.

        assert( t >= 0. );

        return 1;
    }

    real falloff_quadratic( real t )
    {
        // Your code goes here.

        assert( t >= 0. );

        return 1;
    }

    real falloff_special( real t )
    {
        // Your code goes here.

        assert( t >= 0. );

        return 1.;
    }

    real falloff( AirBrushShape shape, real t )
    {
        using namespace graphics101;

        switch( shape ) {
            case Constant: return falloff_constant(t);
            case Linear: return falloff_linear(t);
            case Quadratic: return falloff_quadratic(t);
            case Special: return falloff_special(t);
            default:
                assert(!"Invalid AirBrushShape. We shouldn't be here.");
                return -1.;
        }
    }

    inline int lerp( int a, int b, real t ) {
        // Your code goes here.

        // Don't forget to round floating point numbers whose decimal part is >= 0.5 up.
        return 0;
    }

    inline ColorRGBA8 composite( const ColorRGBA8& foreground, const ColorRGBA8& background ) {
        // Your code goes here.

        return ColorRGBA8(
                    // red
                    0,
                    // green
                    0,
                    // blue
                    0
                    );
    }
}

namespace graphics101
{
    void create_airbrush( Image& airbrush_out, AirBrushShape shape, int radius, ColorRGBA8 color )
    {
        assert( radius >= 0 );

        // Step 1. Allocate space for the mask. Make a color image with 8 bits for red, green, blue, and alpha.
        const int size = 1+2*radius;
        if( airbrush_out.width() != size || airbrush_out.height() != size ) {
            airbrush_out.resize( size, size );
        }

        // Step 2. Set the RGB contents of the image to the `color`'s RGB values.
        //         Set the alpha channel of the image to `color.alpha` scaled by the appropriate falloff.

        // Your code goes here.


        // For debugging, this may be useful:
        // airbrush_out.save( "airbrush.png" );
        // On a Mac, the file may be in "airbrush.app/Contents/MacOS/airbrush.png".
        // You can right-click or control-click on "airbrush.app"
        // and choose "Show Package Contents" to see inside.
    }

    Rect paint_at( Image& image_to_modify,
                    const Image& airbrush_image,
                    int center_x,
                    int center_y
                    )
    {

        // Your code goes here.

        assert( airbrush_image.width() % 2 == 1 );
        assert( airbrush_image.height() % 2 == 1 );
        assert( airbrush_image.width() == airbrush_image.height() );

        const int radius = airbrush_image.width()/2;

        // Step 1. Compute the start and end offsets into `image_to_modify` and `airbrush_image`
        //         that you will iterate over.

        // Step 2. Modify `image_to_modify` pixels by compositing `airbrush_image`.

        // Step 3. Return the rectange dimensions of the part of `image_to_modify` that was modified
        //         as a Rect.
        return Rect( 0, 0, 1, 1 );
    }

    std::string StringFromAirBrushShape( AirBrushShape shape ) {
        switch( shape ) {
            case Constant: return "Constant";
            case Linear: return "Linear";
            case Quadratic: return "Quadratic";
            case Special: return "Special";
            default:
                assert(!"Invalid AirBrushShape. We shouldn't be here.");
                return "Special";
        }
    }

    AirBrushShape AirBrushShapeFromString( const std::string& str ) {
        if( str == "Constant" ) return Constant;
        else if( str == "Linear" ) return Linear;
        else if( str == "Quadratic" ) return Quadratic;
        else if( str == "Special" ) return Special;
        else {
            assert(!"Invalid AirBrushShape. We shouldn't be here.");
            return Special;
        }
    }
}
