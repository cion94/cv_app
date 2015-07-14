#ifndef IMAGE_H
#define IMAGE_H

#include "freeimage/FreeImage.h"
#include <iostream>

struct ImageMat
{
	unsigned int rows, cols;
	BYTE* data;
};

void ApplyKernel( ImageMat* in, ImageMat* out, ImageMat* kernel, int normFact );
void LoadImageMat( const char* filename, ImageMat* imgMat );
void SaveImageMat( const char* filename, ImageMat* imgMat );
void FreeImageMat( ImageMat* img );

#endif
