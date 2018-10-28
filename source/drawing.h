#pragma once

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

#include "GraphSearch.h"
#include <list>
#include <iostream>
using namespace std;

using namespace cv;


void loadImage(int argc, char** argv);
void on_mouse(int event, int x, int y, int flags, void* ustc);
void setSegPoint(int x, int y);
