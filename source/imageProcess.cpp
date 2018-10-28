#include "imageProcess.h"

//�����Vec3�п����и���
int maxVec3(Vec3i t) {
	int tempmax = 0;
	for (int i = 0; i < 2; i++)
		tempmax = max(tempmax, abs(t[i]));

	return tempmax;
}
bool hasZero(Vec3i t) {
	if (t[0] != 0 && t[1] != 0 && t[2] != 0)
		return false;
	return true;
}

Mat GX_gradient;
Mat GY_gradient;
double maxG;
void sobleFilter(Mat &source, Mat &dst) {
	dst.copyTo(GX_gradient);
	dst.copyTo(GY_gradient);
	//��int��Ϊ���������Ǹ���
	maxG = 0;
	Vec3i GX_3channels, GY_3channels;
	double GX, GY, GXY;
	for (int y = 1; y < source.rows - 1; y++)
		for (int x = 1; x < source.cols - 1; x++) {
			GX_3channels = (Vec3i)source.at<Vec3b>(y + 1, x + 1) - (Vec3i)source.at<Vec3b>(y + 1, x - 1)
				+ 2 * (Vec3i)source.at<Vec3b>(y, x + 1) - (Vec3i)source.at<Vec3b>(y, x - 1)
				+ (Vec3i)source.at<Vec3b>(y - 1, x + 1) - (Vec3i)source.at<Vec3b>(y - 1, x - 1);

			GY_3channels = (Vec3i)source.at<Vec3b>(y + 1, x + 1) - (Vec3i)source.at<Vec3b>(y - 1, x + 1)
				+ 2 * (Vec3i)source.at<Vec3b>(y + 1, x) - (Vec3i)source.at<Vec3b>(y - 1, x)
				+ (Vec3i)source.at<Vec3b>(y + 1, x - 1) - (Vec3i)source.at<Vec3b>(y - 1, x - 1);
			GX = maxVec3(GX_3channels);
			GY = maxVec3(GY_3channels);
			GXY = sqrt(pow(GX, 2) + pow(GY, 2));
			maxG = max(maxG, GXY);

			dst.at<uchar>(y, x) = GXY;
			GX_gradient.at<uchar>(y, x) = GX;
			GY_gradient.at<uchar>(y, x) = GY;
		}
	//for debug
	namedWindow("gradient_magnitude_image", CV_WINDOW_AUTOSIZE);
	imshow("gradient_magnitude_image", dst);
}

void LaplacianMask(Mat &source, Mat_<Vec3i> &dst) {
	if (dst.empty())
		dst.create(source.size());
	//��float��Ϊ���׵��������Ǹ�������Ϊ�˷�ֹ����0�ĳ���
	Vec3i GXY_3channels;
	for (int y = 1; y < source.rows - 1; y++)
		for (int x = 1; x < source.cols - 1; x++) {
			GXY_3channels = (Vec3i)source.at<Vec3b>(y, x + 1) + (Vec3i)source.at<Vec3b>(y, x - 1)
				+ (Vec3i)source.at<Vec3b>(y + 1, x ) + (Vec3i)source.at<Vec3b>(y-1, x)
				 - 4*(Vec3i)source.at<Vec3b>(y, x);
			dst.at<Vec3i>(y, x) = GXY_3channels;
		}
}


void laplacianZeroCrossing(Mat &source, Mat &dst) {
	Mat_<Vec3i> laplacian_image;
	//Laplacian(source, laplacian_image, CV_8UC3 ,3 , 1, 0);
	LaplacianMask(source, laplacian_image);

	Vec3i ILq;
	//�õ�zero-crossingͼ��
	for (int y = 1; y<source.rows - 1; y++)
		for (int x = 1; x < source.cols - 1; x++) {
			ILq = laplacian_image.at<Vec3i>(Point(x, y));

			//����ȫ����Ϊ1����Ϊֻ�б�Ե����0���ʹ���·��
			//dst.at<uchar>(Point(x, y)) = 255;
			dst.at<uchar>(Point(x, y)) = 0;

			if (hasZero(ILq)) //���ٵ�պõ���0
				//dst.at<uchar>(Point(x, y)) = 0;
				dst.at<uchar>(Point(x, y)) = 255;
			else {//�ж�Laplacianͼ���У��õ��Ƿ�����������ӽ�0�ģ������Ƿ�����෴���ŵ�����
				for (int i = -1; i <= 1; i++)
					for (int j = -1; j <= 1; j++) {
						Vec3i neighbor_pixel = laplacian_image.at<Vec3i>(Point(x + i, y + i));
						//Vec3b zero_crossing_3channels;
						for (int c = 0; c < 3; c++) {
							bool positive = ILq[c] > 0 ? true : false;//��������ILq��������
							if (neighbor_pixel[c] > 0 ? true : false != positive) {//�������
								if (abs(ILq[c]) < abs(neighbor_pixel[c]))
									//dst.at<uchar>(Point(x, y)) = 0;
									dst.at<uchar>(Point(x, y)) = 255;
								else
									//dst.at<uchar>(Point(x+i, y+i)) = 0;
									dst.at<uchar>(Point(x + i, y + i)) = 255;
								break;
							}
						}
					}
			}
		}

	imshow("ZeroCrossing", dst);
}