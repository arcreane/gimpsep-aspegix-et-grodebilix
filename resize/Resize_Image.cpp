#include "Resize_Image.hpp"
#include <iostream>

using namespace std;
using namespace cv;

ImageResizer::ImageResizer() : scaleFactor(0.0), width(0), height(0) {}

bool ImageResizer::loadImage() {
    string imagePath;
    cout << "Entrez le chemin de l'image : ";
    cin >> imagePath;

    sourceImage = imread(imagePath);
    if (sourceImage.empty()) {
        cerr << "Erreur : image non trouvee." << endl;
        return loadImage();
    }
    return true;
}

bool ImageResizer::getParameters() {
    int choix;
    cout << "\n=== Choix du mode de redimensionnement ===\n";
    cout << "Image chargee est de dimension : " << sourceImage.cols << " x " << sourceImage.rows << endl;
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

Mat ImageResizer::resizeAndShow() {
    Mat resized;

    if (scaleFactor > 0.0) {
        resize(sourceImage, resized, Size(), scaleFactor, scaleFactor);
    }
    else if (width > 0 && height > 0) {
        resize(sourceImage, resized, Size(width, height));
    }
    else {
        cerr << "Paramètres invalides pour le redimensionnement." << endl;
        return Mat();
    }

    namedWindow("Image Originale", WINDOW_NORMAL);
    namedWindow("Image Redimensionnée", WINDOW_NORMAL);

    imshow("Image Originale", sourceImage);
    imshow("Image Redimensionnée", resized);

    cout << "\nAppuyez sur 'Echap' sur une image pour quitter\n";


    while (true) {
        int key = waitKey(1);
        if (key == 27) {
            destroyAllWindows();
            return resized;
        }
    }
}

int ImageResizer::postResizeOptions() {
    int action;
    cout << "\n=== Que souhaitez-vous faire ? ===\n";
    cout << "1. Valider le redimensionnement\n";
    cout << "2. Modifier les paramètres\n";
    cout << "3. Annuler et quitter\n";
    cout << "Votre choix : ";
    cin >> action;

    return action;
}

void ImageResizer::run() {
    if (!loadImage()) return;

    while (true) {
        if (!getParameters())
            continue;

        Mat result = resizeAndShow();
        if (result.empty()) {
            cout << "Erreur lors du redimensionnement. Reessayer.\n";
            continue;
        }

        int action = postResizeOptions();
        if (action == 1) {
            cout << "Configuration validee !\n";
            // Optional: imwrite("output.jpg", result);
            break;
        }
        else if (action == 3) {
            cout << "Annulation.\n";
            break;
        }
    }
}