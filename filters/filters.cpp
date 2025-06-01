#include "filters.h"

using namespace cv;

filters::filters(){}
filters::~filters(){}

void  filters::applyFilters(cv::Mat &img, int filterIndex){

    // There are three filters : 1 cartoon, 2 vintage, 3 black and white

    switch (filterIndex)
{

    case 1:
    {
        // Color palette
        Mat img_color;
        pyrMeanShiftFiltering(img, img_color, 15, 20);

        // Edge detection
        Mat gray, edges;
        cvtColor(img, gray, COLOR_BGR2GRAY);
        medianBlur(gray, gray, 5);
        adaptiveThreshold(gray, edges, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 9, 2);

        // Combine color and edges
        cvtColor(edges, edges, COLOR_GRAY2BGR);
        bitwise_and(img_color, edges, img);

        // Brightness
        img.convertTo(img, -1, 1.1, 0);
        break;
    }

    case 2:
    {
        Mat channels[3];
        split(img, channels);

        // Colors
        channels[0] *= 0.8; // Blue
        channels[1] *= 0.9; // Green
        channels[2] *= 1.1; // Red
        merge(channels, 3, img);

        // Brightness
        img.convertTo(img, -1, 1.0, 10);

        // Blur
        GaussianBlur(img, img, Size(5, 5), 0);

        // Noise
        Mat noise(img.size(), CV_8UC3);
        randn(noise, 0, 60);
        add(img, noise, img);

        // Mask
        Mat mask = Mat::zeros(img.size(), CV_8UC3);
        ellipse(mask, Point(img.cols / 2, img.rows / 2), Size(img.cols / 1.8, img.rows / 1.8), 0, 0, 360, Scalar(255, 255, 255), -1);
        GaussianBlur(mask, mask, Size(71, 71), 0);
        img = img.mul(mask, 1.0 / 255.0);
        break;
    }

    case 3:
    {
        // Colors
        Mat gray;
        cvtColor(img, gray, COLOR_BGR2GRAY);
        cvtColor(gray, img, COLOR_GRAY2BGR);

        // Contrast
        img.convertTo(img, -1, 1.3, 10);

        // Blur
        GaussianBlur(img, img, Size(3, 3), 0.8);
        break;
    }

    default:
        break;
    }

}

cv::Mat filters::applyFiltersGUI(const cv::Mat& input, int filterIndex) {
    cv::Mat result = input.clone();
    applyFilters(result, filterIndex);
    return result;
}
