#pragma once

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <cmath>
using namespace std;
using namespace cv;

//Ϊ����չ���治ͬ��kernel������������
//void convolution() {
//
//}
//����gradientDirection


void sobleFilter(Mat &source, Mat &dst);
void laplacianZeroCrossing(Mat &source, Mat &dst);