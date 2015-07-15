#include "edge_detector.h"
#include <math.h>

#define PI 3.14159265

enum DIRECTION { HORIZONTAL, LEFT_DIAGONAL, VERTICAL, RIGHT_DIAGONAL, VISITED };

void DetectEdges( ImageMat* inImg, ImageMat* outImg )
{
	outImg->rows = inImg->rows;
	outImg->cols = inImg->cols;
	
	int size = (outImg->rows * outImg->cols);
	outImg->data = (BYTE*)malloc( size << 2 );
	memset( outImg->data, 0, size << 2 );
	
	//memcpy( outImg->data, inImg->data, size << 2 );

	//smooth image
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

	/* /////////////////////
	int kx[] = { -1, 0, 1,
				 -2, 0, 2,
				 -1, 0, 1 };

	int ky[] = { -1, -2, -1,
				  0,  0,  0,
				  1,  2,  1 };
	/////////////////////////// */

	char* PixelDir = (char*)malloc( size );
	memset(PixelDir, VISITED, size);

	for( i = 1; i < inImg->rows-1; i++ )
	{
		for( j = 1; j < inImg->cols-1; j++ )
		{
			memset(&Gx, 0, 12);
			memset(&Gy, 0, 12);

			for( k = 0; k < 3; k++ )
			{
				inOffset = ((i-1) * inImg->cols + (j-1)) << 2;
				Gx[k] += inImg->data[inOffset + k] * (-1);

				inOffset = ((i) * inImg->cols + (j-1)) << 2;
				Gx[k] += inImg->data[inOffset + k] * (-2);

				inOffset = ((i+1) * inImg->cols + (j-1)) << 2;
				Gx[k] += inImg->data[inOffset + k] * (-1);

				inOffset = ((i-1) * inImg->cols + (j+1)) << 2;
				Gx[k] += inImg->data[inOffset + k] * (1);

				inOffset = ((i) * inImg->cols + (j+1)) << 2;
				Gx[k] += inImg->data[inOffset + k] * (2);

				inOffset = ((i+1) * inImg->cols + (j+1)) << 2;
				Gx[k] += inImg->data[inOffset + k] * (1);

				Gx[k] = Gx[k] >> 2;
				//if( Gx[k] < 0 ) Gx[k] *= -1;
			}

			for( k = 0; k < 3; k++ )
			{
				inOffset = ((i-1) * inImg->cols + (j-1)) << 2;
				Gy[k] += inImg->data[inOffset + k] * (-1);

				inOffset = ((i-1) * inImg->cols + (j)) << 2;
				Gy[k] += inImg->data[inOffset + k] * (-2);

				inOffset = ((i-1) * inImg->cols + (j+1)) << 2;
				Gy[k] += inImg->data[inOffset + k] * (-1);


				inOffset = ((i+1) * inImg->cols + (j-1)) << 2;
				Gy[k] += inImg->data[inOffset + k] * (1);

				inOffset = ((i+1) * inImg->cols + (j)) << 2;
				Gy[k] += inImg->data[inOffset + k] * (2);

				inOffset = ((i+1) * inImg->cols + (j+1)) << 2;
				Gy[k] += inImg->data[inOffset + k] * (1);

				Gy[k] = Gy[k] >> 2;
				//if( Gy[k] < 0 ) Gy[k] *= -1;
			}

			outOffset = (i * inImg->cols + j) << 2;
			outImg->data[outOffset]		= sqrt(Gx[0] * Gx[0] + Gy[0] * Gy[0]);
			outImg->data[outOffset + 1] = sqrt(Gx[1] * Gx[1] + Gy[1] * Gy[1]);
			outImg->data[outOffset + 2] = sqrt(Gx[2] * Gx[2] + Gy[2] * Gy[2]);
			outImg->data[outOffset + 3] = 255;

			dirOffset = (i * inImg->cols + j);
			double theta =  atan2((Gy[0] + Gy[1] + Gy[2]), (Gx[0] + Gx[1] + Gx[2]));

			theta = theta * 180.0f / PI;
			
			char direction[100];
			int dir = VISITED;

			if( theta < 0.0f )
			{
				theta  = 180.0f + theta;
			}

			if( (theta >= 0 && theta <= 22.5f) || (theta > 157.5f && theta <= 180.0f) )
			{
				dir = HORIZONTAL;
			}
			else if( theta > 22.5f && theta <= 67.5f  )
			{
				dir = RIGHT_DIAGONAL;
			}
			else if( theta >= 112.5f && theta <= 157.5f )
			{
				dir = LEFT_DIAGONAL;
			}
			else
			{
				dir = VERTICAL;
			}

			PixelDir[dirOffset] = dir;
		}
	}

	// non - maximum supression
	int maxStrenght, maxL, maxC;
	int sum;
	int col, line;

	for( i = 0; i < outImg->rows; i++ )
	{
		for( j = 0; j < outImg->cols; j++ )
		{
			dirOffset = i * outImg->cols + j;

			switch( PixelDir[dirOffset] )
			{
			case VISITED:
				{
					break;
				}
			case HORIZONTAL:
				{
					outOffset = (dirOffset) << 2;
					maxStrenght = outImg->data[outOffset] + outImg->data[outOffset + 1] + outImg->data[outOffset + 2];
					maxC = 0;

					for( col = 1; j + col < outImg->cols; col++ )
					{
						if( PixelDir[dirOffset + col] != HORIZONTAL )
							break;
						else
							PixelDir[dirOffset + col] = VISITED;
						outOffset = (dirOffset + col) << 2;
						sum = outImg->data[outOffset] + outImg->data[outOffset + 1] + outImg->data[outOffset + 2];
						if( sum > maxStrenght )
						{
							maxStrenght = sum;
							maxC = col;
						}
					}

					for( int k = 0; k < col; k++ )
					{
						if( k != maxC )
						{
							outOffset = (dirOffset + k) << 2;
							memset(&outImg->data[outOffset], 0, 3);
						}
					}
					//outOffset = (dirOffset + maxC) << 2;
					//memset(&outImg->data[outOffset], 255, 4);
					break;
				}
			case VERTICAL:
				{
					outOffset = (dirOffset) << 2;
					maxStrenght = outImg->data[outOffset] + outImg->data[outOffset + 1] + outImg->data[outOffset + 2];
					maxL = 0;

					for( line = 1; i + line < outImg->rows; line++ )
					{
						if( PixelDir[dirOffset + line * outImg->cols ] != VERTICAL )
							break;
						else
							PixelDir[dirOffset + line * outImg->cols ] = VISITED;
						outOffset = (dirOffset + line * outImg->cols) << 2;
						sum = outImg->data[outOffset] + outImg->data[outOffset + 1] + outImg->data[outOffset + 2];
						if( sum > maxStrenght )
						{
							maxStrenght = sum;
							maxL = line;
						}
					}

					for( int k = 0; k < line; k++ )
					{
						if( k != maxL )
						{
							outOffset = (dirOffset + k * outImg->cols) << 2;
							memset(&outImg->data[outOffset], 0, 3);
						}
					}

					//outOffset = (dirOffset + maxL * outImg->cols) << 2;
					//memset(&outImg->data[outOffset], 255, 4);
					break;
				}
			case LEFT_DIAGONAL:
				{
					outOffset = (dirOffset) << 2;
					maxStrenght = outImg->data[outOffset] + outImg->data[outOffset + 1] + outImg->data[outOffset + 2];
					maxL = 0;
					maxC = 0;

					for( line = 1, col = 1; i + line < outImg->rows && j + col < outImg->cols; line++, col++ )
					{
						if( PixelDir[dirOffset + line * outImg->cols + col] != LEFT_DIAGONAL )
							break;
						else
							PixelDir[dirOffset + line * outImg->cols + col] = VISITED;
						outOffset = (dirOffset + line * outImg->cols + col) << 2;
						sum = outImg->data[outOffset] + outImg->data[outOffset + 1] + outImg->data[outOffset + 2];
						if( sum > maxStrenght )
						{
							maxStrenght = sum;
							maxL = line;
							maxC = col;
						}
					}

					for( int k = 0; k < line; k++ )
					{
						if( k != maxL )
						{
							outOffset = (dirOffset + k * outImg->cols + k) << 2;
							memset(&outImg->data[outOffset], 0, 3);
						}
					}

					//outOffset = (dirOffset + maxL * outImg->cols + maxC) << 2;
					//memset(&outImg->data[outOffset], 255, 4);

					break;
				}
			case RIGHT_DIAGONAL:
				{
					outOffset = (dirOffset) << 2;
					maxStrenght = outImg->data[outOffset] + outImg->data[outOffset + 1] + outImg->data[outOffset + 2];
					maxL = 0;
					maxC = 0;

					for( line = -1, col = -1; i + line >= 0 && j + col >= 0; line--, col-- )
					{
						if( PixelDir[dirOffset + line * outImg->cols + col] != RIGHT_DIAGONAL )
							break;
						else
							PixelDir[dirOffset + line * outImg->cols + col] = VISITED;
						outOffset = (dirOffset + line * outImg->cols + col) << 2;
						sum = outImg->data[outOffset] + outImg->data[outOffset + 1] + outImg->data[outOffset + 2];
						if( sum > maxStrenght )
						{
							maxStrenght = sum;
							maxL = line;
							maxC = col;
						}
					}

					for( int k = 0; k > line; k-- )
					{
						if( k != maxL )
						{
							outOffset = (dirOffset + k * outImg->cols + k) << 2;
							memset(&outImg->data[outOffset], 0, 3);
						}
					}

					//outOffset = (dirOffset + maxL * outImg->cols + maxC) << 2;
					//memset(&outImg->data[outOffset], 255, 4);

					break;
				}
			default:
				{
					break;
				}
			}
		}
	}

	free(PixelDir);
}