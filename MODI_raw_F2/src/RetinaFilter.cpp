#ifndef RETINAFILTER_CPP
#define RETINAFILTER_CPP

#include <opencv2/core/core.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <ctime>
#include "Retina.hpp"
#include "RIAR.hpp"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{

    Retina * retina = new Retina();

    Mat img, out;

    img = imread(argv[1], CV_LOAD_IMAGE_COLOR);   // Read the file
    cvtColor(img, img, CV_BGR2GRAY );

    namedWindow("Input Image", WINDOW_AUTOSIZE); // Create a window for display.
    namedWindow("Output Image", WINDOW_AUTOSIZE); // Create a window for display.

    out = retina -> GetImg(img);

    imshow("Input Image", img);
    imshow("Output Image", out);

    waitKey(0);

    return 0;
}

#endif
