#include "panorama.h"

#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/stitching.hpp>

panorama::panorama(cv::Mat image) {
    images.push_back(image);
}

panorama::~panorama() {}

void panorama::addImage(cv::Mat image) {
    images.push_back(image);
}

cv::Mat panorama::stitchImages() {

    bool stop = false;

    // On charge toutes les images du panorama
    while (!stop) {
        std::string imgPath;
        std::string response = "";
        std::cout << "Load a new image for the panorama ? (y/n)" << std::endl;
        std::cin >> response;

        if (response == "y") {
            std::cout << "Enter the path of the image you wish to add : ";
            std::cin >> imgPath;
            images.push_back(cv::imread(imgPath));
        } else {
            stop = true;
        }
    }


    cv::Mat result;

    // Création du stitcher
    cv::Ptr<cv::Stitcher> stitcher = cv::Stitcher::create(cv::Stitcher::PANORAMA);
    cv::Stitcher::Status status = stitcher->stitch(images, result);

    // Si ça marche pas on renvoie la première image
    if (status != cv::Stitcher::OK) {
        std::cout << "Can't stitch images, error code = " << int(status) << std::endl;
        return images[0];
    }

    // Création et affichage de la fenêtre
    std::string windowName = "Panorama";

    cv::imshow(windowName, result);

    cv::waitKey(0);

    cv::destroyWindow(windowName);

    return result;

}
