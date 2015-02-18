#pragma once

#include "OpenCVIncludes.h"

class HistogramEqualizer
{
public:
	static Mat EqualizeColor(Mat sourceImage);

	static Mat EqualizeGrayscale(Mat sourceImage, Mat &grayscaledInput);

	static void Demo(Mat originalImage);
};

