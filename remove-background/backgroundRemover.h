#ifndef BACKGROUNDREMOVER_H
#define BACKGROUNDREMOVER_H

#include <opencv2/opencv.hpp>


class backgroundRemover {

  cv::Mat image;

  public:
    backgroundRemover(cv::Mat img);
    ~backgroundRemover();
    cv::Mat foregroundExtraction();
    cv::Mat thresholding();

};

#endif //BACKGROUNDREMOVER_H
