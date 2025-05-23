#include "image.h"
#include <filesystem>

image::image(std::string path){
  // On ouvre l'image à partir du chemin spécifié
  currentImage = cv::imread(path);
  historique.push_back(currentImage);
}

image::image(cv::Mat image) {
  // Mise à jour de l'image
  currentImage = image;
  historique.push_back(currentImage);
}

cv::Mat image::getImage() {
  return currentImage;
}

void image::setImage(cv::Mat image) {
  currentImage = image;
}

void image::addImageToHistorique(const cv::Mat& image) {
  historique.push_back(image.clone());
}


void image::loadNewImage(const std::string& path) {
  // Remplacer l'image actuelle avec une nouvelle image
  if (!currentImage.empty()) {
    historique.push_back(currentImage.clone());
  }
  currentImage = cv::imread(path);
  if (currentImage.empty()) {
    std::cerr << "Failed to load image from: " << path << std::endl;
  }
}

void image::undo() {
  if (historique.size() > 1) {
    historique.pop_back();
    currentImage = historique.back();
  } else {
    std::cout << "History is empty" << std::endl;
  }
}

void image::showHistory() {

  if (historique.empty()) {
    std::cout << "Empty history" << std::endl;
    return;
  }

  std::string windowName = "Historique";
  const int total = historique.size();
  int index = 0;

  cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);

  // Options pour le texte en bas à droite
  int font = cv::FONT_HERSHEY_SIMPLEX;
  double fontScale = 1.0;
  int thickness = 1;
  cv::Size textSize = cv::getTextSize(windowName, font, 1, 1, NULL);

  while (true) {


    // On fait une copie pour pas rajouter du texte sur l'image pour de vrai
    cv::Mat displayImage = historique[index].clone();

    // Le texte qu'on affiche
    std::string text = std::to_string(index + 1) + " / " + std::to_string(total);

    // Taille du texte
    int baseline = 0;
    cv::Size textSize = cv::getTextSize(text, font, fontScale, thickness, &baseline);

    // En bas à droite
    cv::Point textOrg(displayImage.cols - textSize.width - 10, displayImage.rows - 10);

    // Overlay pout le texte en noir pour qu'on puisse le voir sur fond blanc
    cv::putText(displayImage, text, textOrg, font, fontScale, cv::Scalar(0, 0, 0), thickness + 2);

    // Texte en blanc
    cv::putText(displayImage, text, textOrg, font, fontScale, cv::Scalar(255, 255, 255), thickness);

    // Affichage de l'image
    cv::imshow(windowName, displayImage);

    // Visiblement les flèches ça marche pas trop donc j'ai mis Q et D

    int key = cv::waitKey(0);
    if (key == 27) { // ESC
      break;
    } else if (key == 'q' || key == 'Q') { // Gauche
      index = (index - 1 + total) % total;
    } else if (key == 'd' || key == 'D') { // Droite
      index = (index + 1) % total;
    }

  }

  // Fermeture de la fenêtre
  cv::destroyWindow(windowName);

}

void image::restoreToVersion(int version) {

  if (version < 1 || version > historique.size()) {
    return;
  }

  // On remet une ancienne vesrion de l'image
  cv::Mat temp = currentImage;
  currentImage = historique[version - 1];
  historique.push_back(temp);
}

cv::Mat image::loadImageFromWebcam() {

  cv::Mat result;

  cv::VideoCapture cap = cv::VideoCapture(0);

  if (!cap.isOpened()) {
    return result;
  }

  std::string windowName = "Webcam";
  cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);

  while (true) {

    cv::Mat frame;

    cap >> frame;

    imshow(windowName, frame);

    int key = cv::waitKey(1);
    if (key == 27 || key == 13 || key == 10) {
      result = frame.clone();
      break;
    }

  }

  cap.release();
  cv::destroyWindow(windowName);

  return result;

}

void image::saveImage() {
  std::string basePath = "./output/output";
  std::string extension = ".png";
  std::string savePath = basePath + extension;
  int version = 1;

  // On vérifie que y'a pas déjà un ficiher qui s'appelle comme ça
  // Sinon on rajoute un chiffre derrière comme dans les explorateurs de fichiers
  while (std::filesystem::exists(savePath)) {
    savePath = basePath + "_" + std::to_string(version) + extension;
    version++;
  }

  // Enregistrement de l'image
  if (cv::imwrite(savePath, getImage())) {
    std::cout << "Image saved to " << savePath << std::endl;
  } else {
    std::cerr << "Could not save image." << std::endl;
  }
}