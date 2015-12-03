
#ifndef IMG_PROC_H
#define IMG_PROC_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

#define SMALL_RECT_WIDTH 250
#define SMALL_RECT_HEIGHT 250

#define IMG_WIDTH 4160
#define HORZ_VIEW_ANGLE 53.5
#define PIX_ANG_CONSTANT (HORZ_VIEW_ANGLE / IMG_WIDTH)

struct box {
	Rect bbox;
	float angle;
};

vector <box> procImg(Mat src, int thresh, int blurMag)
{
	Mat threshold_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	cvtColor(src, src, CV_BGR2GRAY);
	blur(src, src, Size(blurMag,blurMag));
	threshold(src, threshold_output, thresh, 255, THRESH_BINARY);
	findContours(threshold_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	vector<vector<Point> > contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());
	vector<Point2f> center(contours.size());
	vector<float> radius(contours.size());


	for(unsigned int i = 0; i < contours.size(); i++)
	{
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));
	}

#ifdef SHOW_PROC_FEED
	string name = "Processed";
	namedWindow(name, CV_WINDOW_NORMAL);
	imshow(name, src);
#endif

	vector <box> boxes;

	for(unsigned int i = 0; i < contours.size(); i++)
	{
		if(boundRect[i].height <= SMALL_RECT_HEIGHT || boundRect[i].width <= SMALL_RECT_WIDTH)
			continue;	//ignore small rectangles

		int x = (boundRect[i].x + boundRect[i].width) / 2;
		/*
		 * Assume the change in x is proportional to change in angle:
		 * 		dtheta = k * dx
		 * So,
		 * 		k = dtheta / dx
		 * Assuming theta_initial = 0 and x_initial = 0
		 * 		k = theta_max / x_max
		 * 		k = horzViewAngle / imageWidth
		 *
		 * 	and
		 *
		 * 		theta = k * x
		 * */
		float theta = PIX_ANG_CONSTANT * x;
		box tmpBox = box();
		tmpBox.bbox = boundRect[i];
		tmpBox.angle = theta;
		boxes.push_back(tmpBox);
	}
	return boxes;
}

#endif	//IMG_PROC_H
