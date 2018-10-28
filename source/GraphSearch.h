#pragma once
//����Local cost�ļ��㷽ʽ�Լ�·���ļ����㷨

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
#define CLK_TCK  CLOCKS_PER_SEC //����ʱ��ÿ��Ĵ����  
using namespace std;
using namespace cv;

void init();
//local_const
double local_cost(GraphNode *p, GraphNode *q);
//2D-DP Graph Search
void calPath(CvPoint start_point);
//
void getPath(CvPoint start_point, CvPoint end_point);
//����segmentation��ʱ���Զ�Ѱַ�����gradient Magnitude���ֵ��
CvPoint cursor_snap(int x, int y);
