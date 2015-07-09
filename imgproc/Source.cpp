#include "Image.h"
using namespace std;

int main()
{
	Mat in, Gx, Gy, kernel, out, inblur;

	FIBITMAP* hImg = FreeImage_Load(FIF_PNG, "images/src2.png");
	GetMatFromImage(&in, hImg);

	inblur.cols = in.cols;
	inblur.rows = in.rows;
	inblur.depth = in.depth;
	inblur.data = (unsigned char*)malloc( in.rows * in.cols * in.depth );

	FIBITMAP* hOut = FreeImage_Allocate( in.cols, in.rows, FreeImage_GetBPP(hImg));
	GetMatFromImage(&out, hOut);

	out.cols = in.cols;
	out.rows = in.rows;
	out.depth = in.depth;
	memset(out.data, 0, out.cols * out.rows * out.depth );

	Gx.depth = in.depth;
	Gx.cols = in.cols;
	Gx.rows = in.rows;
	Gx.data = new unsigned char[in.cols * in.rows * in.depth ];

	Gy.depth = in.depth;
	Gy.cols = in.cols;
	Gy.rows = in.rows;
	Gy.data = new unsigned char[in.cols * in.rows * in.depth ];


	/*int blur[] = { 1, 1, 1,
				   1, 1, 1,
				   1, 1, 1 };*/
	int blur[] = {
		1,4, 6, 4, 1,
		4, 16, 24, 16, 4,
		6, 24, 36, 24, 6,
		4, 16, 24, 16, 4,
		1,  4,  6,  4, 1
	};
	

	int kx[] =  { -1, 0, 1,
				  -2, 0, 2,
				  -1, 0, 1};

	int ky[] = { -1, -2, -1,
				  0 , 0, 0,
				  1, 2, 1};

	/* to do - sobel edge detector  */

	kernel.data = (BYTE*)blur;
	kernel.rows = 5;
	kernel.cols = 5;
	ApplyKernel(&in, &inblur, &kernel, 256);
	//memcpy(inblur.data, in.data, in.cols * in.rows * in.depth);

	kernel.data = (BYTE*)kx;
	kernel.rows = 3;
	kernel.cols = 3;
	ApplyKernel(&inblur, &Gx, &kernel, 4);

	kernel.data = (BYTE*)ky;
	kernel.rows = 3;
	kernel.cols = 3;
	ApplyKernel(&inblur, &Gy, &kernel, 4);

	int upperTresh=255;
	int lowerTresh=1;

	//// compute output gradient based on Gx and Gy
	for( unsigned int i = 0; i < out.rows; i++ )
	{
		for( unsigned int j = 0; j < out.cols; j++ )
		{
			int outOffset = (i * out.cols + j) * out.depth;

			for( int component = 0; component < out.depth; component++ )
			{
				out.data[outOffset + component] = sqrt( Gx.data[outOffset + component] * Gx.data[outOffset + component] + 
														Gy.data[outOffset + component] * Gy.data[outOffset + component] );
			}
		}
	}


	FreeImage_Save(FIF_PNG, hOut, "images/test.png");

	delete [] Gx.data;
	delete [] Gy.data;

	return 0;
}