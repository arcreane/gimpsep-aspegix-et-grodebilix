#ifndef PANORAMA_H
#define PANORAMA_H

#include <opencv2/core/mat.hpp>

class panorama {

    private:
        std::vector<cv::Mat> images;

    public:
        panorama(cv::Mat image);
        ~panorama();

        void addImage(cv::Mat image);
        cv::Mat stitchImages();

        cv::Mat stitchImagesGUI();
};



#endif
