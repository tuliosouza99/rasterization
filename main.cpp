#include "main.h"

//-----------------------------------------------------------------------------
void MyGlDraw(void)
{
	//*************************************************************************
	// Chame aqui as funções do mygl.h
	//*************************************************************************

	Pixel fPixel = Pixel( 255, 100, 255, 0, 0, 255);
	Pixel iPixel = Pixel( 129, 300, 0, 255, 0, 255);
	Pixel mPixel = Pixel( 385, 300, 0, 0, 255, 255);

	DrawTriangle( &mPixel, &iPixel , &fPixel );
}

//-----------------------------------------------------------------------------
int main(int argc, char **argv)
{
	// Inicializações.
	InitOpenGL(&argc, argv);
	InitCallBacks();
	InitDataStructures();

	// Ajusta a função que chama as funções do mygl.h
	DrawFunc = MyGlDraw;	

	// Framebuffer scan loop.
	glutMainLoop();

	return 0;
}

