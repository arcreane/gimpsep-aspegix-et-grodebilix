#include "canny.h"

canny::canny() {};
canny::~canny() {};

cv::Mat canny::detectEdges(cv::Mat image) {

  bool cancel = false;

  std::string windowName = "Edge detection";
  cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);


  int lowThreshold = 0;
  int highThreshold = 0;
  cv::createTrackbar("Low threshold", windowName, &lowThreshold, 500, 0, 0);
  cv::createTrackbar("High threshold", windowName, &highThreshold, 500, 0, 0);

  // Création de l'image en noir et blanc
  cv::Mat gray;
  cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

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

  return edges;

}