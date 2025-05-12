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
  for (const cv::Mat img : historique) {
    std::string title = "Version n°" + std::to_string(i);
    cv::imshow(title, img);
    i++;
  }
}

void image::restoreToVersion(int version) {
  cv::Mat temp = currentImage;
  auto it = historique.begin();
  std::advance(it, version);
  currentImage = *it;
  historique.push_front(temp);
}
