#include "StdAfx.h"
#include "HistogramEqualizer.h"


Mat HistogramEqualizer::EqualizeColor(Mat inputImage)
{
	if(inputImage.channels() >= 3)
    {
        Mat ycrcb;

        cvtColor(inputImage,ycrcb,CV_BGR2YCrCb);

        vector<Mat> channels;
        split(ycrcb,channels);

        equalizeHist(channels[0], channels[0]);

        Mat result;
        merge(channels,ycrcb);

        cvtColor(ycrcb,result,CV_YCrCb2BGR);

        return result;
    }
    return Mat();
}

Mat HistogramEqualizer::EqualizeGrayscale(Mat colorInput, Mat &grayscaledInput)
{
	cvtColor(colorInput, grayscaledInput, CV_BGR2GRAY);

	Mat result;

	equalizeHist(grayscaledInput, result);

	return result;
}

void HistogramEqualizer::Demo(Mat originalImage)
{
	Mat converted = HistogramEqualizer::EqualizeColor(originalImage);
	//display results
	imshow("Original", originalImage);
	imshow("Equalized", converted);
	//do  histogram equalization in grayscale
	Mat grayscaleOriginal;
	Mat grayscaleEqualized = HistogramEqualizer::EqualizeGrayscale(originalImage, grayscaleOriginal);
	//display results
	imshow("Grayscale Original",	grayscaleOriginal);
	imshow("Grayscale Equalized",	grayscaleEqualized);
}
