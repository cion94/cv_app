#include "edge_detector.h"
using namespace std;

int main()
{
	ImageMat in, out;

	LoadImageMat("images/src4.png", &in);

	DetectEdges(&in, &out);

	SaveImageMat("images/out.png", &out);

	FreeImageMat(&in);
	FreeImageMat(&out);

	return 0;
}