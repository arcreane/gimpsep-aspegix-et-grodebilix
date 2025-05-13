#ifndef BRIGHTNESS_HPP
#define BRIGHTNESS_HPP

#include <opencv2/opencv.hpp>

enum class BrightnessMode {
    Lighten = 1,
    Darken = 2
};

enum class BrightnessAction {
    Valider = 1,
    Modifier = 2,
    Annuler = 3
};

class BrightnessAdjuster {
private:
    cv::Mat sourceImage;
    cv::Mat resultImage;
    BrightnessMode mode;
    int intensity;

public:
    BrightnessAdjuster();

    bool loadImage();
    bool getParameters();
    void adjust();
    void showImages();
    BrightnessAction postOptions();
    void run();
};

#endif
