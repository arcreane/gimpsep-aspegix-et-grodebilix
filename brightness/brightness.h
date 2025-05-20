#ifndef BRIGHTNESS_H
#define BRIGHTNESS_H

#include <opencv2/opencv.hpp>

class brightness {
private:
    int mode;   // 1 = lumineux, 2 = sombre
    int value;  // valeur de luminosité [0-100]

public:
    brightness();

    // Demande à l'utilisateur le mode et la valeur
    bool getParameters();

    // Applique la luminosité sur une image et affiche le résultat
    cv::Mat changeAndShow(const cv::Mat& image);

    // Méthode unique pour l'interface
    cv::Mat apply(const cv::Mat& image);
};

#endif
