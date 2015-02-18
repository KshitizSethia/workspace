// Main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "HistogramEqualizer.h"
#include "ColorMultiplier.h"
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>

using namespace cv;

int main(int argc, char* argv[])
{
	//read image
	Mat originalImage = imread(argv[1]);
	//resize to smaller size
	resize(originalImage, originalImage, Size(512, 512));
	
	//demo histogram equalization
	//HistogramEqualizer::Demo(originalImage);

	//demo color equalization
	ColorMultiplier mult(originalImage, argv[2]);
	mult.Demo();

	//wait for user
	waitKey(0);

	return EXIT_SUCCESS;
}

