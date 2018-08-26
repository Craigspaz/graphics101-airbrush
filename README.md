Computer Graphics - Homework Assignment 1 - Airbrush
====================================================

Goals:
------

* Understand raster images and simple compositing.

* Become familiar with C++.

* Successfully install the Qt development environment.

Getting Started & Handing In:
-----------------------------

* This is a programming assignment. You will need to download the code
framework from Piazza.

* The code will be written in C++, but nothing fancy. It will look like a
mix of C and Java.

* The code is written using the open source Qt GUI framework. You will
only need to understand a few functions, described below.

* The assignments make use of the cross-platform, open source Qt development environment,
which comes with a great IDE. Install the open source version
of the Qt environment: <https://www.qt.io/download-open-source>
(At the time of writing, version 5.11 is the newest version. Any 5.x
version should work.) This installer, by default, includes all versions of
Qt. This is unnecessary and takes a huge amount of space.
Install only the most recent version (e.g. Qt 5.11) and the Qt Creator IDE.
On Windows, also install the MingW compiler.

  * Mac users can alternatively install with [Homebrew](https://brew.sh/): `brew install qt` and `brew cask install qt-creator`.
  * Ubuntu Linux users can alternatively install via `apt-get install qtbase5-dev qtcreator`.

* Download the assignment. This will create a folder named `airbrush`. Open the
file named `airbrush.pro`. This should launch the Qt Creator
development environment (IDE).

* Build and run the code. The code should compile and run, but clicking
and dragging in the window won't have any effect.

* Add your code to `airbrush.cpp`.

* Build and run and test that it is working correctly. Qt Creator has a
great debugger.

* Create a painting and save it into the folder.

* When done, zip your entire `airbrush` directory along with a
*Notes.txt* file and your painting as *hw01_lastname_firstname.zip* and
upload your solution to Blackboard before the deadline. Your *Notes.txt*
should describe any known issues or extra features. Your *Notes.txt*
should also note the names of people in the class who deserve a star for
helping you (not by giving your their code!).

* The framework and libraries provide all the support code that you need.
Feel free to enhance the interface if you so desire.

* **THIS IS AN INDIVIDUAL, NOT A GROUP ASSIGNMENT. That means all code
written for this assignment should be original! Although you are
permitted to consult with each other while working on this assignment,
code that is substantially the same will be considered cheating.** In your
`Notes.txt`, please note who deserves a star (who helped you with the
assignment).

Overview:
---------

In this assignment, you will be implementing a digital painting tool
similar to the airbrush in Photoshop (and better than the painting tools
in MS Paint). You will be able to create stunning artwork like
this:

![van art beach scene](docs/images/beach.png)

Although it looks fancy, it's pretty simple if we break it down into two
parts: (1) creating a translucent RGBA image for a spray of the airbrush
and (2) depositing the paint by compositing the airbrush RGBA image onto
the background image centered at the mouse location.

Rubric:
-------

1. **(50 points)** Creating a translucent RGBA image for a spray of the
airbrush. An airbrush is defined by its color, radius, and shape.
Here are some examples:

    * Shape: Quadratic  
    Radius: 25 pixels  
    Color: (0, 0, 255, 64) aka blue at 25% opacity  
    ![quadratic blue brush](docs/images/airbrush_quadratic.png)
    
    * Shape: Linear  
    Radius: 25 pixels  
    Color: (0, 0, 255, 64) aka blue at 25% opacity  
    ![linear blue brush](docs/images/airbrush_linear.png)

    * Shape: Constant  
    Radius: 25 pixels  
    Color: (0, 0, 255, 64) aka blue at 25% opacity  
    ![constant blue brush](docs/images/airbrush_constant.png)


    The algorithm to generate any of these airbrush images is very similar.
We are given an RGBA color, which means it has red, green, blue, and
alpha components. In our code, an RGBA color takes up 32-bits. Each component
will be stored as one of its four bytes. That means that each component will be
an 8-bit integer whose value lies in the range 0 to 255. It is sometimes
convenient to think of components as lying in the continuous range
[0,1], in which case you would divide the 8-bit component values by 255.
An airbrush image is a square RGBA image with dimensions
`2*radius+1` by `2*radius+1`. Each pixel has the same RGB values taken
directly from the color but varying A (alpha) or opacity. In the brushes
above, alpha is at its maximum at the center pixel and falls off to 0
for pixels whose distance is radius or greater from the center. The
brushes above differ only in how the opacity falls off. To compute the
opacity for a pixel, let's set a variable *t* to the distance from the
center pixel as a fraction of the radius:

    *t* = sqrt( ( x - x₀)² + ( y - y₀ )² ) )/radius
    
    ![](docs/images/t.png)
    
    $t = \frac{ \sqrt{ ( x - x_0 )^2 + ( y - y_0 )^2 } }{ \textit{radius} }$

    Then all we need is a *falloff* function of *t* to create interesting
circular shapes. With such a function, the each airbrush image pixel's
alpha value is then conveniently expressed as color's alpha * *f(t)*.

    * **(25 points)** Writing the code that iterates over every x,y pixel of
the brush image and sets its RGBA components appropriately. That
function signature is:

            void create_airbrush(
                QImage& airbrush_out,
                AirBrushShape shape, int radius, QRgb color
                )

        `QImage` is Qt's image class that wraps an array of RGBA values with
convenience methods. The `&` is C++'s way of passing by reference (it's
like a dereferenced pointer in C). `AirBrushShape` is an `enum`. You will
assign every x,y pixel of the output parameter `airbrush_out`. x ranges
from 0 to `airbrush_out.width()` and y ranges from 0 to
`airbrush_out.height()`. You can assign pixels either by calling
`airbrush_out.setPixel()` or, if you want to work closer to the metal, by
accessing the pointer to the array of pixels directly via
`airbrush_out.scanLine()` (see below). To access the appropriate falloff function (the
ones you write, see below), call the helper function `falloff( shape, t )`.

    * **(25 points)** Implement *falloff* functions for the following three
shapes.

        ![plot of falloff functions](docs/images/falloffs.png)

        * **(5 of the 25 points)** A constant function `falloff_constant()`. It just
makes a circle. (These equations are shown 3 ways because GitHub's Markdown processing doesn't handle equations well.)

            ![constant function](docs/images/constant.png)

            $$
            f_\textit{constant}(t) =
            \begin{cases}
            1 & \text{if} \quad t < 1 \\
            0 & \text{otherwise}
            \end{cases}
            $$

                falloff_constant(t) =
                    { 1 if t < 1
                    { 0 otherwise

        * **(10 of the 25 points)** A linear function `falloff_linear()`.
It makes a "cone".

            ![linear function](docs/images/linear.png)

            $$
            f_\textit{linear}(t) =
            \begin{cases}
            1-t & \text{if} \quad t < 1 \\
            0 & \text{otherwise}
            \end{cases}
            $$

                falloff_linear(t) =
                    { 1-t if t < 1
                    { 0   otherwise

        * **(10 of the 25 points)** A quadratic function `falloff_quadratic()` that
looks like a Gaussian (very aesthetically pleasing) but without an
infinite tail.

            ![quadratic function](docs/images/quadratic.png)

            $$
            f_\textit{quadratic}(t) =
            \begin{cases}
            1-3t^2 & \text{if\ \ \ } t < \frac{1}{3} \\
            1.5t^2 - 3t + 1.5 & \text{if\ \ \ } \frac{1}{3} \leq t < 1 \\
            0 & \text{otherwise}
            \end{cases}
            $$

                falloff_quadratic(t) =
                    { 1-3t^2            if t < 1/3
                    { 1.5t^2 - 3t + 1.5 if 1/3 <= t < 1
                    { 0                 otherwise

        * **(up to 5 bonus points)** A special function `falloff_special()` that does
anything you want!

2. **(50 points)** Depositing the paint by compositing the airbrush RGBA
image onto the background image centered at the mouse location. The
function signature for this is:

        QRect paint_at(
            QImage& image_to_modify,
            const QImage& airbrush_image, const QPoint& center
            )

    You will modify a subset of the pixels of the output parameter
`image_to_modify` by compositing the pixels of `airbrush_image` over
them. `airbrush_image` should be composited with its center at the
`center.x()`, `center.y()` pixel of `image_to_modify`. Note that depending
on the size of `airbrush_image` and the location of its center, only part
of it will fit on `image_to_modify`. Figuring out which pixels of
`airbrush_image` and `image_to_modify` to iterate over—without
accessing out-of-bounds memory—is the first step of this function.
You will return the rectangle corresponding to the pixels of `image_to_modify`
that you actually modified.

    * **(25 of the 50 points)** Correct iteration bounds. Note that you must
return the iteration bounds related to `image_to_modify` as a `QRect`.
This is so that the GUI knows which part of the window to redraw.

    * **(25 of the 50 points)** Compositing the corresponding RGBA pixel of
`airbrush_image` onto the RGB pixel of `image_to_modify`. To help you
with this, I have created a helper function signature (you fill it in):

            QRgb composite( const QRgb& foreground, const QRgb& background );

        Recall from class or the textbook or Wikipedia that the formula to
composite a foreground color "over" a background color is:

        *C.red = F.alpha * F.red + (1 - F.alpha) * B.red*

        where *C* is the output composited pixel, *F* is the foreground pixel,
and *B* is the background pixel. The formula is the same for the green
and blue channels. In this formula, alpha values range from [0,1], not
[0,255], so don't forget to convert. (Note that if the background
image were also transparent, the formula would be slightly different.)
The formula `(1 - t)*a + t*b` appears so frequently in math and computer
science that it has a shorthand name: lerp, short for linear
interpolation. You may find it useful to fill in and use a helper
function. The signature is provided:

            int lerp( int a, int b, real t )

        `real` is a typedef for a floating point number. It is a requirement that
you fill in and make use of `composite()`. It is not a requirement that
you fill in and use `lerp()`.

    * **(5 bonus points)** Write composite (and lerp) without using floating
point numbers. Floating point operations have traditionally been slower than integer operations. You will have to modify the function signature for lerp to

            int lerp( int a, int b, int t )

        to accomplish this.

Qt functions you need for this assignment
-----------------------------------------

**QRect.** The constructor of `QRect` is `QRect( x, y, width, height )`.

**QPoint.** The methods `p.x()` and `p.y()` return the x and y components of
a `QPoint p`.

**QImage:**

* `image.pixel(x,y)` returns the `QRgb` color for pixel x,y of a `QImage image`.
* `image.setPixel(x,y,c)` sets the pixel to a `QRgb` color `c`.

* `image.width()` and `image.height()` return the width and height of the
image.

* (Optional) `image.scanLine(y)` returns a pointer to the array of `QRgb` pixel data for
row y. You do not need to do this but may wish to get closer to the metal for
performance (avoiding function calls) or the experience. If you wish to attempt this,
first make your code work using `.pixel()` and `.setPixel(x,y,c)`.
The pointer returned by `image.scanLine(y)` points to the pixel (0,y).
If you have a pointer to a pixel `QRgb* pix`, the next pixel in the row is `pix+1`
and the next pixel in the column is `pix+image.bytesPerLine()/4`.

`sqrt(x)`, `std::min(a,b)`, `std::max(a,b)`. These are not Qt functions.
They are part of C's `math.h` and C++'s <algorithm>. Nevertheless, you
will find them useful. Note that `std::min` and `std::max` require both
parameters to have the exact same type. If not, you will get a very long
compiler error since they are generic functions written using C++
templates.

**QRgb.** To get the red, green, blue, and alpha components of a QRgb
color `c` as 8-bit values, use `qRed(c)`, `qGreen(c)`, `qBlue(c)`, and
`qAlpha(c)`. In this assignment, we are ignoring alpha. To create an RGB
`QRgb` color, use `qRgb( red, green, blue )` with 8-bit parameters. Note
that `QRgb` is not a class or a struct. It is a `typedef` for an `unsigned
int`, and those functions are just getting and setting the appropriate
bytes. The header `qrgb.h` is very short and readable. Here is most of
it:

```c++
typedef unsigned int QRgb;                        // RGB triplet

inline int qRed(QRgb rgb)                // get red part of RGB
{ return ((rgb >> 16) & 0xff); }

inline int qGreen(QRgb rgb)                // get green part of RGB
{ return ((rgb >> 8) & 0xff); }

inline int qBlue(QRgb rgb)                // get blue part of RGB
{ return (rgb & 0xff); }

inline int qAlpha(QRgb rgb)                // get alpha part of RGBA
{ return rgb >> 24; }

inline QRgb qRgb(int r, int g, int b)// set RGB value
{ return (0xffu << 24) | ((r & 0xffu) << 16) | ((g & 0xffu) << 8) | (b & 0xffu); }

inline QRgb qRgba(int r, int g, int b, int a)// set RGBA value
{ return ((a & 0xffu) << 24) | ((r & 0xffu) << 16) | ((g & 0xffu) << 8) | (b & 0xffu); }
```
