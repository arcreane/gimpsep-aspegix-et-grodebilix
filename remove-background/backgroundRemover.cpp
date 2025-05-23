#include "backgroundRemover.h"

backgroundRemover::backgroundRemover(cv::Mat img) {
  image = img.clone();
}
backgroundRemover::~backgroundRemover() {}

cv::Mat backgroundRemover::foregroundExtraction() {
  cv::Mat mask;
  cv::Mat bgModel, fgModel;

  cv::Rect rect(50, 50, image.cols - 100, image.rows - 100);

  mask.create(image.size(), CV_8UC1);
  mask.setTo(cv::GC_BGD);

  cv::grabCut(image, mask, rect, bgModel, fgModel, 5, cv::GC_INIT_WITH_RECT);

  cv::Mat resultMask = (mask == cv::GC_FGD) | (mask == cv::GC_PR_FGD);

  cv::Mat foreground(image.size(), image.type(), cv::Scalar(0, 0, 0));
  image.copyTo(foreground, resultMask);

  return foreground;
}

cv::Mat backgroundRemover::thresholding() {

  bool cancel = false;

  cv::Mat gray;
  cv::Mat binary;
  cv::Mat foreground;

  std::string windowName = "backgroundRemover";
  cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);

  int threshold = 0;
  cv::createTrackbar("Treshold", windowName, &threshold, 255, 0, 0);

  if (image.channels() == 3) {
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
  } else {
    gray = image;
  }

  while (true) {
    cv::threshold(gray, binary, threshold, 255, cv::THRESH_BINARY_INV);

    cv::Mat foreground;
    image.copyTo(foreground, binary);

    cv::imshow(windowName, foreground);

    int key = cv::waitKey(10);

    //ESC
    if (key == 27) {
      cancel = true;
      break;
    }

    // ENTER
    if (key == 13 || key == 10) {
      break;
    }
  }

  cv::destroyWindow(windowName);

  if (cancel) {
    return image;
  }

  return foreground;

}

void backgroundRemover::onMouse(int event, int x, int y, int, void* userdata) {
  if (event != cv::EVENT_LBUTTONDOWN) return;

  // On trouve la couleur à l'emplacement de la sourie
  backgroundRemover* self = static_cast<backgroundRemover*>(userdata);
  self->selectedColor = self->image.at<cv::Vec3b>(y, x);
  self->colorSelected = true;

}

cv::Mat backgroundRemover::chromaKey() {

  colorSelected = false;

  cv::Mat img = image.clone();
  std::string windowName = "Select a color to remove";
  cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);
  cv::setMouseCallback(windowName, onMouse, this);

  while (!colorSelected) {
    cv::imshow(windowName, image);
    if (cv::waitKey(10) == 27) {
      return image;  // ESC
    }
  }
  cv::destroyWindow(windowName);

  cv::Mat selected(1, 1, CV_8UC3, selectedColor);
  cv::Mat selectedHSV;
  cv::cvtColor(selected, selectedHSV, cv::COLOR_BGR2HSV);
  cv::Vec3b targetHSV = selectedHSV.at<cv::Vec3b>(0, 0);

  int hTolerance = 10;
  int sTolerance = 60;
  int vTolerance = 60;

  cv::Scalar lower(targetHSV[0] - hTolerance, std::max(0, targetHSV[1] - sTolerance), std::max(0, targetHSV[2] - vTolerance));
  cv::Scalar upper(targetHSV[0] + hTolerance, std::min(255, targetHSV[1] + sTolerance), std::min(255, targetHSV[2] + vTolerance));

  cv::Mat hsvImg;
  cv::cvtColor(img, hsvImg, cv::COLOR_BGR2HSV);
  cv::Mat mask;
  cv::inRange(hsvImg, lower, upper, mask);


  cv::Mat result(image.size(), image.type(), cv::Scalar(0, 0, 0));
  cv::Mat invertedMask;
  cv::bitwise_not(mask, invertedMask);
  image.copyTo(result, invertedMask);


  return result;

}
