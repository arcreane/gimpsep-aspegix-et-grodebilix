#include <iostream>
#include "interface.h"
#include "image/image.h"

int main() {

    std::cout << "SUPER APPLICATION COMME GIMP" << std::endl; // Penser à un meilleur titre

    std::string imagePath;

    std::cout << "Enter image path : ";
    std::cin >> imagePath;

    // TODO : Faut que je mette ça à jour mais je ferrais ça quand toutes les fonctionalités de base seront implémentées

    image * img = new image(imagePath);
    interface * inter = new interface(img->getImage());

    bool running = true;
    while (running) {
        inter->chooseOperation();
        cv::waitKey(1);
    }

    return 0;
}

