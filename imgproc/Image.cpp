#include "Image.h"


void Conv( Mat *in, Mat* out, Mat* mask, int normFact )
{
	unsigned int i, j, k, l, kf, lf;
	unsigned int ii, jj, maskCenterR, maskCenterC;

	if( in == NULL || out == NULL || mask == NULL)
		return;
	if( in->data == NULL || out->data == NULL || mask->data == NULL )
		return;

	memset(out->data, 0, out->cols * out->rows * out->depth);

	maskCenterR = mask->rows >> 1;
	maskCenterC = mask->cols >> 1;

	for( i = 0; i < out->rows; i++ )
	{
		for( j = 0; j < out->cols; j++ )
		{
			int outOffset = (i * out->cols + j) * out->depth;
			int outV[4];
			memset(&outV, 0, sizeof(int)*4);

			for( k = 0; k < mask->rows; k++ )
			{
				kf = mask->rows - 1 - k;
				for( l = 0; l < mask->cols; l++ )
				{
					lf = mask->cols - 1 - l;

					ii = i + (k - maskCenterR);
					jj = j + (l - maskCenterC);

					if( ii >= 0 && ii < in->rows  && jj >= 0 && jj < in->cols)
					{
						//out->data[i][j] += in->data[ii][jj] *  mask->data[kf][lf];
						int inOffset = (ii * in->cols + jj) * in->depth;
						int maskOffset = (kf * mask->cols + lf);

						for( int component = 0; component < out->depth; component++ )
						{
							//out->data[outOffset + d] += in->data[inOffset + d] * (int)mask->data[maskOffset];
							outV[component] += in->data[inOffset + component] * (int)(*((int*)mask->data + maskOffset));
						}
					}
				}
			}
			//normalize components
			for( int component = 0; component < out->depth; component++ )
			{
				outV[component] = ( outV[component] / normFact );
				if(outV[component] > 255) outV[component] = 255;
				if(outV[component] < 0) outV[component] = 0;
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
