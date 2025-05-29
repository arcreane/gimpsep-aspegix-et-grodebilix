#ifndef BRIGHTNESS_H
#define BRIGHTNESS_H

#include <opencv2/opencv.hpp>

class brightness {
private:
    int mode;   // 1 = lumineux, 2 = sombre
    int value;  // valeur de luminosit� [0-100]

public:
    brightness();

    // Demande � l'utilisateur le mode et la valeur
    bool getParameters();

    // Applique la luminosit� sur une image et affiche le r�sultat
    cv::Mat changeAndShow(const cv::Mat& image);

    // M�thode unique pour l'interface
    cv::Mat apply(const cv::Mat& image);

    cv::Mat changeAndShowGUI(const cv::Mat &image, int mode, int value);
};

#endif
