#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "drawing.h"
#include "GraphSearch.h"

using namespace cv;

int main(int argc, char** argv)
{
	loadImage( argc,  argv);

	waitKey(0);
	return 0;
}