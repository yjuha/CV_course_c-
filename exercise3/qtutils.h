#ifndef QTUTILS_H_
#define QTUTILS_H_

#include <QApplication>
#include <QtCore>
#include <QtGui>
#include <QLabel>
#include <QGridLayout>

#include <opencv2/opencv.hpp>

namespace qtutils {

QImage cvmat2qimg(cv::Mat& img);

}
#endif
