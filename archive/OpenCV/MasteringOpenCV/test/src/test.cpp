//============================================================================
// Name        : test.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

/*#include <iostream>
using namespace std;

int main() {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	return 0;
}*/


#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
  Mat im = imread(argc == 2 ? argv[1] : "C:\\Users\\sethiak\\Desktop\\temp\\koala.jpg", 1);
  if (im.empty())
  {
    cout << "Cannot open image!" << endl;
    return -1;
  }

  imshow("image", im);
  waitKey(0);

  return 0;
}
