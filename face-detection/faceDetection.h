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


};



#endif //FACEDETECTION_H
