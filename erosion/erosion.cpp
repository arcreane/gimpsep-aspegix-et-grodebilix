#include "erosion.h"

erosion::erosion() {};

erosion::~erosion() {}

cv::Mat erosion::erodeGrayScale(cv::Mat image) {

  bool cancel = false;

  // Creating the window
  std::string windowName = "Erode GrayScale";
  cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);

  // Creation of the trackbar
  int erosionSize = 0;
  cv::createTrackbar("Erosion", windowName, &erosionSize, 10, 0, 0);

  // Transforming the image into a black and white version
  cv::Mat gray;
  if (image.channels() == 3) {
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
  } else {
    gray = image;
  }

  // Threshold matrix
  cv::Mat binary;
  cv::threshold(gray, binary, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

  // Creation of the necessary matrix's
  cv::Mat element;
  cv::Mat eroded;

  while (true) {

    // Modify the image based on the value of the trackbar
    element = getStructuringElement(cv::MORPH_RECT,cv::Size(2 * erosionSize + 1, 2 * erosionSize + 1), cv::Point(erosionSize, erosionSize));
    erode(binary, eroded, element);

    // Display the image
    cv::imshow(windowName, eroded);

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

  // Close the window
  cv::destroyWindow(windowName);

  // Cancel the modifications
  if (cancel) {
    return image;
  }

  return eroded;

}

cv::Mat erosion::erodeColor(cv::Mat image) {

  bool cancel = false;

  // Creating the window
  std::string windowName = "Erode Color";
  namedWindow(windowName, cv::WINDOW_AUTOSIZE);

  // Creation of the trackbar
  int erosionSize = 0;
  cv::createTrackbar("Erosion", windowName, &erosionSize, 10, 0, 0);

  // Creating the matrix and the kernel
  cv::Mat eroodedColor;
  cv::Mat kernel;

  while (true) {

    // initializing the kernel
    kernel = getStructuringElement(cv::MORPH_RECT, cv::Size(2 * erosionSize + 1, 2 * erosionSize + 1));

    // Erosion
    cv::erode(image, eroodedColor, kernel);
    cv::imshow(windowName, eroodedColor);

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

  // Cancel the modifications
  if (cancel) {
    return image;
  }

  return eroodedColor;

}

cv::Mat erosion::erodeGUI(cv::Mat image, int size, int mode) {
  cv::Mat output;

  // Make sure that size is non-negative
  int kernelSize = std::max(0, size);
  cv::Mat element = cv::getStructuringElement(
      cv::MORPH_RECT,
      cv::Size(2 * kernelSize + 1, 2 * kernelSize + 1),
      cv::Point(kernelSize, kernelSize)
  );

  switch (mode) {
    case 0: { // Grayscale erosion
      cv::Mat gray, binary;
      if (image.channels() == 3) {
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
      } else {
        gray = image;
      }
      cv::threshold(gray, binary, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
      cv::erode(binary, output, element);
      break;
    }
    case 1: { // Color erosion
      cv::erode(image, output, element);
      break;
    }
    case 2: { // Grayscale dilation
      cv::Mat gray, binary;
      if (image.channels() == 3) {
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
      } else {
        gray = image;
      }
      cv::threshold(gray, binary, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
      cv::dilate(binary, output, element);
      break;
    }
    case 3: { // Color dilation
      cv::dilate(image, output, element);
      break;
    }
    default: {
      // Invalid mode: return original
      output = image.clone();
      break;
    }
  }

  return output;
}


cv::Mat erosion::dilateGrayScale(cv::Mat image) {

  bool cancel = false;

  // Creation of the window
  std::string windowName = "Dilate GrayScale";
  namedWindow(windowName, cv::WINDOW_AUTOSIZE);

  // Creation of the trackbar
  int dilationSize = 0;
  cv::createTrackbar("Dilatation", windowName, &dilationSize, 10, 0, 0);

  // We put the image in black and white
  cv::Mat gray;
  if (image.channels() == 3) {
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
  } else {
    gray = image;
  }

  // Creation of the binary matrix
  cv::Mat binary;
  cv::threshold(gray, binary, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

  cv::Mat element;
  cv::Mat dilated;

  while (true) {

    // Dilatation
    element = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(2 * dilationSize + 1, 2 * dilationSize + 1), cv::Point(dilationSize, dilationSize));
    cv::dilate(binary, dilated, element);

    // Display the image
    cv::imshow(windowName, dilated);

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

  // Cancel modifications
  if (cancel) {
    return image;
  }

  return dilated;
}

cv::Mat erosion::dilateColor(cv::Mat image) {

  bool cancel = false;

  // Creation of the window
  std::string windowName = "Dilate Color";
  namedWindow(windowName, cv::WINDOW_AUTOSIZE);

  // Creation of the trackbar
  int dilationSize = 0;
  cv::createTrackbar("Dilatation", windowName, &dilationSize, 10, 0, 0);

  cv::Mat dilatedColor;
  cv::Mat kernel;
  while (true) {

    // Dilatation
    kernel = getStructuringElement(cv::MORPH_RECT, cv::Size(2 * dilationSize + 1, 2 * dilationSize + 1));
    cv::dilate(image, dilatedColor, kernel);

    // Display the image
    cv::imshow(windowName, dilatedColor);

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

  // Close the window
  cv::destroyWindow(windowName);

  // Cancel modifications
  if (cancel) {
    return image;
  }

  return dilatedColor;

}