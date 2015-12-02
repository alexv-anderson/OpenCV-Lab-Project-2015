#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

using namespace cv;
using namespace std;

#define USE_HIGUI

#include "header.h"



// Function to get images

int main (int argc, char** argv) {
	// Open a video stream
	VideoCapture* vidStream;
	if(argc == 2)
	{
		cout << argv[1] << endl;
		vidStream = new VideoCapture(argv[1]);
	}
	else
		vidStream = new VideoCapture(0);

	if(!vidStream->isOpened())
	{
		cout << "Could not open camera\n";
		return -1;
	}

	// Capture a frame (mat)

	Mat frame;

	while(true)
	{
	//vidStream.read(frame);
		vidStream->read(frame);


	// Pass the mat to the process function
	// 0 < thresh < 255
	// blur > 0
	procImg(frame, 100, 100);
	}
	// Use boxes to navigate
	return 0;
}
