// SIFT.cpp : Defines the entry point for the console application.
//
// code picked from http://stackoverflow.com/questions/5461148/sift-implementation-with-opencv-2-2

#include "stdafx.h"
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2\nonfree\features2d.hpp>

#include <vector>

using namespace std;
using namespace cv;

int _tmain(int argc, _TCHAR* argv[])
{
  Mat image = cv::imread("C:\\Users\\sethiak\\Desktop\\carrom\\top-1.jpg", 0); //Load as grayscale

    // Create smart pointer for SIFT feature detector.
  SiftFeatureDetector featureDetector(30);// = SiftFeatureDetector::create("SIFT");
  
  vector<KeyPoint> keypoints;

  // Detect the keypoints
  featureDetector.detect(image, keypoints); // NOTE: featureDetector is a pointer hence the '->'.

  //Similarly, we create a smart pointer to the SIFT extractor.
  Ptr<DescriptorExtractor> featureExtractor = DescriptorExtractor::create("SIFT");

  // Compute the 128 dimension SIFT descriptor at each keypoint.
  // Each row in "descriptors" correspond to the SIFT descriptor for each keypoint
  Mat descriptors;
  featureExtractor->compute(image, keypoints, descriptors);

  // If you would like to draw the detected keypoint just to check
  Mat outputImage;
  Scalar keypointColor = Scalar(255, 0, 0);     // Blue keypoints.
  drawKeypoints(image, keypoints, outputImage, keypointColor, DrawMatchesFlags::DEFAULT);

  namedWindow("Output");
  imshow("Output", outputImage);

  char c = ' ';
  while ((c = waitKey(0)) != 'q');  // Keep window there until user presses 'q' to quit.

  return 0;
}