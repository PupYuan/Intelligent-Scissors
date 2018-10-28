#pragma once

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <cmath>
using namespace std;
using namespace cv;

//为了扩展后面不同的kernel，引入卷积操作
//void convolution() {
//
//}
//用于gradientDirection


void sobleFilter(Mat &source, Mat &dst);
void laplacianZeroCrossing(Mat &source, Mat &dst);