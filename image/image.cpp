#include "image.h"

image::image(std::string path){
  currentImage = cv::imread(path);
  historique.push_front(currentImage);
}

image::image(cv::Mat image) {
  currentImage = image;
  historique.push_front(currentImage);
}

cv::Mat image::getImage() {
  return currentImage;
}

void image::setImage(cv::Mat image) {
  currentImage = image;
}

void image::addImageToHistorique(cv::Mat image) {
  historique.push_front(image);
}


void image::loadNewImage(std::string path) {
  if (!currentImage.empty()) { // Normalement c'est pas possible que ce soit vide mais au cas où
    historique.push_front(currentImage);
    currentImage = cv::imread(path);
  } else {
    currentImage = cv::imread(path);
  }
}

void image::undo() {
  if (historique.size() > 1) {
    historique.pop_front();
    currentImage = historique.front();
  } else {
    std::cout << "History is empty" << std::endl;
  }
}

void image::showHistory() {
  int i = 0;
  std::vector<std::string> windowNames;

  for (const cv::Mat& img : historique) {
    std::string title = "Version n°" + std::to_string(i);
    cv::namedWindow(title, cv::WINDOW_AUTOSIZE); // Ensure window is registered
    cv::imshow(title, img);
    windowNames.push_back(title);
    i++;
  }

  cv::waitKey(0);

  for (const std::string& title : windowNames) {
    cv::destroyWindow(title);
  }
}

void image::restoreToVersion(int version) {
  cv::Mat temp = currentImage;
  auto it = historique.begin();
  std::advance(it, version);
  currentImage = *it;
  historique.push_front(temp);
}
