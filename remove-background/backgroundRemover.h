#ifndef BACKGROUNDREMOVER_H
#define BACKGROUNDREMOVER_H

#include <qcolor.h>
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

  cv::Mat backgroundRemoverGUI(int mode, int thresholdValue, int hTolerance, int sTolerance, int vTolerance);

  cv::Mat thresholdingGUI(int threshold);

  cv::Mat chromaKeyGUI(QColor qcolor, int hTolerance, int sTolerance, int vTolerance);
};

#endif //BACKGROUNDREMOVER_H
