#include "interface.h"
#include "erosion/erosion.h"
#include "canny/canny.h"

interface::interface() {}

interface::interface(const cv::Mat &image) {
  setCurrentImage(image);
}


void interface::setCurrentImage(cv::Mat img) {
  image = img.clone();
}

void interface::loadNewImage(std::string path) {
  image = cv::imread(path);
}

void interface::showCurrentImage(const std::string& imageName = "") const {

  if (imageName.empty()) {
    cv::imshow("image", image);
  } else {
    cv::imshow(imageName, image);
  }

  cv::startWindowThread();
  cv::waitKey(0);
  cv::destroyAllWindows();

}

cv::Mat interface::getCurrentImage() {
  return image;
}

void interface::chooseOperation() {

  int chosen;
  std::string path;

  std::cout << "1. Load and image with it's path" << std::endl;
  std::cout << "2. Show the current image" << std::endl;
  std::cout << "3. Dilatation / Erosion" << std::endl;
  std::cout << "4. Resize" << std::endl;
  std::cout << "5. Lighten / Darken" << std::endl;
  std::cout << "6. Panorama / stitching" << std::endl;
  std::cout << "7. Canny edge detection" << std::endl;
  std::cout << "8. Quit" << std::endl;

  std::cout << "Choose an operation to perform : " << std::endl;
  std::cin >> chosen;

  switch (chosen) {
    case 1:

      std::cout << "Enter the path of the image you wish to edit : ";
      std::cin >> path;
      loadNewImage(path);

      break;
    case 2:
      showCurrentImage();
      break;
    case 3 : {

      int option;
      int erotionSize;
      std::cout << "1. Grayscale erosion" << std::endl;
      std::cout << "2. Color erosion" << std::endl;
      std::cout << "3. Grayscale dilatation" << std::endl;
      std::cout << "4. Color dilatation" << std::endl;
      std::cout << "What modification would you like to do : ";
      std::cin >> option;

      std::cout << "Input erosion / dilatation size : ";
      std::cin >> erotionSize;

      switch (option) {
        case 1: {
          erosion *er = new erosion();
          setCurrentImage(er->erodeGrayScale(getCurrentImage(), erotionSize));
          delete er;
          break;
        }
        case 2: {
          erosion *er = new erosion();
          setCurrentImage(er->erodeColor(getCurrentImage(), erotionSize));
          delete er;
          break;
        }
        case 3: {
          erosion *er = new erosion();
          setCurrentImage(er->dilateGrayScale(getCurrentImage(), erotionSize));
          delete er;
          break;
        }
        case 4: {
          erosion *er = new erosion();
          setCurrentImage(er->dilateColor(getCurrentImage(), erotionSize));
          delete er;
          break;
        }
        default:
          break;
      }


      break;
    }
    case 4:
      // TODO : add resize
      break;
    case 5:
      // TODO: add lighten / darken
      break;
    case 6:
      // TODO: add panorama / stitching
      break;
    case 7: {

      double lowThreshold;
      double highThreshold;

      std::cout << "Enter the low threshhold (50 is good) :";
      std::cin >> lowThreshold;
      std::cout << "Enter the high threshhold (150 is good) :";
      std::cin >> highThreshold;

      canny *can = new canny();
      setCurrentImage(can->detectEdges(getCurrentImage(), lowThreshold, highThreshold));
      delete can;

      break;
    }
    case 8:
      std::exit(0);
      break;
    default:
    chooseOperation();
  }

}
