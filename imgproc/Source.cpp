#include "Image.h"
using namespace std;

int main()
{
	Mat in, out, mask;

	FIBITMAP* hImg = FreeImage_Load(FIF_PNG, "images/src.png");
	GetMatFromImage(&in, hImg);

	FIBITMAP* hOut = FreeImage_Allocate( in.cols, in.rows, FreeImage_GetBPP(hImg));
	GetMatFromImage(&out, hOut);

	int m[] =  { 0, -1, 0,
				 -1, 4, -1,
				 0, -1, 0 };

	/*int m[] = { 0, 1, 1, 2, 2, 2, 1, 1, 0,
				1, 2, 4, 5, 5, 5, 4, 2, 1,
				1, 4, 5, 3, 0, 3, 5, 4, 1,
				2, 5, 3, -12, -24, -12, 3, 5, 2,
				2, 5, 0, -24, -40, -24, 0, 5, 2,
				2, 5, 3, -12, -24, -12, 3, 5, 2,
				1, 4, 5, 3, 0, 3, 5, 4, 1,
				1, 2, 4, 5, 5, 5, 4, 2, 1,
				0, 1, 1, 2, 2, 2, 1, 1, 0};*/

	mask.data = (BYTE*)m;
	mask.rows = 3;
	mask.cols = 3;

	Conv(&in, &out, &mask, 1);

	FreeImage_Save(FIF_PNG, hOut, "images/test.png");

	return 0;
}