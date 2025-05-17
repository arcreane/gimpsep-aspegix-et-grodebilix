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

    while (!stop) {
        std::string imgPath;
        std::string response = "";
        std::cout << "Load a new image for the panorama ? (y/n)" << std::endl;
        std::cin >> response;

        if (response == "y") {
            std::cout << "Enter the path of the image you wish to add : ";
            // TODO : penser à ajouter des checks de sécurité pour ce genre de trucs
            std::cin >> imgPath;
            images.push_back(cv::imread(imgPath));
        } else {
            stop = true;
        }
    }


    cv::Mat result;

    cv::Ptr<cv::Stitcher> stitcher = cv::Stitcher::create(cv::Stitcher::PANORAMA);
    cv::Stitcher::Status status = stitcher->stitch(images, result);

    if (status != cv::Stitcher::OK) {
        std::cout << "Can't stitch images, error code = " << int(status) << std::endl;
        return images[0];
    }

    std::string windowName = "Panorama";

    cv::imshow(windowName, result);

    cv::waitKey(0);

    cv::destroyWindow(windowName);

    return result;

}
