#include "utils.h"
#include <random>

void imnoise(cv::Mat& src, float prob) {
    
    if ( src.channels() > 1 ) {
        std::cout << "Error: cv::Mat src is not a single channel image." << std::endl;
        return;
    }

    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution(0, 1);


    int rows = src.rows;
    int cols = src.cols;

    int i, j;
    float* p;

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
