#include "canny.h"

canny::canny() {};
canny::~canny() {};


cv::Mat canny::detectEdges(cv::Mat image) {

  bool cancel = false;

  // Create the window
  std::string windowName = "Edge detection";
  cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);

  // Create the tackbars
  int lowThreshold = 50;
  int highThreshold = 150;
  cv::createTrackbar("Low threshold", windowName, &lowThreshold, 500, 0, 0);
  cv::createTrackbar("High threshold", windowName, &highThreshold, 500, 0, 0);

  // Create the black and white image
  cv::Mat gray;
  if (image.channels() == 3) {
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
  } else {
    gray = image;
  }


  cv::Mat edges;

  while (true) {
    // Edge detection
    cv::Canny(image, edges, lowThreshold, highThreshold);

    // Display the image
    cv::imshow(windowName, edges);

    // Wait for user input
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

  // Closing the window
  cv::destroyWindow(windowName);

  if (cancel) {
    return image;
  }

  return edges;

}

cv::Mat canny::detectEdgesGUI(cv::Mat image, int lowThreshold = 50, int highThreshold = 150) {

  cv::Mat gray;
  if (image.channels() == 3) {
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
  } else {
    gray = image;
  }

  cv::Mat edges;

  cv::Canny(gray, edges, lowThreshold, highThreshold);

  return edges;

}