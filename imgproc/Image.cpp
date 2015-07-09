#include "Image.h"


void ApplyKernel( Mat *in, Mat* out, Mat* kernel, int normFact )
{
	unsigned int i, j, k, l, kf, lf, component;
	unsigned int ii, jj, maskCenterR, maskCenterC;
	int outOffset, inOffset, kernelOffset;
	int outV[4];

	if( in == NULL || out == NULL || kernel == NULL)
		return;
	if( in->data == NULL || out->data == NULL || kernel->data == NULL )
		return;

	memset(out->data, 0, out->cols * out->rows * out->depth);

	maskCenterR = kernel->rows >> 1;
	maskCenterC = kernel->cols >> 1;

	for( i = 0; i < out->rows; i++ )
	{
		for( j = 0; j < out->cols; j++ )
		{
			outOffset = (i * out->cols + j) * out->depth;
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
						inOffset = (ii * in->cols + jj) * in->depth;
						kernelOffset = (kf * kernel->cols + lf);

						for( component = 0; component < out->depth; component++ )
						{
							outV[component] += in->data[inOffset + component] * (int)(*((int*)kernel->data + kernelOffset));
						}
					}
				}
			}
			//normalize components
			for( component = 0; component < out->depth; component++ )
			{
				outV[component] = ( outV[component] / normFact );
				if(outV[component] < 0) outV[component] *= -1;
				if(outV[component] > 255) outV[component] = 255;
				out->data[outOffset + component] = (unsigned char)outV[component];
			}
		}
	}
}

void GetMatFromImage( Mat* src, FIBITMAP* hImg )
{
	src->rows = FreeImage_GetHeight(hImg);
	src->cols = FreeImage_GetWidth(hImg);

	src->data  = (BYTE*)FreeImage_GetBits(hImg);
	src->depth = FreeImage_GetBPP(hImg);
	src->depth = src->depth >> 3;
}
