#ifndef INTERFACE_H
#define INTERFACE_H

#include <opencv2/opencv.hpp>

class interface {

  cv::Mat image;

  public:
    interface();
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
