#ifndef IMAGE_H
#define IMAGE_H

#include "freeimage/FreeImage.h"
#include <iostream>

//24 bit and 32 bit only

struct Mat
{
	unsigned int rows, cols;
	BYTE* data;
	int depth;
};

void Conv( Mat* in, Mat* out, Mat* mask, int normFact );
void GetMatFromImage( Mat* src, FIBITMAP* hImg );

#endif
