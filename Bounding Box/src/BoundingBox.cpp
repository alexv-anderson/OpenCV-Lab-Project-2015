//Based on tutorial at: http://docs.opencv.org/2.4/doc/tutorials/imgproc/shapedescriptors/bounding_rects_circles/bounding_rects_circles.html

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

Mat src; Mat src_gray;
int thresh = 127;
int max_thresh = 255;
RNG rng(12345);

void thresh_callback(int, void*);

int main(int argc, char** argv)
{
	cout << "Reading image...";
	src = imread(argv[1], 1);
	cout << "\tSuccessful\n";

	cout << "Converting to grayscale...\n";
	cvtColor(src, src_gray, CV_BGR2GRAY);
	cout << "Blur...\n";
	blur(src_gray, src_gray, Size(100,100));

	cout << "Creating Source window...\n";
	string source_window = "Source";
	namedWindow(source_window, CV_WINDOW_NORMAL);
	imshow(source_window, src);

	cout << "Creating Trackbar...\n";
	createTrackbar(" Threshold:", "Source", &thresh, max_thresh, thresh_callback);
	cout << "Initial call to thresh_callback()...\n";
	thresh_callback(0, 0);

	cout << "Waiting for input\n";
	waitKey(0);
	return 0;
}

void thresh_callback(int, void*)
{
	Mat threshold_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	cout << "\tthreshold...\n";
	threshold(src_gray, threshold_output, thresh, 255, THRESH_BINARY);
	namedWindow("Threshold", CV_WINDOW_NORMAL);
	imshow("Threshold", threshold_output);
	cout << "\tfindContours...\n";
	findContours(threshold_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	vector<vector<Point> > contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());
	vector<Point2f> center(contours.size());
	vector<float> radius(contours.size());

	cout << "\t" << contours.size() << " contours found\n";

	for(int i = 0; i < contours.size(); i++)
	{
		cout << "\t\t approxPolyDP for contour " << i << endl;
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		cout << "\t\t boundingRect for contour " << i << endl;
		boundRect[i] = boundingRect(Mat(contours_poly[i]));
	}

	cout << "\tCreating " << contours.size() << " drawings\n";

	Mat drawing = Mat::zeros(threshold_output.size(), CV_8UC3);
	for(int i = 0; i < contours.size(); i++)
	{
		if(boundRect[i].height <= 250 || boundRect[i].width <= 250)
			continue;	//ignore small rectangles
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		cout << "\t\tdrawContours for contour " << i << endl;
		drawContours(drawing, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point());
		cout << "\t\trectangle for contour " << i << endl;
		rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), 2, 8, 0);

		int x = (boundRect[i].x + boundRect[i].width) / 2;
		int y = (boundRect[i].y + boundRect[i].height) / 2;

		int imageWidth = 4160;	//width of the image in pixels
		float horzViewAngle = 53.5;	//horizontal view angle of the camera
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
		float conversion = horzViewAngle / imageWidth;
		float theta = conversion * x;

		cout << "\t\tAt (" << x << ", " << y << ") Cartesian or (?, " << theta << ") Polar\n";	//this can be turned into an angle as soon as we know the width of the field of view
	}

	cout << "\tDrawing Contours Window\n";
	namedWindow("Contours", CV_WINDOW_NORMAL);
	imshow("Contours", drawing);
}
