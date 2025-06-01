#include "interface.h"
#include "erosion/erosion.h"
#include "canny/canny.h"
#include "brightness/brightness.h"
#include "resize/resize.h"
#include "panorama/panorama.h"
#include "objectDetection/objectDetection.h"

interface::interface() {}
interface::~interface() {}

interface::interface(image* newImg) {
  img = newImg;
  //setCurrentImage(image);
}


void interface::setCurrentImage(cv::Mat newImage) {
  img->setImage(newImage);
}

void interface::loadNewImage(std::string path) {
  //image = cv::imread(path);
  img->loadNewImage(path);
}

void interface::showCurrentImage(const std::string& imageName = "") const {

  if (imageName.empty()) {
    cv::imshow("image", img->getImage());
  } else {
    cv::imshow(imageName, img->getImage());
  }

  cv::startWindowThread();
  cv::waitKey(0);
  cv::destroyAllWindows();

}

cv::Mat interface::getCurrentImage() {
  return img->getImage();
}

void interface::chooseOperation() {

  int chosen;
  std::string path;

  std::cout << "--- GESTION DE L'IMAGE ---" << std::endl;
  std::cout << "1. Load and image with it's path" << std::endl;
  std::cout << "2. Show the current image" << std::endl;
  std::cout << "3. Afficher l'historique des modifications" << std::endl;
  std::cout << "4. Restaurer une ancienne version de l'image" << std::endl;
  std::cout << "--- OPERATIONS SUR L'IMAGE ---" << std::endl;
  std::cout << "5. Dilatation / Erosion" << std::endl;
  std::cout << "6. Resize" << std::endl;
  std::cout << "7. Lighten / Darken" << std::endl;
  std::cout << "8. Panorama / stitching" << std::endl;
  std::cout << "9. Canny edge detection" << std::endl;
  std::cout << "10. Remove backgound" << std::endl;
  std::cout << "11. Object detection" << std::endl;
  std::cout << "12. Quit" << std::endl;

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
    case 3: {
      img->showHistory();
      break;
    }
    case 4: {

      int version;
      std::cout << "Quelle version de l'image voulez vous restaurer :";
      std::cin >> version;
      img->restoreToVersion(version);
      break;
    }
    case 5 : {

      int option;
      std::cout << "1. Grayscale erosion" << std::endl;
      std::cout << "2. Color erosion" << std::endl;
      std::cout << "3. Grayscale dilatation" << std::endl;
      std::cout << "4. Color dilatation" << std::endl;
      std::cout << "What modification would you like to do : ";
      std::cin >> option;

      switch (option) {
        case 1: {
          erosion *er = new erosion();
          setCurrentImage(er->erodeGrayScale(getCurrentImage()));
          img->addImageToHistorique(getCurrentImage());
          delete er;
          break;
        }
        case 2: {
          erosion *er = new erosion();
          setCurrentImage(er->erodeColor(getCurrentImage()));
          img->addImageToHistorique(getCurrentImage());
          delete er;
          break;
        }
        case 3: {
          erosion *er = new erosion();
          setCurrentImage(er->dilateGrayScale(getCurrentImage()));
          img->addImageToHistorique(getCurrentImage());
          delete er;
          break;
        }
        case 4: {
          erosion *er = new erosion();
          setCurrentImage(er->dilateColor(getCurrentImage()));
          img->addImageToHistorique(getCurrentImage());
          delete er;
          break;
        }
        default:
          break;
      }

      break;
    }
    case 6: {
        gimpsep::resize size;
        cv::Mat result = size.apply(getCurrentImage());
        if (!result.empty()) {
            setCurrentImage(result);
            img->addImageToHistorique(result);
        }
        break;
    }
    case 7: {
        brightness bright;
        cv::Mat result = bright.apply(getCurrentImage());
        if (!result.empty()) {
            setCurrentImage(result);
            img->addImageToHistorique(result);
        }
        break;
    }
    case 8: {

      panorama *pan = new panorama(getCurrentImage());

      setCurrentImage(pan->stitchImages());
      img->addImageToHistorique(getCurrentImage());
      delete pan;

      break;
    }
    case 9: {

      canny *can = new canny();
      setCurrentImage(can->detectEdges(getCurrentImage()));
      img->addImageToHistorique(getCurrentImage());
      delete can;

      break;
    }
    case 11: {
        try {
            objectDetection detector("objectDetection/yolov5s.onnx");
            cv::Mat result = detector.detectObjects(getCurrentImage());
            if (!result.empty()) {
                setCurrentImage(result);
                img->addImageToHistorique(result);
            }
        }
        catch (const cv::Exception& e) {
            std::cerr << "[ERREUR] Impossible de charger YOLOv5 :\n" << e.what() << std::endl;
        }
        break;
    }
    case 12:
      std::exit(0);
      break;
    default:
    chooseOperation();
  }

}
