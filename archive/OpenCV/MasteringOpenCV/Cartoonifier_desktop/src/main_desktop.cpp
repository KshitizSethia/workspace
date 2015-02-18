//============================================================================
// Name        : Cartoonifier_desktop.cpp
// Author      : Mastering OpenCV
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "opencv2/opencv.hpp"
#include "cartoon.h"

using namespace std;
using namespace cv;

int main(int argc, char** argv) {

	// Get access to the camera.
	VideoCapture video;
	video.open("C:\\Google Drive\\MOOC\\optimization\\3 - 2 - Course Introduction - philosophy design grading rubric (1130).mp4");
//	video.open("C:\\Users\\Public\\Pictures\\Sample Pictures\\Koala.jpg");

	if (!video.isOpened()) {
	std::cerr << "ERROR: Could not access the camera or video!" <<
	std::endl;
	exit(1);
	}

	// Try to set the camera resolution.
	video.set(CV_CAP_PROP_FRAME_WIDTH , 640/2);
	video.set(CV_CAP_PROP_FRAME_HEIGHT , 480/2);

	while(true)
	{
		Mat videoFrame;
		video >> videoFrame;
		if(videoFrame.empty())
			break;

		Mat frameToDisplay(videoFrame.size(), CV_8UC3);

		Cartoon::cartoonifyImage(videoFrame, frameToDisplay);

		Mat faceOutline = Mat::zeros(frameToDisplay.size(), CV_8UC3);
		Cartoon::getFace(frameToDisplay, faceOutline);
		//overlay faceOutline
		addWeighted(frameToDisplay, 1.0, faceOutline, 0.7, 0, frameToDisplay, CV_8UC3);

		//cvtColor(videoFrame, frameToDisplay, CV_BGR2GRAY);
		imshow("Cartoonified Video", frameToDisplay);

		// IMPORTANT: Wait for at least 20 milliseconds,
		// so that the image can be displayed on the screen!
		// Also checks if a key was pressed in the GUI window.
		// Note that it should be a "char" to support Linux.
		char keypress = cv::waitKey(20); // Need this to see anything!
		if (keypress == 27) { // Escape Key
			// Quit the program
			break;
		}
	}
}
