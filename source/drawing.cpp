#include "drawing.h"

Mat image;
Mat gray_image;
char *imageName;

list<list<CvPoint>> contours;//��������������
list<CvPoint>seg_points;//���������е�segmentation point
extern list<CvPoint>current_path;//2D-DP Search�ҵ���·��

//������ǰ�����������е�segmentation point��interactive_line
void draw() {
	//����circle����Զ�ظı���ͼƬ������������ö�����ԭ����ͣ�ش�����ͼƬ��ˢ��
	Mat temp;
	image.copyTo(temp);

	for (auto c : contours)
		for (auto i : c)
			circle(temp, i, 1, Scalar(0, 255, 0), -1, 8, 0);

	for (auto n : current_path)
		circle(temp, n, 1, Scalar(0, 255, 0), -1, 8, 0);

	for (auto s : seg_points)
		circle(temp, s, 4, Scalar(255, 0, 0), -1, 8, 0);
	//������ʾͼƬ
	imshow(imageName, temp);
}


//clock_t Begin, End;
//double duration;

void setSegPoint(int x, int y) {
	CvPoint automatic_seg_point = cursor_snap(x, y);
	if(!seg_points.empty())
	  getPath(seg_points.back(), automatic_seg_point);
	seg_points.push_back(automatic_seg_point);
	contours.push_back(current_path);

	double t = (double)getTickCount();
	// do something ...���ظô�����ִ�����ĵ�ʱ�䣬��λΪ��  
	calPath(seg_points.back());
	t = ((double)getTickCount() - t) / getTickFrequency();
	cout << "duration:" << t << " s"<<endl << endl; 

	/*Begin = clock();
	End = clock();
	duration = (End - Begin) / CLK_TCK;
	cout << CLK_TCK << endl;
	cout << "duration:" << duration << endl<<endl;*/
}

void on_mouse(int event, int x, int y, int flags, void * ustc)
{
	if (event == CV_EVENT_MOUSEMOVE) {
		//��ǰ��Ȼ��seed point
		if (!seg_points.empty()) {
			getPath(seg_points.back(), CvPoint(x, y));
		}
	}
	else if (event == CV_EVENT_LBUTTONDOWN) { //��seed point
		setSegPoint(x, y);
	}
	else if (event == CV_EVENT_RBUTTONDOWN) {
		if (!seg_points.empty()) {
			seg_points.pop_back();
			contours.pop_back();
			current_path.clear();
			if (!seg_points.empty()) {
				calPath(seg_points.back());
			}
		}
	}
	//ÿ������¼��������»���
	draw();
}


void loadImage(int argc, char** argv) {
	//�����л�ȡ��ͼ��·��
	//if (argc != 2)
	//{
	//	printf("useage: %s <imagefile>\n ", argv[0]);
	//	exit(1);
	//}
	if (argc != 2)
	{
		cout << "load default image:lena.jpg" << endl;
		//printf("useage: %s <imagefile>\n ", argv[0]);
		const char* default_image = "./lena.jpg";
		imageName = new char[20];
		strcpy_s(imageName,21 ,default_image);
	}else  imageName = argv[1];
	
	image = imread(imageName, CV_LOAD_IMAGE_COLOR);
	//assert(image.channels == 3);

	if (!image.data)
	{
		printf(" No image data \n ");
		exit(1);
	}
	cvtColor(image, gray_image, CV_BGR2GRAY);
	init();
}