#include "brightness.h"
#include <iostream>

using namespace std;
using namespace cv;

brightness::brightness() : mode(0), value(0) {}

bool brightness::getParameters() {
    cout << "\n=== Modification de la luminosité ===" << endl;
    cout << "1. Rendre l'image plus lumineuse" << endl;
    cout << "2. Rendre l'image plus sombre" << endl;
    cout << "Votre choix : ";
    cin >> mode;

    if (mode != 1 && mode != 2) {
        cerr << "Choix invalide.\n";
        return false;
    }

    cout << "Entrez la valeur de luminosité [0 - 100] : ";
    cin >> value;

    if (value < 0 || value > 100) {
        cerr << "Valeur invalide.\n";
        return false;
    }

    return true;
}

cv::Mat brightness::changeAndShow(const cv::Mat& image) {
    Mat result = image.clone();
    double factor = value / 100.0;

    if (mode == 1) {
        result += Scalar(255 * factor, 255 * factor, 255 * factor);
    }
    else if (mode == 2) {
        result = result * (1.0 - factor);
    }

    namedWindow("Image Originale", WINDOW_NORMAL);
    namedWindow("Image Modifiée", WINDOW_NORMAL);

    imshow("Image Originale", image);
    imshow("Image Modifiée", result);

    cout << "\nAppuyez sur 'Echap' sur une image pour quitter\n";

    while (true) {
        int key = waitKey(1);
        if (key == 27) {
            destroyAllWindows();
            return result;
        }
    }
    return result;
}

cv::Mat brightness::apply(const cv::Mat& image) {
    if (!getParameters()) {
        return Mat();
    }
    return changeAndShow(image);
}
