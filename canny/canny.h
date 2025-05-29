#ifndef CANNY_H
#define CANNY_H

#include <opencv2/opencv.hpp>

class canny {
    public:
      canny();
      ~canny();
      cv::Mat detectEdges(cv::Mat image);

      cv::Mat detectEdgesGUI(cv::Mat image, int lowThreshold, int highThreshold);
};



#endif //CANNY_H
