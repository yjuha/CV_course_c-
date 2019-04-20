#ifndef UTILS_H_
#define UTILS_H_

#include <opencv2/opencv.hpp>

void gaussian(cv::Mat& src, float prob);
void imnoise(cv::Mat& src, std::string mode, float prob);

void conv2D(cv::Mat& src, cv::Mat& dst, cv::Mat& k);
#endif
