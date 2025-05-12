#include "erosion.h"

erosion::erosion() {};

erosion::~erosion() {}

cv::Mat erosion::erodeGrayScale(cv::Mat image,int erosionSize) {

  cv::Mat gray;
  cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

  cv::Mat binary;
  cv::threshold(gray, binary, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);


  cv::Mat element = getStructuringElement(cv::MORPH_RECT,cv::Size(2 * erosionSize + 1, 2 * erosionSize + 1), cv::Point(erosionSize, erosionSize));

  cv::Mat eroded;
  erode(binary, eroded, element);

  return eroded;

}

cv::Mat erosion::erodeColor(cv::Mat image,int erosionSize) {

  cv::Mat eroodedColor;
  cv::Mat kernel = getStructuringElement(cv::MORPH_RECT, cv::Size(2 * erosionSize + 1, 2 * erosionSize + 1));

  cv::erode(image, eroodedColor, kernel);

  return eroodedColor;

}

cv::Mat erosion::dilateGrayScale(cv::Mat image, int dilationSize) {
  cv::Mat gray;
  cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

  cv::Mat binary;
  cv::threshold(gray, binary, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

  cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(2 * dilationSize + 1, 2 * dilationSize + 1), cv::Point(dilationSize, dilationSize));

  cv::Mat dilated;
  cv::dilate(binary, dilated, element);

  return dilated;
}

cv::Mat erosion::dilateColor(cv::Mat image, int dilationSize) {

  cv::Mat dilatedColor;
  cv::Mat kernel = getStructuringElement(cv::MORPH_RECT, cv::Size(2 * dilationSize + 1, 2 * dilationSize + 1));

  cv::dilate(image, dilatedColor, kernel);

  return dilatedColor;

}