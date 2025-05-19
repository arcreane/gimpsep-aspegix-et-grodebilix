#include "interface.h"
#include "erosion/erosion.h"
#include "canny/canny.h"
#include "brightness/Brightness.hpp"
#include "resize/Resize_Image.hpp"
#include "panorama/panorama.h"
#include "remove-background/backgroundRemover.h"
#include "face-detection/faceDetection.h"

interface::interface() {}

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
  std::cout << "11. Detect a face" << std::endl;
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
    case 6:
      // TODO : refacto resize pour l'intégrer là dedans
      break;
    case 7: {
      // TODO: refacto lighten / darken pour l'intégrer là dedans

      int choix;

      std::cout << "1. Rendre l'image plus lumineuse" << std::endl;
      std::cout << "2. Rendre l'image plus sombre" << std::endl;

      std::cin >> choix;

      switch (choix) {

        case 1: {
          break;
        }

        case 2: {
          break;
        }

        default:
          break;
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
    case 10: {

      int methode;

      std::cout << "1. Foreground extraction" << std::endl;
      std::cout << "2. Thresholding" << std::endl;
      std::cout << "3. Chromakey" << std::endl;
      std::cout << "What method would you like to use : ";
      std::cin >> methode;

      backgroundRemover *bgRemover = new backgroundRemover(getCurrentImage());

      switch (methode) {
        case 1: {
          setCurrentImage(bgRemover->foregroundExtraction());
          img->addImageToHistorique(getCurrentImage());
          break;
        }
        case 2: {
          setCurrentImage(bgRemover->thresholding());
          img->addImageToHistorique(getCurrentImage());
          break;
        }
        case 3: {
          setCurrentImage(bgRemover->chromaKey());
          img->addImageToHistorique(getCurrentImage());
          break;
        }
        default:
          break;
      }

      delete bgRemover;

      break;
    }
    case 11: {

      int choix;

      std::cout << "1. Detect a face from the loaded image"<< std::endl;
      std::cout << "2. Detect a face from the webcam"<< std::endl;

      std::cin >> choix;
      faceDetection *face_detection = new faceDetection();

      switch (choix) {
        case 1: {
          setCurrentImage(face_detection->detectFromImage(getCurrentImage()));
          img->addImageToHistorique(getCurrentImage());
          break;
        }
        case 2: {
          setCurrentImage(face_detection->detectFromWebcam());
          img->addImageToHistorique(getCurrentImage());
          break;
        }
        default:
          break;
      }

      delete face_detection;

      break;
    }
    case 12:
      std::exit(0);
      break;
    default:
    chooseOperation();
  }

}
