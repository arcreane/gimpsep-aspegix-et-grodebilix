#ifndef RESIZE_H
#define RESIZE_H

#include <opencv2/opencv.hpp>

namespace gimpsep {
    class resize {
    private:
        double scaleFactor;
        int width;
        int height;

    public:
        resize();

        // Demande � l'utilisateur les param�tres de redimensionnement
        bool getParameters(const cv::Mat& source);

        // Applique le redimensionnement sur l'image d'origine
        cv::Mat resizeAndShow(const cv::Mat& source);

        cv::Mat apply(const cv::Mat& input);

        cv::Mat resizeAndShowGUI(const cv::Mat &source, bool byFactor, double scaleFactor, int width, int height);
    };
}
#endif 
