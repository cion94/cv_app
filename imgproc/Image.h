#ifndef IMAGE_H
#define IMAGE_H

#include "freeimage/FreeImage.h"
#include <iostream>

//24 bit and 32 bit only

struct ImageMat
{
	unsigned int rows, cols;
	BYTE* data;
};

void ApplyKernel( ImageMat* in, ImageMat* out, ImageMat* kernel, int normFact );
void GetMatFromImage( ImageMat* src, FIBITMAP* hImg );

#endif
