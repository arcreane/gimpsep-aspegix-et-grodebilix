#include "canny.h"

canny::canny() {};
canny::~canny() {};

cv::Mat canny::detectEdges(cv::Mat image, double lowThreshold, double highThreshold) {

  cv::Mat gray;
  cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

  // Si on rajoute du flou ça rends plus clean sur certaines images mais on perds du détail
  // Je vais commenter pour l'instant à voir si on garde
  // cv::Mat blurred;
  // cv::GaussianBlur(gray, blurred, cv::Size(5, 5), 1.4);

  cv::Mat edges;
  cv::Canny(image, edges, lowThreshold, highThreshold);

  return edges;

}