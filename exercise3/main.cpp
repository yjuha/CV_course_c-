#include <iostream>
#include <opencv2/opencv.hpp>

#include "utils.h"

int main(int argc, char** argv ) {

    //! Help 
    if ( argc != 2 )
    {
        std::cout << "Usage: ./ex3 <Image_Path>\n" << std::endl;
        return -1;
    }

    //! Create cv::Mat object and load image
    cv::Mat image;
    image = cv::imread(argv[1], 1);

    if ( !image.data )
    {
        std::cout << "No image data\n" << std::endl;
        return -1;
    }

    //! Convert image to grayscale
    cv::Mat gray;
    cv::cvtColor(image, gray, CV_BGR2GRAY);

    //! Convert from CV_8U to CV_32F, and scale to the range [0,1]
    cv::Mat grayf;
    gray.convertTo(grayf, CV_32F);
    grayf /= 255.;

    //! Apply "salt'n pepa" noise
    imnoise(grayf, 0.05);

    cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE );
    cv::imshow("Display Image", grayf);

    cv::waitKey(0);

    return 0;
}
