#include "erosion.h"

erosion::erosion() {};

erosion::~erosion() {}

cv::Mat erosion::erodeGrayScale(cv::Mat image) {

  bool cancel = false;

  // Création de la fenêtre
  std::string windowName = "Erode GrayScale";
  cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);

  // Création de la trackbar
  int erosionSize = 0;
  cv::createTrackbar("Erosion", windowName, &erosionSize, 10, 0, 0);

  // Transformation de l'image en noit et blanc
  cv::Mat gray;
  if (image.channels() == 3) {
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
  } else {
    gray = image;
  }

  // Matrice de treshold
  cv::Mat binary;
  cv::threshold(gray, binary, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

  // Création des matrics nécéssaires aux opérations
  cv::Mat element;
  cv::Mat eroded;

  while (true) {

    // On modifie l'image en fonction de la valeur de le trackbar
    element = getStructuringElement(cv::MORPH_RECT,cv::Size(2 * erosionSize + 1, 2 * erosionSize + 1), cv::Point(erosionSize, erosionSize));
    erode(binary, eroded, element);

    // Affichage de l'image
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

  cv::destroyWindow(windowName);

  if (cancel) {
    return image;
  }

  return eroded;

}

cv::Mat erosion::erodeColor(cv::Mat image) {

  bool cancel = false;

  // Création de la fenêtre
  std::string windowName = "Erode Color";
  namedWindow(windowName, cv::WINDOW_AUTOSIZE);

  // Création de la trackbar
  int erosionSize = 0;
  cv::createTrackbar("Erosion", windowName, &erosionSize, 10, 0, 0);

  // Création de la matrice et du kernel
  cv::Mat eroodedColor;
  cv::Mat kernel;

  while (true) {

    kernel = getStructuringElement(cv::MORPH_RECT, cv::Size(2 * erosionSize + 1, 2 * erosionSize + 1));
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

  cv::destroyWindow(windowName);

  if (cancel) {
    return image;
  }

  return eroodedColor;

}

cv::Mat erosion::dilateGrayScale(cv::Mat image) {

  bool cancel = false;

  // Création de la fenêtre
  std::string windowName = "Dilate GrayScale";
  namedWindow(windowName, cv::WINDOW_AUTOSIZE);

  // Création de la trackbar
  int dilationSize = 0;
  cv::createTrackbar("Dilatation", windowName, &dilationSize, 10, 0, 0);

  // On met l'image en noir et blanc
  cv::Mat gray;
  if (image.channels() == 3) {
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
  } else {
    gray = image;
  }

  // Création de la matrice binary
  cv::Mat binary;
  cv::threshold(gray, binary, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

  cv::Mat element;
  cv::Mat dilated;
  while (true) {

    // Dilatation
    element = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(2 * dilationSize + 1, 2 * dilationSize + 1), cv::Point(dilationSize, dilationSize));
    cv::dilate(binary, dilated, element);

    // Affichage de l'image
    cv::imshow(windowName, dilated);

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

  return dilated;
}

cv::Mat erosion::dilateColor(cv::Mat image) {

  bool cancel = false;

  // Création de la fenêtre
  std::string windowName = "Dilate Color";
  namedWindow(windowName, cv::WINDOW_AUTOSIZE);

  // Création de la trackbar
  int dilationSize = 0;
  cv::createTrackbar("Dilatation", windowName, &dilationSize, 10, 0, 0);

  cv::Mat dilatedColor;
  cv::Mat kernel;
  while (true) {

    // On applique la dilatation
    kernel = getStructuringElement(cv::MORPH_RECT, cv::Size(2 * dilationSize + 1, 2 * dilationSize + 1));
    cv::dilate(image, dilatedColor, kernel);

    // Affichage de l'image
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

  cv::destroyWindow(windowName);

  if (cancel) {
    return image;
  }

  return dilatedColor;

}