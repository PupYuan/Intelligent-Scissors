#include "util.h"

void normalize(cv::Vec2d &dst) {
	double sum = sqrt(pow(dst[0], 2) + pow(dst[1], 2));
	dst[0] = dst[0] / sum;
	dst[1] = dst[1] / sum;
}