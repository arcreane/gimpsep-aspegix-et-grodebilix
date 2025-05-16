#ifndef INTERFACE_H
#define INTERFACE_H

#include <opencv2/opencv.hpp>
#include "image/image.h"

class interface {

  //cv::Mat image;
  image* img;

  public:
    interface();
    //interface(const cv::Mat &image);
    interface(image* img);
    ~interface();
    void loadNewImage(std::string path);
    void setCurrentImage(cv::Mat image);
    void showCurrentImage(const std::string& imageName) const;
    cv::Mat getCurrentImage();
    void chooseOperation();
  private:
    std::string getImageNameFromPath(std::string path);
};



#endif //INTERFACE_H
