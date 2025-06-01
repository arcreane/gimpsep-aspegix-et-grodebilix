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

    // Load every image in the panorama
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

    // Creation of the sticher
    cv::Ptr<cv::Stitcher> stitcher = cv::Stitcher::create(cv::Stitcher::PANORAMA);
    cv::Stitcher::Status status = stitcher->stitch(images, result);

    // If it doesn't work return the original image
    if (status != cv::Stitcher::OK) {
        std::cout << "Can't stitch images, error code = " << int(status) << std::endl;
        return images[0];
    }

    // Creation and display of the window
    std::string windowName = "Panorama";

    cv::imshow(windowName, result);

    cv::waitKey(0);

    cv::destroyWindow(windowName);

    return result;

}

cv::Mat panorama::stitchImagesGUI() {
    cv::Mat result;

    if (images.size() < 2) {
        std::cerr << "Need at least two images to stitch." << std::endl;
        return result;
    }

    cv::Ptr<cv::Stitcher> stitcher = cv::Stitcher::create(cv::Stitcher::PANORAMA);
    cv::Stitcher::Status status = stitcher->stitch(images, result);

    if (status != cv::Stitcher::OK) {
        std::cerr << "Can't stitch images, error code = " << int(status) << std::endl;
        return cv::Mat();  // Return empty image if it fails
    }

    return result;
}
