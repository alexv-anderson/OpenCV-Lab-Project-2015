
#ifndef NAVIGATION_H
#define NAVIGATION_H

#include "imgProc.h"

using namespace std;

#define WAY_POINT_HEIGHT (6 + (5/8))	//inches
#define WAY_POINT_WIDTH (9 + (3/4))		//inches
#define WAY_POINT_RATIO (WAY_POINT_HEIGHT / WAY_POINT_WIDTH)

#define ANGLE_TOLERANCE 5
#define WAY_POINT_STOP_WIDTH 1266

struct NavBoxes
{
	bool foundWaypoint;
	box waypoint;
	vector<box> obstacles;
};

struct NavBits
{
	bool left, go, right;
};

NavBoxes getNavBoxes(vector<box> boxes)
{
	NavBoxes output;

	output.foundWaypoint = false;
	float minRatioError = 0.1;
	for(int i = 0; i < boxes.size(); i++)
	{
		box b = boxes[i];
		float ratio = b.bbox.height / b.bbox.width;

		float testError = fabs(WAY_POINT_RATIO - ratio);
		if(testError < minRatioError)
		{
			output.foundWaypoint = true;
			output.waypoint = b;
			minRatioError = testError;
		}
		else
			output.obstacles.push_back(b);
	}

	return output;
}

NavBits waypointNav(box waypoint)
{
	NavBits output;

	const float midAng = HORZ_VIEW_ANGLE / 2;

	float angError = waypoint.angle - midAng;
	if(angError < -ANGLE_TOLERANCE)
	{
		output.left = true;
		output.right = false;
	}
	else if(angError > ANGLE_TOLERANCE)
	{
		output.left = false;
		output.right = true;
	}
	else
	{
		output.left = false;
		output.right = false;
	}

	if(waypoint.bbox.width < WAY_POINT_STOP_WIDTH)
		output.go = true;
	else
		output.go = false;

	return output;
}

#endif	//NAVIGATION_H
