#ifndef RESIZE_IMAGE_HPP
#define RESIZE_IMAGE_HPP

#include <opencv2/opencv.hpp>

class ImageResizer {
private:
    cv::Mat sourceImage;
    double scaleFactor;
    int width;
    int height;

public:
    ImageResizer();

    bool loadImage();
    bool getParameters();

    cv::Mat resizeAndShow();

    int postResizeOptions();
    void run();
};

#endif 
