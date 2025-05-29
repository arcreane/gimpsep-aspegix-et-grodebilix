#ifndef FILTERS_H
#define FILTERS_H

#include <opencv2/opencv.hpp>

class filters {

     public:
       filters();
       ~filters();
       void applyFilters(cv::Mat &image, int filterIndex);
       cv::Mat applyFiltersGUI(const cv::Mat& input, int filterIndex);

};



#endif //FILTERS_H
