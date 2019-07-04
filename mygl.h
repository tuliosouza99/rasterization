#ifndef _MYGL_H_
#define _MYGL_H_

#include <cstdlib>
#include <math.h>

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

void DrawFilledTriangle( Pixel *a, Pixel *b, Pixel *c )
{
    if ( a->x < b->x ) {
        while ( b->x != a->x ) {
            DrawLine( c, b );
            b->x--;
        }
    } else {
        while ( b->x !=a->x ) {
            DrawLine( c, b );
            b->x++;
        }
    }
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

