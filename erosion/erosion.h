#ifndef EROSION_H
#define EROSION_H

#include <opencv2/opencv.hpp>

class erosion {
public:
    erosion();

    ~erosion();

    cv::Mat erodeGrayScale(cv::Mat image, int erosionSize);

    cv::Mat erodeColor(cv::Mat image, int erosionSize);

    cv::Mat dilateGrayScale(cv::Mat image, int dilatationSize);

    cv::Mat dilateColor(cv::Mat image, int dilatationSize);
};


#endif
