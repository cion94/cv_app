#include "smooth_image.h"

void SmoothImage(ImageMat *inImg, ImageMat* outImg)
{
	outImg->rows = inImg->rows;
	outImg->cols = inImg->cols;
	
	int size = (outImg->rows * outImg->cols);
	outImg->data = (BYTE*)malloc( size << 2 );
	memset( outImg->data, 0, size << 2 );

	int h[] = { 1, 4, 6, 4, 1 };
	int i, j, k, l;
	int outOffset, inOffset, dirOffset;

	int outV[3], Gx[3], Gy[3];

	// apply horizontal
	for( i = 0; i < inImg->rows; i++ )
	{
		for( j = 0; j < inImg->cols; j++ )
		{
			memset(&outV, 0, 12);
			outOffset = ( i * inImg->cols );

			for( k = -2; k <= 2; k++ )
			{
				inOffset = j + k;
				if( inOffset < 0 )
					inOffset = 0;
				if( inOffset >= inImg->cols )
					inOffset = inImg->cols - 1;

				inOffset = (outOffset + inOffset) << 2;
				outV[0] += inImg->data[ inOffset ]		* h[k + 2];
				outV[1] += inImg->data[ inOffset + 1 ]	* h[k + 2];
				outV[2] += inImg->data[ inOffset + 2 ]	* h[k + 2];
			}

			outOffset = (outOffset + j) << 2;
			outImg->data[outOffset]		= (BYTE)(outV[0] >> 4);
			outImg->data[outOffset + 1] = (BYTE)(outV[1] >> 4);
			outImg->data[outOffset + 2] = (BYTE)(outV[2] >> 4);
		}
	}

	// apply vertical
	for( i = 0; i < inImg->rows; i++ )
	{
		for( j = 0; j < inImg->cols; j++ )
		{
			memset(&outV, 0, 12);

			for( k = -2; k <= 2; k++ )
			{
				inOffset = i + k;
				if( inOffset < 0 )
					inOffset = 0;
				if( inOffset >= inImg->rows )
					inOffset = inImg->rows - 1;

				inOffset = ( inOffset * inImg->cols + j ) << 2;
				outV[0] += outImg->data[ inOffset ]		* h[k + 2];
				outV[1] += outImg->data[ inOffset + 1 ]	* h[k + 2];
				outV[2] += outImg->data[ inOffset + 2 ]	* h[k + 2];
			}

			outOffset = ( i * inImg->cols + j ) << 2;
			inImg->data[outOffset]		= (BYTE)(outV[0] >> 4);
			inImg->data[outOffset + 1]	= (BYTE)(outV[1] >> 4);
			inImg->data[outOffset + 2]	= (BYTE)(outV[2] >> 4);
		}
	}

	memcpy( outImg->data, inImg->data, size << 2 );
}