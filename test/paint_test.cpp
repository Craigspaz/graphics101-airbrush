#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <cmath> // std::abs
#include "airbrush.h"

#include <iostream>

namespace graphics101 {
bool operator==( const ColorRGBA8& lhs, const ColorRGBA8& rhs ) {
    return
        lhs.r == rhs.r &&
        lhs.g == rhs.g &&
        lhs.b == rhs.b &&
        lhs.a == rhs.a
        ;
}

bool operator==( const Rect& lhs, const Rect& rhs ) {
    return
        lhs.x == rhs.x &&
        lhs.y == rhs.y &&
        lhs.width == rhs.width &&
        lhs.height == rhs.height
        ;
}
}

using namespace graphics101;

TEST_CASE( "PaintAt Test" ) {
    const ColorRGBA8 color( 255, 255, 255 );
    Image canvas( 4, 4, color );
    Image brush( 3, 3, color );
    
    /*
    from random import randint
    for x in range(4):
        for y in range(4):
            print( f'canvas.pixel({x},{y}) = ColorRGBA8( {randint(0,255)}, {randint(0,255)}, {randint(0,255)} );' )
    */
    canvas.pixel(0,0) = ColorRGBA8( 239, 127, 165 );
    canvas.pixel(0,1) = ColorRGBA8( 164, 73, 192 );
    canvas.pixel(0,2) = ColorRGBA8( 226, 101, 173 );
    canvas.pixel(0,3) = ColorRGBA8( 100, 186, 97 );
    canvas.pixel(1,0) = ColorRGBA8( 127, 164, 158 );
    canvas.pixel(1,1) = ColorRGBA8( 166, 83, 205 );
    canvas.pixel(1,2) = ColorRGBA8( 64, 2, 86 );
    canvas.pixel(1,3) = ColorRGBA8( 208, 113, 213 );
    canvas.pixel(2,0) = ColorRGBA8( 21, 247, 222 );
    canvas.pixel(2,1) = ColorRGBA8( 138, 67, 39 );
    canvas.pixel(2,2) = ColorRGBA8( 134, 68, 149 );
    canvas.pixel(2,3) = ColorRGBA8( 218, 116, 63 );
    canvas.pixel(3,0) = ColorRGBA8( 19, 189, 47 );
    canvas.pixel(3,1) = ColorRGBA8( 224, 160, 12 );
    canvas.pixel(3,2) = ColorRGBA8( 54, 235, 208 );
    canvas.pixel(3,3) = ColorRGBA8( 37, 127, 186 );
    
    /*
    from random import randint
    for x in range(3):
        for y in range(3):
            print( f'brush.pixel({x},{y}) = ColorRGBA8( {randint(0,255)}, {randint(0,255)}, {randint(0,255)}, {randint(0,255)} );' )
    */
    brush.pixel(0,0) = ColorRGBA8( 214, 31, 88, 83 );
    brush.pixel(0,1) = ColorRGBA8( 213, 1, 38, 34 );
    brush.pixel(0,2) = ColorRGBA8( 210, 242, 171, 73 );
    brush.pixel(1,0) = ColorRGBA8( 157, 60, 55, 82 );
    brush.pixel(1,1) = ColorRGBA8( 253, 175, 196, 247 );
    brush.pixel(1,2) = ColorRGBA8( 238, 220, 67, 114 );
    brush.pixel(2,0) = ColorRGBA8( 202, 34, 48, 101 );
    brush.pixel(2,1) = ColorRGBA8( 79, 231, 141, 142 );
    brush.pixel(2,2) = ColorRGBA8( 210, 90, 205, 16 );
    
    {
        Image canvas_copy = canvas;
        const Rect rect = paint_at( canvas_copy, brush, 0, 0 );
        
        // Generate the correct output on a solution build:
        /*
        for( int x = 0; x < canvas_copy.width(); x++ ) {
            for( int y = 0; y < canvas_copy.height(); y++ ) {
                std::cerr << "CHECK( std::abs( canvas_copy.pixel(" << x << "," << y << ").r - " << int(canvas_copy.pixel(x,y).r) << " ) <= 2 );" << std::endl;
            }
        }
        std::cerr << "CHECK( rect == Rect( " << rect.x << ", " << rect.y << ", " << rect.width << ", " << rect.height << " ) );" << std::endl;
        */
        
        CHECK( std::abs( canvas_copy.pixel(0,0).r - 253 ) <= 2 );
        CHECK( std::abs( canvas_copy.pixel(0,1).r - 197 ) <= 2 );
        CHECK( std::abs( canvas_copy.pixel(0,2).r - 226 ) <= 2 );
        CHECK( std::abs( canvas_copy.pixel(0,3).r - 100 ) <= 2 );
        CHECK( std::abs( canvas_copy.pixel(1,0).r - 100 ) <= 2 );
        CHECK( std::abs( canvas_copy.pixel(1,1).r - 169 ) <= 2 );
        CHECK( std::abs( canvas_copy.pixel(1,2).r - 64 ) <= 2 );
        CHECK( std::abs( canvas_copy.pixel(1,3).r - 208 ) <= 2 );
        CHECK( std::abs( canvas_copy.pixel(2,0).r - 21 ) <= 2 );
        CHECK( std::abs( canvas_copy.pixel(2,1).r - 138 ) <= 2 );
        CHECK( std::abs( canvas_copy.pixel(2,2).r - 134 ) <= 2 );
        CHECK( std::abs( canvas_copy.pixel(2,3).r - 218 ) <= 2 );
        CHECK( std::abs( canvas_copy.pixel(3,0).r - 19 ) <= 2 );
        CHECK( std::abs( canvas_copy.pixel(3,1).r - 224 ) <= 2 );
        CHECK( std::abs( canvas_copy.pixel(3,2).r - 54 ) <= 2 );
        CHECK( std::abs( canvas_copy.pixel(3,3).r - 37 ) <= 2 );
        
        CHECK( rect == Rect( 0, 0, 2, 2 ) );
    }
    
    {
        Image canvas_copy = canvas;
        const Rect rect = paint_at( canvas_copy, brush, 3, 3 );
        
        // Generate the correct output on a solution build:
        /*
        for( int x = 0; x < canvas_copy.width(); x++ ) {
            for( int y = 0; y < canvas_copy.height(); y++ ) {
                std::cerr << "CHECK( std::abs( canvas_copy.pixel(" << x << "," << y << ").r - " << int(canvas_copy.pixel(x,y).r) << " ) <= 2 );" << std::endl;
            }
        }
        std::cerr << "CHECK( rect == Rect( " << rect.x << ", " << rect.y << ", " << rect.width << ", " << rect.height << " ) );" << std::endl;
        */
        
        CHECK( std::abs( canvas_copy.pixel(0,0).r - 239 ) <= 2 );
        CHECK( std::abs( canvas_copy.pixel(0,1).r - 164 ) <= 2 );
        CHECK( std::abs( canvas_copy.pixel(0,2).r - 226 ) <= 2 );
        CHECK( std::abs( canvas_copy.pixel(0,3).r - 100 ) <= 2 );
        CHECK( std::abs( canvas_copy.pixel(1,0).r - 127 ) <= 2 );
        CHECK( std::abs( canvas_copy.pixel(1,1).r - 166 ) <= 2 );
        CHECK( std::abs( canvas_copy.pixel(1,2).r - 64 ) <= 2 );
        CHECK( std::abs( canvas_copy.pixel(1,3).r - 208 ) <= 2 );
        CHECK( std::abs( canvas_copy.pixel(2,0).r - 21 ) <= 2 );
        CHECK( std::abs( canvas_copy.pixel(2,1).r - 138 ) <= 2 );
        CHECK( std::abs( canvas_copy.pixel(2,2).r - 160 ) <= 2 );
        CHECK( std::abs( canvas_copy.pixel(2,3).r - 217 ) <= 2 );
        CHECK( std::abs( canvas_copy.pixel(3,0).r - 19 ) <= 2 );
        CHECK( std::abs( canvas_copy.pixel(3,1).r - 224 ) <= 2 );
        CHECK( std::abs( canvas_copy.pixel(3,2).r - 87 ) <= 2 );
        CHECK( std::abs( canvas_copy.pixel(3,3).r - 246 ) <= 2 );
        
        CHECK( rect == Rect( 2, 2, 2, 2 ) );
    }
}
