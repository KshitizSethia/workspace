/*
 * cartoon.h
 *
 *  Created on: Jul 23, 2013
 *      Author: sethiak
 */

#ifndef CARTOON_H_
#define CARTOON_H_

#include "opencv2/opencv.hpp"
using namespace cv;

class Cartoon
{

public:
	static void cartoonifyImage(Mat& srcColor, Mat& destination)
	{

		Mat mask;
		//extractEdgeMask(srcColor, mask, 7, 5, 80, true, 2); //for evil mode
		extractEdgeMask(srcColor, mask, 7, 5, 80);

		Mat filtered;
		repeatBilateralFilter(srcColor, filtered, 7);

		// put mask on top of bigImg
		//destination = Mat(bigImg);
		filtered.copyTo(destination, mask);
	}

	static void getFace(Mat& src, Mat &faceOutline)
	{
		Size srcSize = src.size();

		// Draw the color face onto a black background.
//		Mat faceOutline = Mat::zeros(srcSize, CV_8UC3);
		Scalar color = CV_RGB(255,255,0); // Yellow.
		int thickness = 4;
		// Use 70% of the screen height as the face height.
		int sw = srcSize.width;
		int sh = srcSize.height;
		int faceH = sh/2 * 70/100; // "faceH" is the radius of the ellipse.
		// Scale the width to be the same shape for any screen width.
		int faceW = faceH * 72/100;
		// Draw the face outline.
		ellipse(faceOutline, Point(sw/2, sh/2), Size(faceW, faceH),
		0, 0, 360, color, thickness, CV_AA);

		// Draw the eye outlines, as 2 arcs per eye.
		int eyeW = faceW * 23/100;
		int eyeH = faceH * 11/100;
		int eyeX = faceW * 48/100;
		int eyeY = faceH * 13/100;
		Size eyeSize = Size(eyeW, eyeH);
		// Set the angle and shift for the eye half ellipses.
		int eyeA = 15; // angle in degrees.
		int eyeYshift = 11;
		// Draw the top of the right eye.
		ellipse(faceOutline, Point(sw/2 - eyeX, sh/2 - eyeY),
				eyeSize, 0, 180+eyeA, 360-eyeA, color, thickness, CV_AA);
		// Draw the bottom of the right eye.
		ellipse(faceOutline, Point(sw/2 - eyeX, sh/2 - eyeY),
				eyeSize, 0, 0+eyeA, 180-eyeA, color, thickness, CV_AA);
		// Draw the top of the left eye.
		ellipse(faceOutline, Point(sw/2 + eyeX, sh/2 - eyeY),
				eyeSize, 0, 180+eyeA, 360-eyeA, color, thickness, CV_AA);
		// Draw the bottom of the left eye.
		ellipse(faceOutline, Point(sw/2 +eyeX, sh/2 - eyeY - eyeYshift),
				eyeSize, 0, 0+eyeA, 180-eyeA, color, thickness, CV_AA);

		// Draw the bottom lip of the mouth.
		int mouthY = faceH * 48/100;
		int mouthW = faceW * 45/100;
		int mouthH = faceH * 6/100;
		ellipse(faceOutline, Point(sw/2, sh/2 + mouthY),
				Size(mouthW, mouthH), 0, 0, 180, color, thickness, CV_AA);

		// Draw anti-aliased text.
		int fontFace = FONT_HERSHEY_COMPLEX;
		float fontScale = 1.0f;
		int fontThickness = 2;
		string szMsg = "Put your face here";
		putText(faceOutline, szMsg, Point(sw * 23/100, sh * 10/100),
		fontFace, fontScale, color, fontThickness, CV_AA);


	}

	static void fillColor(Mat& srcColor)//, Mat& faceOutline)
	{
		Size smallSize(srcColor.size().width/2, srcColor.size().height/2);

		Mat yuv = Mat(smallSize, CV_8UC3);
		cvtColor(srcColor, yuv, CV_BGR2YCrCb);

		int sw = smallSize.width;
		int sh = smallSize.height;
		Mat mask, maskPlusBorder, edges;
		maskPlusBorder = Mat::zeros(sh+2, sw+2, CV_8UC1);
		mask = maskPlusBorder(Rect(1,1,sw,sh)); // mask is in maskPlusBorder.
		extractEdgeMask(srcColor, edges, 7, 5, 80);
		resize(edges, mask, smallSize); // Put edges in both of them.

		const int EDGES_THRESHOLD = 80;
		threshold(mask, mask, EDGES_THRESHOLD, 255, THRESH_BINARY);
		dilate(mask, mask, Mat());
		erode(mask, mask, Mat());

		int const NUM_SKIN_POINTS = 6;
		Point skinPts[NUM_SKIN_POINTS];
		skinPts[0] = Point(sw/2, sh/2 - sh/6);
		skinPts[1] = Point(sw/2 - sw/11, sh/2 - sh/6);
		skinPts[2] = Point(sw/2 + sw/11, sh/2 - sh/6);
		skinPts[3] = Point(sw/2, sh/2 + sh/16);
		skinPts[4] = Point(sw/2 - sw/9, sh/2 + sh/16);
		skinPts[5] = Point(sw/2 + sw/9, sh/2 + sh/16);

		const int LOWER_Y = 60;
		const int UPPER_Y = 80;
		const int LOWER_Cr = 25;
		const int UPPER_Cr = 15;
		const int LOWER_Cb = 20;
		const int UPPER_Cb = 15;
		Scalar lowerDiff = Scalar(LOWER_Y, LOWER_Cr, LOWER_Cb);
		Scalar upperDiff = Scalar(UPPER_Y, UPPER_Cr, UPPER_Cb);

		const int CONNECTED_COMPONENTS = 4; // To fill diagonally, use 8.
		const int flags = CONNECTED_COMPONENTS | FLOODFILL_FIXED_RANGE \
		| FLOODFILL_MASK_ONLY;
		Mat edgeMask = mask.clone(); // Keep a copy of the edge mask.
		// "maskPlusBorder" is initialized with edges to block floodFill().
		for (int i=0; i< NUM_SKIN_POINTS; i++) {
		floodFill(yuv, maskPlusBorder, skinPts[i], Scalar(), NULL,
		lowerDiff, upperDiff, flags);
		}

		mask-=edges;

		Mat smallImgBGR(srcColor.size().height/2, srcColor.size().width/2, CV_8UC3);
		resize(srcColor, smallImgBGR, smallSize, 0, 0, INTER_LINEAR);
		int Red = 0;
		int Green = 70;
		int Blue = 0;
//		add(smallImgBGR, CV_RGB(Red, Green, Blue), smallImgBGR, mask);
	}

private:
	static inline void extractEdgeMask(Mat&srcColor,
			Mat& mask,
			int medianBlurFilterSize,
			int laplacianFilterSize,
			int edgeThreshold,
			bool evilMode = false,
			int thresholdDivisorForEvilMode=1)
	{
		Mat gray(srcColor.size(), CV_8U);
		cvtColor(srcColor, gray, CV_BGR2GRAY);

		Mat edges, edges2;
		if(evilMode)
		{
			Scharr(gray, edges, CV_8U, 1, 0);
			Scharr(gray, edges2, CV_8U, 1, 0, -1);
			edges+=edges2;
		}
		else
		{
			medianBlur(gray, gray, medianBlurFilterSize);
			Laplacian(gray, edges, CV_8U, laplacianFilterSize);
		}

		threshold(edges, mask, edgeThreshold/thresholdDivisorForEvilMode, 255, THRESH_BINARY_INV);
	}

	static inline void repeatBilateralFilter(Mat& srcColor, Mat& dst, int repetitions)
	{
		// convert image to smaller size
		Mat smallSrcColor(srcColor.size().height/2, srcColor.size().width/2, CV_8UC3);
		resize(srcColor, smallSrcColor, smallSrcColor.size(), 0, 0, INTER_LINEAR);

		//apply filter
		Mat temp(smallSrcColor.size(), CV_8UC3);
		int kSize = 9;
		double sigmaColor = 9;
		double sigmaSpace = 7;

		for(int i=0; i<repetitions; i++)
		{
			bilateralFilter(smallSrcColor, temp, kSize, sigmaColor, sigmaSpace);
			bilateralFilter(temp, smallSrcColor, kSize, sigmaColor, sigmaSpace);
		}

		// bring back image to original size
		resize(smallSrcColor, dst, srcColor.size(), 0,0, INTER_LINEAR);
	}

};

#endif /* CARTOON_H_ */