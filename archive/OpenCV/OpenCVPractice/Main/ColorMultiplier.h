#pragma once

#include "OpenCVIncludes.h";

class ColorMultiplier
{
public:
	int gain;
	static const int gain_divider=100;
	int bias;
	double gamma;
	string windowName;

	ColorMultiplier(Mat input, char* aGamma);

	void Demo();
	static void Display(int, void*);
private:
	Mat image;
};

