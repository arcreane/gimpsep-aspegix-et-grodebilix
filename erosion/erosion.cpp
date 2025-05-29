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

  // Fermeture de la fenêtre
  cv::destroyWindow(windowName);

  // On annule les modifications
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

    // Création du kernel
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

  // Fermeture de la fenêtre
  cv::destroyWindow(windowName);

  // On annule les modifications
  if (cancel) {
    return image;
  }

  return eroodedColor;

}

cv::Mat erosion::erodeGUI(cv::Mat image, int size, int mode) {
  cv::Mat output;

  // Ensure size is non-negative
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

    // On attends l'input de l'utilisateur
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

  // Fermeture de le fenêtre
  cv::destroyWindow(windowName);

  // On annule les modifications
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

  // Fermeture de la fenêtre
  cv::destroyWindow(windowName);

  // On annule les modifications
  if (cancel) {
    return image;
  }

  return dilatedColor;

}