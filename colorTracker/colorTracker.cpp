#include "colorTracker.h"
#include <iostream>
#include <opencv2/opencv.hpp>

static int lowH = 0, highH = 10;
static int lowS = 100, highS = 255;
static int lowV = 100, highV = 255;
static cv::Mat currentHSVFrame;  // access HSV image in mouse callback

// Tolerance for color selection
const int toleranceH = 10;
const int toleranceS = 50;
const int toleranceV = 50;

void onMouse(int event, int x, int y, int, void*) {
    if (event != cv::EVENT_LBUTTONDOWN)
        return;

    if (currentHSVFrame.empty())
        return;

    // Retrieve the HSV color of the clicked pixel
    cv::Vec3b hsvPixel = currentHSVFrame.at<cv::Vec3b>(y, x);

    int H = hsvPixel[0];
    int S = hsvPixel[1];
    int V = hsvPixel[2];

    std::cout << "Couleur cliqu�e HSV: (" << H << ", " << S << ", " << V << ")\n";

    // Adjust thresholds with tolerance and bounds
    lowH = std::max(H - toleranceH, 0);
    highH = std::min(H + toleranceH, 179);

    lowS = std::max(S - toleranceS, 0);
    highS = std::min(S + toleranceS, 255);

    lowV = std::max(V - toleranceV, 0);
    highV = std::min(V + toleranceV, 255);

    std::cout << "Nouvelle plage HSV : "
        << "LowH=" << lowH << ", HighH=" << highH << ", "
        << "LowS=" << lowS << ", HighS=" << highS << ", "
        << "LowV=" << lowV << ", HighV=" << highV << std::endl;

    // Update trackbars
    cv::setTrackbarPos("LowH", "Control", lowH);
    cv::setTrackbarPos("HighH", "Control", highH);
    cv::setTrackbarPos("LowS", "Control", lowS);
    cv::setTrackbarPos("HighS", "Control", highS);
    cv::setTrackbarPos("LowV", "Control", lowV);
    cv::setTrackbarPos("HighV", "Control", highV);
}

colorTracker::colorTracker() {
    lowH = 0; highH = 10;
    lowS = 100; highS = 255;
    lowV = 100; highV = 255;
}

colorTracker::~colorTracker() {}

void colorTracker::createControlWindow() {
    cv::namedWindow("Control", cv::WINDOW_AUTOSIZE);
    cv::createTrackbar("LowH", "Control", &lowH, 179);
    cv::createTrackbar("HighH", "Control", &highH, 179);
    cv::createTrackbar("LowS", "Control", &lowS, 255);
    cv::createTrackbar("HighS", "Control", &highS, 255);
    cv::createTrackbar("LowV", "Control", &lowV, 255);
    cv::createTrackbar("HighV", "Control", &highV, 255);
}

void colorTracker::trackColorFromWebcam() {
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "Erreur : Impossible d'acc�der � la webcam." << std::endl;
        return;
    }

    createControlWindow();
    cv::Mat frame, hsv, mask;

    // Bind window to mouse callback
    cv::namedWindow("Tracking", cv::WINDOW_AUTOSIZE);
    cv::setMouseCallback("Tracking", onMouse, nullptr);

    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        // Conversion to HSV
        cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);
        currentHSVFrame = hsv; // Update HSV image for callback

        // Filtering the selected color
        cv::inRange(hsv,
            cv::Scalar(lowH, lowS, lowV),
            cv::Scalar(highH, highS, highV),
            mask);

        // Find the largest contour
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        if (!contours.empty()) {
            auto maxContour = *std::max_element(contours.begin(), contours.end(),
                [](const std::vector<cv::Point>& c1, const std::vector<cv::Point>& c2) {
                    return cv::contourArea(c1) < cv::contourArea(c2);
                });

            cv::Moments m = cv::moments(maxContour);
            if (m.m00 > 0) {
                int cx = int(m.m10 / m.m00);
                int cy = int(m.m01 / m.m00);
                cv::circle(frame, cv::Point(cx, cy), 10, cv::Scalar(0, 255, 0), -1);
            }
        }

        // Display
        cv::imshow("Tracking", frame);
        cv::imshow("Mask", mask);

        char c = (char)cv::waitKey(30);
        if (c == 27) break; // ESC
    }

    cap.release();
    cv::destroyAllWindows();
}

