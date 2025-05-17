#ifndef CANNY_H
#define CANNY_H

#include <opencv2/opencv.hpp>

class canny {
    public:
      canny();
      ~canny();
      cv::Mat detectEdges(cv::Mat image);
};



#endif //CANNY_H
