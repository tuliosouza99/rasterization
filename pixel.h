#ifndef GUARD_PIXEL_H_
#define GUARD_PIXEL_H_

class Pixel
{
public:
    int x, y;
    int r, g, b, a;

    Pixel( int x, int y, int r, int g, int b, int a ) : x( x ),
                                                        y( y ),
                                                        r( r ),
                                                        g( g ),
                                                        b( b ),
                                                        a( a )
    { }

};

#endif // GUARD_PIXEL_H_