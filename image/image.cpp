#include "image.h"

image::image(std::string path){
  // Open image from the specified path
  currentImage = cv::imread(path);
  historique.push_back(currentImage);
}

image::image(cv::Mat image) {
  // Update the image
  currentImage = image;
  historique.push_back(currentImage);
}

image::~image() {

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
  // Replace the current image with a new image from path
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

  // Options for the text
  int font = cv::FONT_HERSHEY_SIMPLEX;
  double fontScale = 1.0;
  int thickness = 1;
  cv::Size textSize = cv::getTextSize(windowName, font, 1, 1, NULL);

  while (true) {


    // We copy the image to not add text to the real one
    cv::Mat displayImage = historique[index].clone();

    // Displayed text
    std::string text = std::to_string(index + 1) + " / " + std::to_string(total);

    // Text size
    int baseline = 0;
    textSize = cv::getTextSize(text, font, fontScale, thickness, &baseline);

    // Bottom right
    cv::Point textOrg(displayImage.cols - textSize.width - 10, displayImage.rows - 10);

    // Overlay for the text to see it on a white background
    cv::putText(displayImage, text, textOrg, font, fontScale, cv::Scalar(0, 0, 0), thickness + 2);

    // Text in white
    cv::putText(displayImage, text, textOrg, font, fontScale, cv::Scalar(255, 255, 255), thickness);

    // Display the image
    cv::imshow(windowName, displayImage);

    // The arrow keys don't seem to work so i've replaced them with A and D

    int key = cv::waitKey(0);
    if (key == 27) { // ESC
      break;
    } else if (key == 'q' || key == 'Q') { // Left
      index = (index - 1 + total) % total;
    } else if (key == 'd' || key == 'D') { // Right
      index = (index + 1) % total;
    }

  }

  // Close the window
  cv::destroyWindow(windowName);

}

void image::restoreToVersion(int version) {

  if (version < 1 || version > historique.size()) {
    return;
  }

  // We set and old version of the image
  cv::Mat temp = currentImage;
  currentImage = historique[version] - 1;
  historique.push_back(temp);
}
