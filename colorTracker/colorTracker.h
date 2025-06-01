#ifndef COLORTRACKER_H
#define COLORTRACKER_H

#include <opencv2/opencv.hpp>

class colorTracker {
public:
    colorTracker();
    ~colorTracker();

    // Start the tracking of the object's color from the webcam
    void trackColorFromWebcam();

private:
    // Selected HSV parameters
    int lowH, highH;
    int lowS, highS;
    int lowV, highV;

    // Initialise the trackbars to change the settings in real time
    void createControlWindow();

};

#endif // COLORTRACKER_H
