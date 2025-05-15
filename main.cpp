#include <iostream>
#include "interface.h"
#include "image/image.h"

int main() {

    std::cout << "SUPER APPLICATION COMME GIMP" << std::endl; // Penser à un meilleur titre

    std::string imagePath;

    std::cout << "Enter image path : ";
    std::cin >> imagePath;

    image * img = new image(imagePath);
    interface * inter = new interface(img);

    bool running = true;
    while (running) {
        inter->chooseOperation();
        cv::waitKey(1);
    }

    return 0;
}

