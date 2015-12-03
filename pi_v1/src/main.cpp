#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <iostream>

using namespace cv;
using namespace std;

#define SHOW_PROC_FEED

#include "imgProc.h"
#include "navigation.h"
#include "driveGPIO.h"

int main (int argc, char** argv)
{

	// Open a video stream
	VideoCapture* vidStream;
	if(argc == 2)
	{
		//stream from file
		cout << argv[1] << endl;
		vidStream = new VideoCapture(argv[1]);

	}
	else
		vidStream = new VideoCapture(0);	//stream from webcam

	if(!vidStream->isOpened())
	{
		cout << "Could not open camera\n";
		return -1;
	}

	// Capture a frame (mat)
	Mat frame;
	while(true)
	{
		vidStream->read(frame);

		// Pass the mat to the process function
		// 0 < thresh < 255
		// blur > 0
		vector<box> boxes = procImg(frame, 100, 100);

		//filter through boxes to find waypoint amongst the obstacles
		NavBoxes navBoxes = getNavBoxes(boxes);

		//generate command string to send through the GPIO
		NavBits navBits;
		if(navBoxes.foundWaypoint)
			navBits = waypointNav(navBoxes.waypoint);
		else
		{
			navBits.go = false;
			navBits.left = false;
			navBits.right = false;
		}

		//send command string through GPIO
		sendNavBits(navBits);
	}

	return 0;
}
