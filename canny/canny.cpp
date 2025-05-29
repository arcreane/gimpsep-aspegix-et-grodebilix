#include "canny.h"

canny::canny() {};
canny::~canny() {};


cv::Mat canny::detectEdges(cv::Mat image) {

  bool cancel = false;

  // Création de la fenêtre
  std::string windowName = "Edge detection";
  cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);

  // Création des trackbars
  int lowThreshold = 50;
  int highThreshold = 150;
  cv::createTrackbar("Low threshold", windowName, &lowThreshold, 500, 0, 0);
  cv::createTrackbar("High threshold", windowName, &highThreshold, 500, 0, 0);

  // Création de l'image en noir et blanc
  cv::Mat gray;
  if (image.channels() == 3) {
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
  } else {
    gray = image;
  }


  // Si on rajoute du flou ça rends plus clean sur certaines images mais on perds du détail
  // Je vais commenter pour l'instant à voir si on garde
  // cv::Mat blurred;
  // cv::GaussianBlur(gray, blurred, cv::Size(5, 5), 1.4);

  cv::Mat edges;

  while (true) {
    // Edge detection
    cv::Canny(image, edges, lowThreshold, highThreshold);

    // Affichage de l'image
    cv::imshow(windowName, edges);

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

  // Fermeture de la fenêtre
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