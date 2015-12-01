//#include "header.h"
//#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

//#include "opencv2/opencv.hpp"

#define USE_HIGUI

#include "header.h"

using namespace cv;
using namespace std;

// Function to get images

int main (int argc, int argv[]) {
	// Open a video stream
	VideoCapture vidStream(0);

	if(!vidStream.isOpened())
	{
		cout << "Could not open camera\n";
		return -1;
	}

	// Capture a frame (mat)

	Mat frame;

#ifdef USE_HIGUI
	string name = "Raw";
	namedWindow(name, CV_WINDOW_NORMAL);
#endif

	while(true)
	{
	vidStream.read(frame);

#ifdef USE_HIGUI
	imshow(name, frame);
#endif

	// Pass the mat to the process function
	// 0 < thresh < 255
	// blur > 0
	procImg(frame, 100, 100);
	}
	// Use boxes to navigate
	return 0;
}
