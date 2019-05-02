#include <QApplication>
#include <QtCore>
#include <QtGui>
#include <QLabel>
#include <QImage>
#include <QGridLayout>

#include <iostream>
#include <opencv2/opencv.hpp>

#include "qtutils.h"
#include "utils.h"

int main( int argc, char** argv ) {

    //! Help 
    if ( argc != 2 )
    {
        std::cout << "Usage: ./ex3 <Image_Path>\n" << std::endl;
        return -1;
    }

    //! Create cv::Mat object and load image
    cv::Mat img_;
    cv::Mat img;
    img_ = cv::imread(argv[1], 1);

    if ( !img_.data ) {
        std::cout << "No image data\n" << std::endl;
        return -1;
    }

    //! Resize input image to half
    float fx = 0.5;
    float fy = fx;
    int interpolation = cv::INTER_LINEAR;
    cv::resize(img_, img, cv::Size(), fx, fy, interpolation); 

    //! Convert image to grayscale
    cv::Mat gray;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

    //! Convert from CV_8U to CV_32F, and scale to the range [0,1]
    cv::Mat graysp; // saltnpepper noise corrupted
    cv::Mat graygn; // gaussian noise corrupted
    gray.convertTo(graysp, CV_32F);
    graysp /= 255.; 
    graygn = graysp.clone();

    //! Apply "salt'n pepa" noise
    // options: (1) "salt-&-pepper", (2) "gaussian", (3) ...
    imnoise(graysp, "salt-&-pepper", 0.05);

    //! Apply "gaussian" noise
    imnoise(graygn, "gaussian", 0.05);

    cv::Mat graysp_mf (graysp.size(), CV_32F, cv::Scalar(0));
    cv::Mat graysp_gf (graysp.size(), CV_32F, cv::Scalar(0));

    cv::Mat graygn_mf (graygn.size(), CV_32F, cv::Scalar(0));
    cv::Mat graygn_gf (graygn.size(), CV_32F, cv::Scalar(0));
    cv::Mat graygn_bf (graygn.size(), CV_32F, cv::Scalar(0));

    //! Apply 3x3 median filter to sp corrupted image
    int ksize_median = 5;
    cv::medianBlur(graysp, graysp_mf, ksize_median); 
    cv::medianBlur(graygn, graygn_mf, ksize_median);

    //! Apply 13x13 gaussian filter to sp corrupted image
    int ksize = 13;
    double sigma = 2.5;
    int ktype = CV_32F;
    cv::Mat k = cv::getGaussianKernel(ksize, sigma, ktype);
    //cv::filter2D(graysp, graysp_gf, graysp.depth(), k);
    conv2D(graysp, graysp_gf, k);
    conv2D(graygn, graygn_gf, k);

    float k_size = 11;
    float sigmad = 2.5;
    float sigmar = 0.1;
    bilateral(graygn, graygn_bf, k_size, sigmad, sigmar);

    //! Convert noise corrupted images from CV_32F to CV_8U
    cv::Mat graysp_8U;
    graysp *= 255.;
    graysp.convertTo(graysp_8U, CV_8U);

    cv::Mat graygn_8U;
    graygn *= 255.;
    graygn.convertTo(graygn_8U, CV_8U);

    //! Qt stuff 
    QApplication app(argc, argv);

    //! Convert cv::Mat to QImage
    QImage qimg_gn = qtutils::cvmat2qimg(graygn_8U);
    QImage qimg_sp = qtutils::cvmat2qimg(graysp_8U);
    QImage qimg = qtutils::cvmat2qimg(gray);

    //! QGridLayout for showing results
    QGridLayout *gridLayout = new QGridLayout;

    QLabel *tlabel1 = new QLabel;
    QLabel *tlabel2 = new QLabel;
    QLabel *tlabel3 = new QLabel;

    tlabel1->setText("original");
    tlabel2->setText("salt-and-pepper");
    tlabel3->setText("gaussian noise");

    QLabel *qlabel1 = new QLabel;
    QLabel *qlabel2 = new QLabel;
    QLabel *qlabel3 = new QLabel;

    qlabel1->setPixmap(QPixmap::fromImage(qimg));
    qlabel2->setPixmap(QPixmap::fromImage(qimg_sp));
    qlabel3->setPixmap(QPixmap::fromImage(qimg_gn));

    gridLayout->addWidget(tlabel1, 0, 0, 1, 1, Qt::AlignCenter);
    gridLayout->addWidget(tlabel2, 0, 1, 1, 1, Qt::AlignCenter);
    gridLayout->addWidget(tlabel3, 0, 2, 1, 1, Qt::AlignCenter);
    gridLayout->addWidget(qlabel1, 1, 0, 1, 1);
    gridLayout->addWidget(qlabel2, 1, 1, 1, 1);
    gridLayout->addWidget(qlabel3, 1, 2, 1, 1);

    QWidget *w = new QWidget();

    w->setLayout(gridLayout);
    w->setWindowTitle("Original and noise corrupted images");
    w->show();
    
    //! Result images of median filtering
    cv::Mat graysp_mf_U8;
    graysp_mf *= 255.;
    graysp_mf.convertTo(graysp_mf_U8, CV_8U);

    cv::Mat graysp_gf_U8;
    graysp_gf *= 255.;
    graysp_gf.convertTo(graysp_gf_U8, CV_8U);

    QGridLayout *gridLayout2 = new QGridLayout;

    QLabel *tlabel5 = new QLabel;
    QLabel *tlabel6 = new QLabel;
    QLabel *tlabel7 = new QLabel;

    QLabel *qlabel5 = new QLabel;
    QLabel *qlabel6 = new QLabel;
    QLabel *qlabel7 = new QLabel;

    tlabel5->setText("salt-and-pepper");
    tlabel6->setText("gaussian filtered");
    tlabel7->setText("median filtered");

    QImage qgraysp_mf = qtutils::cvmat2qimg(graysp_mf_U8);
    QImage qgraysp_gf = qtutils::cvmat2qimg(graysp_gf_U8);

    qlabel5->setPixmap(QPixmap::fromImage(qimg_sp));
    qlabel6->setPixmap(QPixmap::fromImage(qgraysp_gf));
    qlabel7->setPixmap(QPixmap::fromImage(qgraysp_mf));

    gridLayout2->addWidget(tlabel5, 0, 0, 1, 1, Qt::AlignCenter);
    gridLayout2->addWidget(tlabel6, 0, 1, 1, 1, Qt::AlignCenter);
    gridLayout2->addWidget(tlabel7, 0, 2, 1, 1, Qt::AlignCenter);
    gridLayout2->addWidget(qlabel5, 1, 0, 1, 1);
    gridLayout2->addWidget(qlabel6, 1, 1, 1, 1);
    gridLayout2->addWidget(qlabel7, 1, 2, 1, 1);

    QWidget *w2 = new QWidget();

    w2->setLayout(gridLayout2);
    w2->setWindowTitle("Salt-and-pepper results");
    w2->show();
 
    //! Result images of median filtering
    cv::Mat graygn_mf_U8;
    graygn_mf *= 255.;
    graygn_mf.convertTo(graygn_mf_U8, CV_8U);

    cv::Mat graygn_gf_U8;
    graygn_gf *= 255.;
    graygn_gf.convertTo(graygn_gf_U8, CV_8U);

    QGridLayout *gridLayout3 = new QGridLayout;

    QLabel *tlabel8 = new QLabel;
    QLabel *tlabel9 = new QLabel;
    QLabel *tlabel10 = new QLabel;

    QLabel *qlabel8 = new QLabel;
    QLabel *qlabel9 = new QLabel;
    QLabel *qlabel10 = new QLabel;

    tlabel8->setText("gaussian noise");
    tlabel9->setText("gaussian filtered");
    tlabel10->setText("median filtered");

    QImage qgraygn_mf = qtutils::cvmat2qimg(graygn_mf_U8);
    QImage qgraygn_gf = qtutils::cvmat2qimg(graygn_gf_U8);

    qlabel8->setPixmap(QPixmap::fromImage(qimg_gn));
    qlabel9->setPixmap(QPixmap::fromImage(qgraygn_gf));
    qlabel10->setPixmap(QPixmap::fromImage(qgraygn_mf));

    gridLayout3->addWidget(tlabel8, 0, 0, 1, 1, Qt::AlignCenter);
    gridLayout3->addWidget(tlabel9, 0, 1, 1, 1, Qt::AlignCenter);
    gridLayout3->addWidget(tlabel10, 0, 2, 1, 1, Qt::AlignCenter);
    gridLayout3->addWidget(qlabel8, 1, 0, 1, 1);
    gridLayout3->addWidget(qlabel9, 1, 1, 1, 1);
    gridLayout3->addWidget(qlabel10, 1, 2, 1, 1);

    QWidget *w3 = new QWidget();

    w3->setLayout(gridLayout3);
    w3->setWindowTitle("Gaussian noise results");
    w3->show();
 
    return app.exec();

}
