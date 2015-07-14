#include "Image.h"

void ApplyKernel( ImageMat *in, ImageMat* out, ImageMat* kernel, int normFact )
{
	unsigned int i, j, k, l, kf, lf, component;
	unsigned int ii, jj, maskCenterR, maskCenterC;
	int outOffset, inOffset, kernelOffset;
	int outV[4];

	if( in == NULL || out == NULL || kernel == NULL)
		return;
	if( in->data == NULL || out->data == NULL || kernel->data == NULL )
		return;

	memset( out->data, 0, (out->cols * out->rows) << 2 );

	maskCenterR = kernel->rows >> 1;
	maskCenterC = kernel->cols >> 1;

	for( i = 0; i < out->rows; i++ )
	{
		for( j = 0; j < out->cols; j++ )
		{
			outOffset = (i * out->cols + j) << 2;
			memset(&outV, 0, sizeof(int)*4);

			for( k = 0; k < kernel->rows; k++ )
			{
				kf = kernel->rows - 1 - k;
				for( l = 0; l < kernel->cols; l++ )
				{
					lf = kernel->cols - 1 - l;

					ii = i + (k - maskCenterR);
					jj = j + (l - maskCenterC);

					if( ii >= 0 && ii < in->rows  && jj >= 0 && jj < in->cols)
					{
						inOffset = (ii * in->cols + jj) << 2;
						kernelOffset = (kf * kernel->cols + lf);

						for( component = 0; component < 3; component++ )
						{
							outV[component] += in->data[inOffset + component] * (int)(*((int*)kernel->data + kernelOffset));
						}
					}
				}
			}
			//normalize components
			for( component = 0; component < 3; component++ )
			{
				outV[component] = ( outV[component] / normFact );
				if(outV[component] < 0) outV[component] = 0;
				if(outV[component] > 255) outV[component] = 255;
				out->data[outOffset + component] = (unsigned char)outV[component];
			}
		}
	}
}

void LoadImageMat(const char* filename, ImageMat* outMat )
{
	FIBITMAP* hImg = FreeImage_Load( FIF_PNG, filename );

	ImageMat src;
	src.rows = FreeImage_GetHeight(hImg);
	src.cols = FreeImage_GetWidth(hImg);

	src.data  = (BYTE*)FreeImage_GetBits(hImg);
	int depth = FreeImage_GetBPP(hImg);
	depth = depth >> 3;

	outMat->cols = src.cols;
	outMat->rows = src.rows;

	int size = (outMat->rows * outMat->cols);
	outMat->data = (BYTE*)malloc( size << 2 );
	memset(outMat->data, 255, size << 2);

	int i, srcOffset, outOffset, k;
	for( i = 0; i < size; i++ )
	{
		srcOffset = i * depth;
		outOffset = i << 2;

		for( k = 0; k < depth; k++ )
		{
			outMat->data[ outOffset + k ] = src.data[srcOffset + k];
		}
	}

	FreeImage_Unload(hImg);
}
void SaveImageMat(const char* filename, ImageMat* imgMat )
{
	FIBITMAP* hImg = FreeImage_Allocate( imgMat->cols, imgMat->rows, 32 );
	memcpy(FreeImage_GetBits(hImg), imgMat->data, imgMat->cols * imgMat->rows * 4);
	FreeImage_Save( FIF_PNG, hImg, filename);
	FreeImage_Unload(hImg);
}
void FreeImageMat( ImageMat* img )
{
	free(img->data);
}