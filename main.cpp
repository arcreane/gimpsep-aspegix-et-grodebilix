#include <iostream>
#include "interface.h"

int main() {

    std::cout << "SUPER APPLICATION COMME GIMP" << std::endl; // Penser à un meilleur titre

    interface * inter = new interface();

    bool running = true;
    while (running) {
        inter->chooseOperation();
        cv::waitKey(1);
    }

    return 0;
}

