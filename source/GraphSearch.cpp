#include "GraphSearch.h"

extern Mat gray_image;
extern Mat image;
extern char* imageName;
extern double maxG;
extern Mat GX_gradient;
extern Mat GY_gradient;
Mat gradient_magnitude_image;
Mat zero_crossing_image;
GraphNode **graph;
list<CvPoint>current_path;//当前正在画的线，可能会被存储为contours中的一条线

bool CostWeightChanged = false;
//权重
int Wz = 43;
int Wd = 43;
int Wg = 14;

static void on_track(int, void*)
{
	CostWeightChanged = true;
}

//void callbackButton1(int state, void *userdata) {
//
//}

void init() {

	//确保相同的图片规格和通道数
	gray_image.copyTo(gradient_magnitude_image);
	gray_image.copyTo(zero_crossing_image);

	sobleFilter(image, gradient_magnitude_image);
	laplacianZeroCrossing(image, zero_crossing_image);

	namedWindow(imageName, CV_WINDOW_AUTOSIZE);
	imshow(imageName, image);

	char WzName[50];
	sprintf_s(WzName, "Wz %d", 100);
	createTrackbar(WzName, imageName, &Wz, 100, on_track);
	on_track(Wz, 0);

	char WgName[50];
	sprintf_s(WgName, "Wg %d", 100);
	createTrackbar(WgName, imageName, &Wg, 100, on_track);
	on_track(Wg, 0);

	char WdName[50];
	sprintf_s(WdName, "Wd %d", 100);
	createTrackbar(WdName, imageName, &Wd, 100, on_track);
	on_track(Wd, 0);

	//createButton("button5", callbackButton1, NULL, QT_RADIOBOX, 0);
	//初始化
	graph = new GraphNode*[gray_image.rows];
	for (int i = 0; i < gray_image.rows; i++) {
		graph[i] = new GraphNode[gray_image.cols];
	}
	//初始化graph的坐标
	for (int i = 0; i < gray_image.rows; i++)
		for (int j = 0; j < gray_image.cols; j++) {
			graph[i][j].y = i;
			graph[i][j].x = j;
		}
	//初始化鼠标回调函数,on_mouse在drawing.h中
	cvSetMouseCallback(imageName, on_mouse, 0);
}


CvPoint cursor_snap(int x, int y) {
	CvPoint seg_point(0, 0);
	uchar gradient_magnitude = 0;
	for(int i=-7;i<=7;i++)
		for (int j = -7; j < 7; j++) {
			if (x + i < 0 || x + i >= gradient_magnitude_image.cols ||
				y + j < 0 || y + j >= gradient_magnitude_image.rows)
				continue; //防止越界
			if (gradient_magnitude_image.at<uchar>(Point(x + i, y + j)) > gradient_magnitude) {
				gradient_magnitude = gradient_magnitude_image.at<uchar>(Point(x + i, y + j));
				seg_point.x = x + i;
				seg_point.y = y + j;
			}
		}
	return seg_point;
}

//为了加速，可以全部用GraphNode*指针，但在drawing模块中还是需要转化成CvPoint
void getPath(CvPoint start_point,CvPoint end_point) {
	//list<GraphNode*> path;//要返回的path
	current_path.clear();
	if (CostWeightChanged)
		return;
	GraphNode* free_point = &graph[end_point.y][end_point.x];
	GraphNode* s = &graph[start_point.y][start_point.x];
	while ((free_point != s)&&free_point->back_pointer != nullptr){
		//free_point->drawed_time++;
		//if (free_point->drawed_time > 200) {
		//	//setSegPoint(free_point->x, free_point->y);
		//	//return;
		//}
		current_path.push_back(CvPoint(free_point->x,free_point->y));
		//assert(free_point->back_pointer != NULL);
		free_point = free_point->back_pointer;
	} 
}

void calPath(CvPoint start_point){
	double gtemp;
	GraphNode * p;
	ActiveList L;
	GraphNode * s = &graph[start_point.y][start_point.x];
	//初始化
	for (int i = 0; i < gray_image.rows; i++)
		for (int j = 0; j < gray_image.cols; j++) {
			graph[i][j].e = false;
			graph[i][j].global_cost = 0;
			graph[i][j].inL = false;
			graph[i][j].next = nullptr;
			graph[i][j].back_pointer = nullptr;
			graph[i][j].drawed_time = 0;
		}
	L.add(s);
	while (!L.empty()) {
		p = L.head;//升序排序
		L.remove(p);
		graph[p->y][p->x].e = true;
		//更快地访问q的邻居pixel
		for (int i = -1; i <= 1; i++)
			for (int j = -1; j <= 1; j++) {
				if (p->x + j < 0 || p->x + j > gray_image.cols - 1 ||
					p->y + i < 0 || p->y + i > gray_image.rows - 1)
					continue;//跳过这些neighbor pixel防止越界
				if (graph[p->y+i][p->x+j].e)//跳过已经处理的
					continue;
				GraphNode * r = &graph[p->y + i][p->x + j];
				//local_cost(p,r)我们暂时只有r 
				//(abs(i)+abs(j) == 2?sqrt(2):1) 让对角线元素乘上根号2
				gtemp = graph[p->y][p->x].global_cost + (abs(i)+abs(j) == 2?sqrt(2):1)*local_cost(p,r);
				if (gtemp < graph[p->y + i][p->x + j].global_cost) {
					L.remove(r);
					r->inL = false;
					r->next = nullptr;
				}
				if (r->inL == false) {//if neighbor not on list
					graph[p->y + i][p->x + j].global_cost = gtemp;
					r->back_pointer = p;
					L.add(r);
					r->inL = true;
					//若已经到达指定路径，则提前终止
					//if (p == target)
					//	L.clear();//L中的元素被去除
				}
			}
	}
	CostWeightChanged = false;
	//path_completed = true;
}

//Magnitude 
double fG(GraphNode *p) {
	return 1-gradient_magnitude_image.at<uchar>(p->y, p->x)/ maxG;
}
//2nd Magnitude
double fZ(GraphNode *p) {
	double result = zero_crossing_image.at<uchar>(p->y, p->x) / 255;
	return result;
}
//gradientDirection
double fD(GraphNode *p,GraphNode *q) {
	uchar IX, IY;
	//p
	IX = GX_gradient.at<uchar>(p->y, p->x);
	IY = GY_gradient.at<uchar>(p->y, p->x);
	Vec2d Dp(IX, IY); Vec2d Dip(IY, (-1)*IX);
	normalize(Dp);
	normalize(Dip);
	//q
	IX = GX_gradient.at<uchar>(q->y, q->x);
	IY = GY_gradient.at<uchar>(q->y, q->x);
	Vec2d Dq(IX, IY); Vec2d Diq(IY, (-1)*IX);
	normalize(Dq);
	normalize(Diq);

	cv::Vec2d q_p(q->x - p->x, q->y - p->y);
	cv::Vec2d p_q(p->x - q->x, p->y - q->y);
	//
	normalize(q_p);
	normalize(p_q);

	double dp = Dip.dot(q_p) >= 0 ? Dip.dot(q_p) : Dip.dot(p_q);
	double dq = Dip.dot(q_p) >= 0 ? q_p.dot(Diq) : p_q.dot(Diq);

	double result = (2 / (3 * M_PI))*(acos(dp) + acos(dq));
	return result;
}
//local_const
double local_cost(GraphNode *p, GraphNode *q) {
	return Wz * fZ(q)/100.0 + Wd * fD(p, q) / 100.0 + Wg * fG(q)/100.0;
}
