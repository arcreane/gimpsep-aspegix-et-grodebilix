#ifndef FACEDETECTION_H
#define FACEDETECTION_H

#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>

class faceDetection {

  cv::CascadeClassifier faceCascade;

  public:
    faceDetection();
    ~faceDetection();

    cv::Mat detectFromImage(cv::Mat image);
    cv::Mat detectFromWebcam();

  cv::Mat detectFacesGUI(cv::Mat image);
};



#endif //FACEDETECTION_H
