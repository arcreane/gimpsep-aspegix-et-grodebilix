#include "Brightness.hpp"
#include <iostream>

using namespace std;
using namespace cv;

BrightnessAdjuster::BrightnessAdjuster() : intensity(0), mode(BrightnessMode::Lighten) {}

bool BrightnessAdjuster::loadImage() {
    string imagePath;
    cout << "Entrez le chemin de l'image : ";
    cin >> imagePath;

    sourceImage = imread(imagePath, IMREAD_COLOR);
    if (sourceImage.empty()) {
        cerr << "Erreur : image non trouvee." << endl;
        return loadImage();
    }
    return true;
}

bool BrightnessAdjuster::getParameters() {
    int choix;
    cout << "\n=== Modification de la luminosite ===\n";
    cout << "1. Rendre l'image plus lumineuse\n";
    cout << "2. Rendre l'image plus sombre\n";
    cout << "Votre choix : ";
    cin >> choix;

    if (choix != 1 && choix != 2) {
        cerr << "Choix invalide.\n";
        return false;
    }

    mode = (choix == 1) ? BrightnessMode::Lighten : BrightnessMode::Darken;

    cout << "Entrez la valeur de luminosité [0 - 100] : ";
    cin >> intensity;

    if (intensity < 0 || intensity > 100) {
        cerr << "Valeur invalide.\n";
        return false;
    }

    return true;
}

void BrightnessAdjuster::adjust() {
    resultImage = sourceImage.clone();
    double factor = intensity / 100.0;

    if (mode == BrightnessMode::Lighten) {
        resultImage = sourceImage + Scalar(255 * factor, 255 * factor, 255 * factor);
    }
    else {
        resultImage = sourceImage * (1.0 - factor);
    }
}

void BrightnessAdjuster::showImages() {
    Mat resultImage;
    namedWindow("Image Originale", WINDOW_NORMAL);
    namedWindow("Image Modifiée", WINDOW_NORMAL);

    imshow("Image Originale", sourceImage);
    imshow("Image Modifiée", resultImage);

    cout << "\nAppuyez sur 'Echap' sur une image pour quitter\n";


    while (true) {
        int key = waitKey(1);
        if (key == 27) {
            destroyAllWindows();
            break;
        }
    }
}

BrightnessAction BrightnessAdjuster::postOptions() {
    int action;
    cout << "\n=== Que souhaitez-vous faire ? ===\n";
    cout << "1. Valider la modification\n";
    cout << "2. Refaire les paramètres\n";
    cout << "3. Annuler et quitter\n";
    cout << "Votre choix : ";
    cin >> action;

    switch (action) {
    case 1: return BrightnessAction::Valider;
    case 2: return BrightnessAction::Modifier;
    case 3: return BrightnessAction::Annuler;
    default:
        cerr << "Choix invalide. On recommence.\n";
        return BrightnessAction::Modifier;
    }
}

void BrightnessAdjuster::run() {
    if (!loadImage()) return;

    while (true) {
        if (!getParameters()) continue;

        adjust();
        showImages();

        BrightnessAction action = postOptions();
        if (action == BrightnessAction::Valider) {
            cout << "Modification validée !" << endl;
            // Optionnel : imwrite("image_modifiee.jpg", resultImage);
            break;
        }
        else if (action == BrightnessAction::Annuler) {
            cout << "Annulation." << endl;
            break;
        }
    }
}
int main() {
    BrightnessAdjuster adjuster;
    adjuster.run();
    return 0;
}