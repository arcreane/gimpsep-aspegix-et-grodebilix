#ifndef BACKGROUNDREMOVER_H
#define BACKGROUNDREMOVER_H

#include <opencv2/opencv.hpp>


class backgroundRemover {

  cv::Mat image;
  cv::Vec3b selectedColor;
  bool colorSelected;

  static void onMouse(int event, int x, int y, int, void* userdata);

  public:
    backgroundRemover(cv::Mat img);
    ~backgroundRemover();
    cv::Mat foregroundExtraction();
    cv::Mat thresholding();
    cv::Mat chromaKey();

};

#endif //BACKGROUNDREMOVER_H
