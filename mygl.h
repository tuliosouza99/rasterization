#ifndef _MYGL_H_
#define _MYGL_H_

#include <cstdlib>
#include <math.h>
#include <algorithm>

#include "definitions.h"
#include "pixel.h"

//*****************************************************************************
// Defina aqui as suas funções gráficas
//*****************************************************************************

void PutPixel( Pixel *pixel )
{
    FBptr[(pixel->x * 4) + (pixel->y * IMAGE_WIDTH * 4) + 0] = pixel->r;
    FBptr[(pixel->x * 4) + (pixel->y * IMAGE_WIDTH * 4) + 1] = pixel->g;
    FBptr[(pixel->x * 4) + (pixel->y * IMAGE_WIDTH * 4) + 2] = pixel->b;
    FBptr[(pixel->x * 4) + (pixel->y * IMAGE_WIDTH * 4) + 3] = pixel->a;
}

void Interpolate( Pixel *final, Pixel *start, Pixel *end, int currentStep, double finalStep )
{
    float t = (currentStep / finalStep);
    final->r = (end->r - start->r) * t + start->r;
    final->g = (end->g - start->g) * t + start->g;
    final->b = (end->b - start->b) * t + start->b;
    final->a = (end->a - start->a) * t + start->a;
}

void DrawLine( Pixel *iPixel, Pixel *fPixel )
{
    int dx, dy, d, incr_e, incr_ne, currentStep;
    Pixel newPixel = *iPixel;

    dx = abs( fPixel->x - iPixel->x );
    dy = abs( fPixel->y - iPixel->y );

    double finalStep = sqrt( (dx * dx) + (dy * dy) );
    currentStep = 0;

    PutPixel( &newPixel );
    if ( dx >= dy )
    {
        incr_e = 2 * dy;
        incr_ne = 2 * (dy - dx);

        d = 2 * dy - dx;
        while ( newPixel.x != fPixel->x )
        {
            if ( d <= 0 ) {
                d += incr_e;
            } else {
                d += incr_ne;
                if ( fPixel->y >= iPixel->y ) {
                    newPixel.y++;
                } else {
                    newPixel.y--;
                }
            }

            if ( fPixel->x >= iPixel->x ) {
                newPixel.x++;
            } else {
                newPixel.x--;
            }

            Interpolate( &newPixel, iPixel, fPixel, currentStep, finalStep );
            PutPixel( &newPixel );
            ++currentStep;
        }
    } 
    else
    {
        incr_e = 2 * dx;
        incr_ne = 2 * (dx - dy);

        d = 2 * (dx - dy);
        while ( newPixel.y != fPixel->y )
        {
            if ( d <= 0 ) {
                d += incr_e;
            } else {
                d += incr_ne;
                if ( fPixel->x >= iPixel->x ) {
                    newPixel.x++;
                } else {
                    newPixel.x--;
                }
            }

            if ( fPixel->y >= iPixel->y ) {
                newPixel.y++;
            } else {
                newPixel.y--;
            }

            Interpolate( &newPixel, iPixel, fPixel, currentStep, finalStep );
            PutPixel( &newPixel );
            ++currentStep;
        }
    }    
    
}

void DrawTriangle( Pixel *a, Pixel *b, Pixel *c )
{
    DrawLine( a, b );
    DrawLine( b, c );
    DrawLine( c, a );
}

void DrawFilledTriangle( Pixel *a, Pixel *b, Pixel *c, Pixel *color )
{
    // // Compute vectors        
    // v0 = C - A
    // v1 = B - A
    // v2 = P - A

    // // Compute dot products
    // dot00 = dot(v0, v0)
    // dot01 = dot(v0, v1)
    // dot02 = dot(v0, v2)
    // dot11 = dot(v1, v1)
    // dot12 = dot(v1, v2)

    // // Compute barycentric coordinates
    // invDenom = 1 / (dot00 * dot11 - dot01 * dot01)
    // u = (dot11 * dot02 - dot01 * dot12) * invDenom
    // v = (dot00 * dot12 - dot01 * dot02) * invDenom

    // // Check if point is in triangle
    // return (u >= 0) && (v >= 0) && (u + v < 1)

    // getting the bounding boc of the triangle
    int maxX = std::max( a->x, std::max( b->x, c->x ));
    int minX = std::min( a->x, std::min( b->x, c->x ));
    int maxY = std::max( a->y, std::max( b->y, c->y ));
    int minY = std::min( a->y, std::min( b->y, c->y ));

    // vectors of edge (a, b) and (a, c)
    Pixel vs1 = Pixel( (b->x - a->x), (b->y - a->y), 1, 1, 1, 1 );
    Pixel vs2 = Pixel( (c->x - a->x), (c->y - a->y), 1, 1, 1, 1 );

    for ( int x = minX; x <= maxX; ++x ) {
        for ( int y = minY; y <= maxY; ++y ) {
            Pixel q = Pixel( (x - a->x), (y - a->y), color->r, color->g, color->b, color->a );

            float s = (float) ((q.x - vs2.y) * (vs2.x - q.y)) / (float) ((vs1.x - vs2.y) * (vs2.x - vs1.y));
            float t = (float) ((vs1.x - q.y) * (q.x - vs1.y)) / (float) ((vs1.x - vs2.y) * (vs2.x - vs1.y));

            if ( (s >= 0) && (t >= 0) && (s + t <= 1) ) {
                Pixel newPixel = Pixel( x, y, color->r, color->g, color->b, color->a );
                PutPixel( &newPixel );
            }
        }
    }

    // if ( c->x < b->x ) {
    //     while ( b->x != c->x ) {
    //         DrawLine( a, b );
    //         b->x--;
    //     }
    // } else {
    //     while ( b->x !=c->x ) {
    //         DrawLine( a, b );
    //         b->x++;
    //     }
    // }

    // if ( (a->x <= b->x) && (a->y <= b->y) ) {
    //     while ( b->x != a->x ) {
    //         DrawLine( c, b );
    //         b->x--;
    //         b->y--;
    //     }
    // } else if ( (a->x >= b->x) && (a->y >= b->y) ) {
    //     while ( b->x != a->x ) {
    //         DrawLine( c, b );
    //         b->x++;
    //         b->y++;
    //     }
    // } else if ( (a->x <= b->x) && (a->y >= b->y) ) {
    //     while ( b->x != a->x ) {
    //         DrawLine( c, b );
    //         b->x--;
    //         b->y++;
    //     }
    // } else if ( (a->x >= b->x) && (a->y <= b->y) ) {
    //     while ( b->x != a->x ) {
    //         DrawLine( c, b );
    //         b->x++;
    //         b->y--;
    //     }
    // }
}

void DesenhaPixels( void )
{
    // Escreve um pixel vermelho na posicao (0,0) da tela:
	FBptr[0] = 255;
	FBptr[1] = 0;
	FBptr[2] = 0;
	FBptr[3] = 255;

	// Escreve um pixel verde na posicao (1,0) da tela:
	FBptr[4] = 0;
	FBptr[5] = 255;
	FBptr[6] = 0;
	FBptr[7] = 255;

	// Escreve um pixel azul na posicao (2,0) da tela:
	FBptr[8] = 0;
	FBptr[9] = 0;
	FBptr[10] = 255;
	FBptr[11] = 255;
}

void DesenhaLinha( void )
{
    for (unsigned int i=0; i<250; i++)
	{
	FBptr[4*i + 4*i*IMAGE_WIDTH + 0] = 255;
	FBptr[4*i + 4*i*IMAGE_WIDTH + 1] = 0;
	FBptr[4*i + 4*i*IMAGE_WIDTH + 2] = 255;
	FBptr[4*i + 4*i*IMAGE_WIDTH + 3] = 255;
	}
}


#endif // _MYGL_H_

