#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include <opencv2/opencv.hpp>

class temperature {

    public:
      temperature();
      ~temperature();
    void changeTemperature(cv::Mat &img, int temperature_value);

      cv::Mat changeTemperatureGUI(const cv::Mat &input, int temperature_value);
};


#endif //TEMPERATURE_H
