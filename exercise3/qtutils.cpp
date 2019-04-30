#include "qtutils.h"

QImage qtutils::cvmat2qimg(cv::Mat& img) {
    QImage qimg(img.data, img.cols,
            img.rows, static_cast<int>(img.step),
            QImage::Format_Grayscale8);
    return qimg;
}
