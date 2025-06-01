#include "temperature.h"

using namespace cv;
using namespace std;

temperature::temperature() {}
temperature::~temperature() {}

void temperature::changeTemperature(cv::Mat &img, int temperature_value){

    // temperature_value : 0 (cold) à 200 (hot), 100 = no changes

    const double alpha = (temperature_value - 100) * 0.01;
    vector<Mat> channels(3);
    split(img, channels);

    // References to channels
    Mat &red = channels[2];
    Mat &blue = channels[0];

    // Change the balance
    if (alpha > 0)
    {
        red = red + (255 - red) * alpha * 0.3;
        blue = blue * (1 - alpha * 0.3);
    }
    else if (alpha < 0)
    {
        blue = blue + (255 - blue) * (-alpha) * 0.3;
        red = red * (1 + alpha * 0.3);
    }

    merge(channels, img);

}

cv::Mat temperature::changeTemperatureGUI(const cv::Mat& input, int temperature_value) {
    cv::Mat img = input.clone();
    changeTemperature(img, temperature_value);
    return img;
}