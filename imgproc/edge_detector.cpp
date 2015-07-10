#include "edge_detector.h"

void DetectEdges( ImageMat* inImg, ImageMat* outImg )
{
	outImg->rows = inImg->rows;
	outImg->cols = inImg->cols;
	
	int size = (outImg->rows * outImg->cols);
	outImg->data = (BYTE*)malloc( size << 2 );
	memset( outImg->data, 0, size << 2 );

	//smooth image
	int h[] = { 1, 4, 6, 4, 1 };
	int i, j, k, l;
	int offset;
	int outV[3];

	// apply horizontal
	for( i = 0; i < inImg->rows; i++ )
	{
		for( j = 0; j < inImg->cols - 5; j++ )
		{
			offset = ( i * inImg->cols + j );
			memset(&outV, 0, 12);
			for( k = 0; k < 5; k++ )
			{
				outV[0]	+= inImg->data[(offset + k) << 2]		* h[k];
				outV[1] += inImg->data[((offset + k) << 2) + 1]	* h[k];
				outV[2] += inImg->data[((offset + k) << 2) + 2]	* h[k];
			}
			//normalize
			outV[0] = outV[0] >> 4;
			outV[1] = outV[1] >> 4;
			outV[2] = outV[2] >> 4;

			outImg->data[offset]	 = (BYTE)outV[0];
			outImg->data[offset + 1] = (BYTE)outV[1];
			outImg->data[offset + 2] = (BYTE)outV[2];
		}
	}

	// apply vertical
	for( j = 0; j < outImg->cols; j++ )
	{
		for( i = 0; i < outImg->rows - 5; i++ )
		{
			offset = ( i * outImg->cols + j );
			memset(&outV, 0, 12);
			for( k = 0; k < 5; k++ )
			{
				outV[0]	+= inImg->data[(offset + k) << 2]		* h[k];
				outV[1] += inImg->data[((offset + k) << 2) + 1]	* h[k];
				outV[2] += inImg->data[((offset + k) << 2) + 2]	* h[k];
			}
			//normalize
			outV[0] = outV[0] >> 4;
			outV[1] = outV[1] >> 4;
			outV[2] = outV[2] >> 4;

			inImg->data[offset]		= (BYTE)outV[0];
			inImg->data[offset + 1] = (BYTE)outV[1];
			inImg->data[offset + 2] = (BYTE)outV[2];
		}
	}
}