#ifndef EROSION_H
#define EROSION_H

#include <opencv2/opencv.hpp>

class erosion {
public:
    erosion();

    ~erosion();

    cv::Mat erodeGrayScale(cv::Mat image);

    cv::Mat erodeColor(cv::Mat image);

    cv::Mat dilateGrayScale(cv::Mat image);

    cv::Mat dilateColor(cv::Mat image);
};


#endif
