#ifndef RETINA_CPP
#define RETINA_CPP

#include "Retina.hpp"

Retina::Retina()
{
    smaller_rf = SMALLER_RF;
    w_ratio = W_RATIO;
    s_ratio = S_RATIO;
    n_layers = N_LAYERS;
    spikes_lim = SPIKES_LIM;
}

Retina::~Retina()
{
}

//Mat Retina::GetImg(Mat img, int percentage)
//{
//
////    clock_t begin = clock();
//
//    Mat out;
////    cvtColor(img,img,CV_BGR2GRAY);
//    //out = img.clone();
//
//    out = Filter(img);
//
//
////    clock_t end = clock();
////    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
////    cout << elapsed_secs << endl;
//
//    return out;
//}

Mat Retina::GetImg(Mat img)
{

    Mat out1, out2, out3, out4;


    int fs1[4] = {smaller_rf, smaller_rf, smaller_rf*2+1, smaller_rf*2+1};

    out1 = DoG(img, img.cols, img.rows, fs1[0], fs1[0]/3.0, "ON");
    out2 = DoG(img, img.cols, img.rows, fs1[0], fs1[0]/3.0, "OFF");
    out3 = DoG(img, img.cols/2, img.rows/2, fs1[1], fs1[1]/3.0, "ON");
    out4 = DoG(img, img.cols/2, img.rows/2, fs1[1], fs1[1]/3.0, "OFF");

    Mat rs_out, rs_out1, rs_out2, rs_out3, rs_out4;

    rs_out1 = out1.reshape(1,1);
    rs_out2 = out2.reshape(1,1);
    rs_out3 = out3.reshape(1,1);
    rs_out4 = out4.reshape(1,1);
    hconcat(rs_out1,rs_out2,rs_out);
    hconcat(rs_out,rs_out3,rs_out);
    hconcat(rs_out,rs_out4,rs_out);

    rs_out.convertTo(rs_out, CV_64F);

    int k = 0;

    for (int i = 0;i<(int)(rs_out.rows*rs_out.cols); i++)
    {
        if (rs_out.at<double>(i) <= 130 )
        {
            rs_out.at<double>(i) = 0;
            k++;
        }
    }

    Mat out(img.rows,img.cols*2,CV_64F,Scalar(0));
    out  = out1;
    out.convertTo(out, CV_8U);

    cout << 1.0*(img.rows*img.rows*5/2-k)/(img.rows*img.rows*5/2)*100 << "%"<< endl;

    //img_out = reconstruct_DoG(out, img.cols, img.rows, int fs1, double sigma_inner);


//    clock_t end = clock();
//    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
//    cout << elapsed_secs << endl;

    return out;
}


//vector<unsigned char> Retina::GetSpikes( Mat img, int percentage )
//{
//    Mat out = DoG_Layers( img );
//    vector<unsigned char> array;
//    array.assign(out.datastart, out.dataend);
//    return array;
//}
//
//vector<unsigned char> Retina::GetSpikesPosition( Mat img, int percentage )
//{
//    Mat out = DoG_Layers( img );
//    vector<unsigned char> array;
//    array.assign(out.datastart, out.dataend);
//    return array;
//}
//
//vector<unsigned char> Retina::GetFirstSpikes( Mat img, int percentage )
//{
//    Mat out = DoG_Layers( img );
//    std::vector<unsigned char> array;
//    array.assign(out.datastart, out.dataend);
//    return array;
//}
//
//vector<unsigned char> Retina::GetFirstSpikesPosition( Mat img, int percentage )
//{
//    Mat out = DoG_Layers( img );
//    std::vector<unsigned char> array;
//    array.assign(out.datastart, out.dataend);
//    return array;
//}


/* ON and OFF centered Difference of Gaussians filter for selected ganglion cells */
Mat Retina::DoG(Mat img, int mx, int my, int fs1, double sigma_inner, string type)
{
    Mat aux1, aux2, img2, img_aux;

    int Mx = img.cols;
    int My = img.rows;

    double sigma_outer = sigma_inner*s_ratio;

    int fs2 = (int) (fs1-1)*floor(s_ratio)+1;

    Mat A1 = getGaussianKernel(fs2, sigma_inner, CV_64F );
    Mat Ker1 = A1*A1.t();

    Mat A2 = getGaussianKernel(fs2, sigma_outer, CV_64F );
    Mat Ker2 = A2*A2.t();

    Mat Ker = Ker1-Ker2;

    int top, bottom, left, right;

    /// Add borders to the image
    top = (int) (fs2-1)/2; bottom = (int) (fs2-1)/2;
    left = (int) (fs2-1)/2; right = (int) (fs2-1)/2;
    img_aux = img;

    copyMakeBorder( img, img_aux, top, bottom, left, right, BORDER_REPLICATE);

//    int mx = (int) img.cols*overlap/fs1;
//    int my = (int) img.rows*overlap/fs1;

    Mat out(my,mx,CV_64F,Scalar(0));

    if (type == "ON")
    {

        for (int j=0; j<my; j++) {
            for (int i=0; i<mx; i++) {
                Mat ON_ = img_aux.rowRange(j*My/my,fs2+j*My/my).colRange(i*Mx/mx,fs2+i*Mx/mx);
                Mat ON;
                ON_.convertTo(ON, CV_64F);
                ON = ON.mul(Ker1)-ON.mul(Ker2)*w_ratio;
//                out.at<double>(j,i)= (int)(sum(ON)[0]+255-sum(OFF)[0]);
                out.at<double>(j,i)= (int)(sum(ON)[0]);
            }
        }

    }
    if (type == "OFF")
    {

        Mat Const(fs2,fs2,CV_64F,Scalar(255));

        for (int j=0; j<my; j++) {
            for (int i=0; i<mx; i++) {
                Mat OFF_ = 255-img_aux.rowRange(j*My/my,fs2+j*My/my).colRange(i*Mx/mx,fs2+i*Mx/mx);
                Mat OFF;
                OFF_.convertTo(OFF, CV_64F);
                OFF = OFF.mul(Ker1)-OFF.mul(Ker2)*w_ratio;
                out.at<double>(j,i)= (int)(sum(OFF)[0]);
            }
        }

    }

    // output image
    normalize(out,out,0,255,NORM_MINMAX);
    out.convertTo(out, CV_8U);

    return out;
}

Mat Retina::reconstruct_DoG(Mat img,int Mx, int My, int fs1, double sigma_inner)
{
    Mat aux1, aux2, img2, img_aux;

    // int mx = img.cols;
    // int my = img.rows;

    return img;
}

void Retina::ReverseBytes( unsigned char * img_start, int img_size )
{
    unsigned char * img_end = img_start + img_size;
    reverse(img_start, img_end);

    
}

#endif
