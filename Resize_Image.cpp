#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Mat Resize_Image(const Mat& image, double scaleFactor, int width, int height);
bool Parameter(double& scaleFactor, int& width, int& height);
int Option();

int Resize() {
    string imagePath;
    cout << "Entrez le chemin de l'image : ";
    cin >> imagePath;

    Mat source = cv::imread(imagePath);
    if (source.empty()) {
        std::cerr << "Erreur : image non trouvée." << std::endl;
        return -1;
    }

    double scaleFactor = 0.0;
    int width = 0, height = 0;

    while (true) {
        if (!Parameter(scaleFactor, width, height))
            continue;
        
		Mat result = Resize_Image(source, scaleFactor, width, height);
        int action = Option();


        if (action == 1) {
            cout << "Configuration validée !\n";
            // Optionnel : imwrite("sortie.jpg", result);
            break;
        }
        else if (action == 3) {
            cout << "Annulation.\n";
            return 0;
        }
        // Sinon recommencer
    }

    return 0;
}

bool Parameter(double& scaleFactor, int& width, int& height) {
	// Initialisation des paramètres
    int choix;
    cout << "\n=== Choix du mode de redimensionnement ===\n";
    cout << "1. Par facteur\n";
    cout << "2. Par dimensions (largeur x hauteur)\n";
    cout << "Votre choix : ";
    cin >> choix;

    if (choix == 1) {
        cout << "Entrez le facteur (ex: 0.5 pour 50%) : ";
        cin >> scaleFactor;
        width = height = 0; // reset
    }
    else if (choix == 2) {
        cout << "Entrez la largeur en pixels : ";
        cin >> width;
        cout << "Entrez la hauteur en pixels : ";
        cin >> height;
        scaleFactor = 0.0; // reset
    }
    else {
        cerr << "Choix invalide.\n";
        return false;
    }

	return true;
}

Mat Resize_Image(const Mat& image, double scaleFactor, int width, int height) {
    Mat resized;

    if (scaleFactor > 0.0) {
        resize(image, resized, cv::Size(), scaleFactor, scaleFactor);
    }
    else if (width > 0 && height > 0) {
        resize(image, resized, cv::Size(width, height));
    }
    else {
        std::cerr << "Paramètres invalides pour le redimensionnement." << std::endl;
		return Mat(); // Retourne une image vide
    }

	// Affichage de l'image originale et de l'image redimensionnée
    imshow("Image Originale", image);
    imshow("Image Redimensionnée", resized);

	waitKey(0);
    destroyAllWindows();

    return resized;
}

int Option() {
    int action;
    cout << "\n=== Que souhaitez-vous faire ? ===\n";
    cout << "1. Valider le redimensionnement\n";
    cout << "2. Modifier les paramètres\n";
    cout << "3. Annuler et quitter\n";
    cout << "Votre choix : ";
    cin >> action;

	return action;
}