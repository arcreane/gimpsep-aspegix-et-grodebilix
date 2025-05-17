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
