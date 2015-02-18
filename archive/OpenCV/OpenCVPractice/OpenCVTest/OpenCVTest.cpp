// OpenCVTest.cpp : Defines the entry point for the console application.
//
// initial code taken from http://jepsonsblog.blogspot.in/2012/07/installation-guide-opencv-24-with.html

#include "stdafx.h"
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\features2d\features2d.hpp>
#include <stdlib.h>
#include <stdio.h>


int _tmain(int argc, _TCHAR* argv[])
{
	cv::Mat originalImg = cv::imread("C:\\Users\\sethiak\\Desktop\\aayu.jpg");
	cv::imshow("Original",originalImg);
	
	cv::Mat gaussImage;
	int sigma = 5;
	cv::Size kernelSize(sigma*3, sigma*3); 
	cv::GaussianBlur(originalImg, gaussImage, kernelSize, sigma, sigma);
	std::string heading = "Gaussian blur with sigma " + std::to_string(static_cast<long long>(sigma));
	cv::imshow(heading.c_str(), gaussImage);
	
	cv::Mat medianImage;
	int medianSize = 5;
	cv::medianBlur(originalImg, medianImage, medianSize);
	heading = "Median Blur with median " + std::to_string(static_cast<long long>(medianSize));
	cv::imshow(heading, medianImage);
	
	cv::Mat bilateralImage;
	int filterDiameter = 10;
	cv::bilateralFilter(originalImg, bilateralImage, filterDiameter, 255, filterDiameter);
	heading = "Bilateral Blur";
	cv::imshow(heading, bilateralImage);

    cv::waitKey();

	return EXIT_SUCCESS;
}
