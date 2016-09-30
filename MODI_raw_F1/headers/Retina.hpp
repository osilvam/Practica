#ifndef RETINA_HPP
#define RETINA_HPP

#include <opencv2/core/core.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <cmath>

using namespace cv;
using namespace std;

#define SMALLER_RF 3
#define W_RATIO 1
#define S_RATIO 3
#define N_LAYERS 8
#define SPIKES_LIM 0.6

class Retina
{
    int     smaller_rf; // number of pixels spanned by the filter in one axis (must be odd)
    double  w_ratio; // w_s/w_c
    int     s_ratio; // sigma_outer/sigma_inner
    int     n_layers; // number of layers
    double  spikes_lim; // from 0 to 1. If 0, takes all spikes for reconstruction

    public:

	Retina();
	~Retina();

    Mat GetImg(Mat img, int percentage);
    Mat GetImg(Mat img);
//    vector<unsigned char> GetSpikes(Mat img, int percentage);
//    vector<unsigned char> GetSpikesPosition(Mat img, int percentage);
//    vector<unsigned char> GetFirstSpikes(Mat img, int percentage);
//    vector<unsigned char> GetFirstSpikesPosition(Mat img, int percentage);
    Mat DoG(Mat img, int mx, int my, int fs1, double sigma_inner, string type);
    Mat reconstruct_DoG(Mat img,int Mx, int My, int fs1, double sigma_inner);
    void ReverseBytes( unsigned char * img_star, int img_size );

};

#endif
