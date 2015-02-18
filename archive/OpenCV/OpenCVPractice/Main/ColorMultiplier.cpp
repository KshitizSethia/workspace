#include "StdAfx.h"
#include "ColorMultiplier.h"

using namespace std;

void ColorMultiplier::Demo()
{
	namedWindow(windowName, CV_WINDOW_AUTOSIZE);
	createTrackbar("multiplier", windowName, &gain, gain_divider*10, &ColorMultiplier::Display, this);
	createTrackbar("displacer", windowName, &bias, 256, &ColorMultiplier::Display, this);
	
	Display(0,this);
}

void ColorMultiplier::Display(int, void* objPtr)
{
	ColorMultiplier* myObj = (ColorMultiplier*) objPtr;

	Mat output;
	myObj->image.convertTo(output, -1,((double)myObj->gain/(double)gain_divider) , myObj->bias);
	pow(output, -1*myObj->gamma, output);
	imshow(myObj->windowName, output);
}

ColorMultiplier::ColorMultiplier(Mat input, char* aGamma)
{
	windowName = "Color Multiplier";
	gain=gain_divider;
	bias=0;
	gamma=atof(aGamma);
	image=input;
}