#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <list>
#include <opencv2/opencv.hpp>

class image {

  private:
    cv::Mat currentImage;
    std::list<cv::Mat> historique;

  public:
    image(std::string path);
    image(cv::Mat image);
    ~image();

    cv::Mat getImage();
    void setImage(cv::Mat image);
    void loadNewImage(std::string path);
    void addImageToHistorique(cv::Mat image);
    void undo();
    void showHistory();
    void restoreToVersion(int version);

};



#endif //IMAGE_H
