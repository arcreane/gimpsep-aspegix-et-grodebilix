#ifndef RESIZE_H
#define RESIZE_H

#include <opencv2/opencv.hpp>

class resize {
private:
    double scaleFactor;
    int width;
    int height;

public:
    resize();

    // Demande à l'utilisateur les paramètres de redimensionnement
    bool getParameters(const cv::Mat& source);

    // Applique le redimensionnement sur l'image d'origine
    cv::Mat resizeAndShow(const cv::Mat& source);

    cv::Mat apply(const cv::Mat& input);
};

#endif 
