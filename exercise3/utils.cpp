#include "utils.h"
#include <random>

enum noise_modes {
    saltnpepa_noise,
    gaussian_noise,
};

noise_modes lut (std::string const& ncode) {
    if (ncode == "salt-&-pepper") return saltnpepa_noise;
    if (ncode == "gaussian") return gaussian_noise;
}

void gaussian(cv::Mat& src, float prob) {
    
    //! Random number generator
    std::default_random_engine generator;
    std::normal_distribution<float> distribution(0, prob);

    int rows = src.rows;
    int cols = src.cols;

    int i, j;
    float* p;
    
    //! Apply Gaussian noise
    for (i = 0; i < rows; ++i) {
        p = src.ptr<float>(i);
        for (j = 0; j < cols; ++j) {
            float number = distribution(generator);
            p[j]+=number;
        }
    }

}

void saltnpepa(cv::Mat& src, float prob) {

    //! Random number generator (uniform)
    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution(0, 1);

    int rows = src.rows;
    int cols = src.cols;

    int i, j;
    float* p;

    //! Apply saltnpepa noise 
    for ( i = 0; i < rows; ++i ) {
        p = src.ptr<float>(i);
        for ( j = 0; j < cols; ++j ) {
            float number = distribution(generator);
            if (number < prob) {
                p[j] = 0;
            } else if (number > 1 - prob) {
                p[j] = 1;
            }
         }
    }    
}


void imnoise(cv::Mat& src, std::string noise_mode, float prob) {
    
    if ( src.channels() > 1 ) {
        std::cout << "Error: cv::Mat src is not a single channel image." << std::endl;
        return;
    }

    switch ( lut( noise_mode ) ) {
        case saltnpepa_noise:
            saltnpepa(src, prob);
            break;
        case gaussian_noise:
            gaussian(src, prob);
            break;
        default:
            std::cout << "Not implemented" << std::endl;
    }
}

void conv2D(cv::Mat& src, cv::Mat& dst, cv::Mat& k) {
    double *srcData = (double*) src.data;

    int width = src.cols;
    int height = src.rows;

    int kw = k.cols;
    int kh = k.rows;

    for(int i = 0; i < width; i++) {
        for(int j = 0; j < height; j++) {
	    for (int ki = 0; ki < kw; ki++) {
	        for (int kj = 0; kj < kh; kj++) {
            	    double val = srcData[(i + ki) * width + (j + kj)] * k[ki * kw + kj];


		}   
	    }
        }
    }

}
