#include <iostream>
#include "interface.h"
#include "image/image.h"

#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[]) {

    bool gui = true;

    if (gui) {
        QApplication app(argc, argv);
        MainWindow window;
        window.show();
        return app.exec();
    } else {
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
    }



    return 0;
}

