#include "resize.h"
#include <iostream>

using namespace std;
using namespace cv;

resize::resize() : scaleFactor(0.0), width(0), height(0) {}

bool resize::getParameters(const cv::Mat& source) {
    int choix;
    cout << "\n=== Choix du mode de redimensionnement ===\n";
    cout << "Image chargee est de dimension : " << source.cols << " x " << source.rows << endl;
    cout << "1. Par facteur\n";
    cout << "2. Par dimensions (largeur x hauteur)\n";
    cout << "Votre choix : ";
    cin >> choix;

    if (choix == 1) {
        cout << "Entrez le facteur (ex: 0.5 pour 50%) : ";
        cin >> scaleFactor;
        width = height = 0;
    }
    else if (choix == 2) {
        cout << "Entrez la largeur en pixels : ";
        cin >> width;
        cout << "Entrez la hauteur en pixels : ";
        cin >> height;
        scaleFactor = 0.0;
    }
    else {
        cerr << "Choix invalide.\n";
        return false;
    }

    return true;
}

cv::Mat resize::resizeAndShow(const cv::Mat& source) {
    Mat resized;

    if (scaleFactor > 0.0) {
        cv::resize(source, resized, Size(), scaleFactor, scaleFactor);
    }
    else if (width > 0 && height > 0) {
        cv::resize(source, resized, Size(width, height));
    }
    else {
        cerr << "Param�tres invalides pour le redimensionnement." << endl;
        return Mat();
    }

    namedWindow("Image Originale", WINDOW_NORMAL);
    namedWindow("Image Redimensionn�e", WINDOW_NORMAL);

    imshow("Image Originale", source);
    imshow("Image Redimensionn�e", resized);

    cout << "\nAppuyez sur 'Echap' sur une image pour quitter\n";

    while (true) {
        int key = waitKey(1);
        if (key == 27) {
            destroyAllWindows();
            return resized;
        }
    }
    return resized;
}

cv::Mat resize::apply(const cv::Mat& input) {
    if (!getParameters(input)) {
        return cv::Mat();  // Annul�
    }
    return resizeAndShow(input);
}