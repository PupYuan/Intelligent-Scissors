#pragma once
//定义Local cost的计算方式以及路径的计算算法

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "GraphNode.h"
#include "imageProcess.h"
#include <list>
#include <iostream>
#include <assert.h>
#include "ActiveList.h"
#include "drawing.h"
#include "util.h"
#include<ctime>  
#define CLK_TCK  CLOCKS_PER_SEC //机器时钟每秒的打点数  
using namespace std;
using namespace cv;

void init();
//local_const
double local_cost(GraphNode *p, GraphNode *q);
//2D-DP Graph Search
void calPath(CvPoint start_point);
//
void getPath(CvPoint start_point, CvPoint end_point);
//放置segmentation的时候自动寻址最近的gradient Magnitude最大值点
CvPoint cursor_snap(int x, int y);
