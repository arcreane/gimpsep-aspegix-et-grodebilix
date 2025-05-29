#include "MainWindow.h"

#include "ui_mainwindow.h"
#include "erosion/erosion.h"
#include <QFileDialog>
#include <QVBoxLayout>
#include <QPixmap>
#include <QSlider>
#include <QRadioButton>
#include <QGroupBox>
#include <QMessageBox>
#include <QColorDialog>
#include <QSpinBox>

#include "ParameterDialog.h"
#include "brightness/brightness.h"
#include "canny/canny.h"
#include "face-detection/faceDetection.h"
#include "panorama/panorama.h"
#include "remove-background/backgroundRemover.h"
#include "resize/resize.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), img(nullptr), iface(nullptr) {
    ui->setupUi(this);

    // File menu

    QMenu *fileMenu = menuBar()->addMenu("File");
    QAction *openAction = fileMenu->addAction("Open");
    QAction *saveAction = fileMenu->addAction("Save");
    QAction *exitAction = fileMenu->addAction("Exit");
    QAction *openWebcamAction = fileMenu->addAction("Open webcam");

    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(exitAction);
    fileMenu->addAction(openWebcamAction);

    // Edit menu

    QMenu *editMenu = menuBar()->addMenu("Edit");
    QMenu *erosionMenu = editMenu->addMenu("Erosion");
    QMenu *dilatationMenu = editMenu->addMenu("Dilation");
    QMenu *faceDetectionMenu = editMenu->addMenu("Face Detection");
    QMenu *backGroundRemoveMenu = editMenu->addMenu("BackGround Remove");

    // Erosion
    QAction *erodeGrayScaleAction = erosionMenu->addAction("Erode Gray Scale");
    QAction *erodeColorAction = erosionMenu->addAction("Erode Color");

    // Dilatation
    QAction *dilatationGrayScaleAction = dilatationMenu->addAction("Dilate Gray Scale");
    QAction *dilatationColorAction = dilatationMenu->addAction("Dilate Color");

    // Face detection
    QAction *faceDetectionFromImageAction = faceDetectionMenu->addAction("Face Detection From Image");
    QAction *faceDetectionFromWebcamAction = faceDetectionMenu->addAction("Face Detection From Webcam");

    // Background removal
    QAction *foregroundExtractionAction = backGroundRemoveMenu->addAction("Foreground Extraction");
    QAction *thresholdingAction = backGroundRemoveMenu->addAction("Foreground Thresholding");
    QAction *chromaKeyAction = backGroundRemoveMenu->addAction("Foreground Chroma Key");

    // Brightness
    QAction *brightenAction = editMenu->addAction("Brighten");

    // Resize
    // QAction *resizeAction = editMenu->addAction("Resize");

    // Canny
    QAction *cannyAction = editMenu->addAction("Canny edge detection");

    // Setup image display
    imageLabel = new QLabel(this);
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    imageLabel->setMinimumSize(1, 1);
    ui->verticalLayout->addWidget(imageLabel);


    // Erosion
    connect(erodeGrayScaleAction, &QAction::triggered, this, &MainWindow::onApplyErosionGrayScale);
    connect(erodeColorAction, &QAction::triggered, this, &MainWindow::onApplyErosionColor);

    // Dilatation
    connect(dilatationGrayScaleAction, &QAction::triggered, this, &MainWindow::onApplyDilatationGrayScale);
    connect(dilatationColorAction, &QAction::triggered, this, &MainWindow::onApplyDilatationColor);

    // Background removal
    connect(foregroundExtractionAction, &QAction::triggered, this, &MainWindow::onApplyForegroundExtraction);
    connect(thresholdingAction, &QAction::triggered, this, &MainWindow::onApplyThresholding);
    connect(chromaKeyAction, &QAction::triggered, this, &MainWindow::onApplyChromaKey);

    // Brightness
    connect(brightenAction, &QAction::triggered, this, &MainWindow::onApplyBrighten);

    // Resize
    // connect(resizeAction, &QAction::triggered, this, &MainWindow::onApplyResize);

    // Canny
    connect(cannyAction, &QAction::triggered, this, &MainWindow::onApplyCanny);

    // Face detection
    connect(faceDetectionFromImageAction, &QAction::triggered, this, &MainWindow::onDetectFaceFromImage);
    connect(faceDetectionFromWebcamAction, &QAction::triggered, this, &MainWindow::onDetectFaceFromWebcam);

    // File
    connect(openAction, &QAction::triggered, this, &MainWindow::onLoadImage);

    // Create the submenu for the brightness button
    QMenu *faceDetectionMenu2 = new QMenu(this);
    faceDetectionMenu2->addAction("Detect from image", this, &MainWindow::onApplyFaceDetection);
    faceDetectionMenu2->addAction("Detect from webcam", this, &MainWindow::onDetectFaceFromWebcam);

    // Assign the menu to your button
    ui->faceDetectionButton->setMenu(faceDetectionMenu);
    ui->faceDetectionButton->setPopupMode(QToolButton::InstantPopup);

    // Set the popup mode so clicking opens the menu immediately
    ui->brightnessButton->setPopupMode(QToolButton::InstantPopup);


    connect(ui->cannyButton, &QPushButton::clicked, this, &MainWindow::onApplyCanny);
    connect(ui->erosionButton, &QPushButton::clicked, this, &MainWindow::onApplyErosion);
    connect(ui->panoramaButton, &QPushButton::clicked, this, &MainWindow::onApplyPanorama);
    connect(ui->backgroundRemoverButton, &QPushButton::clicked, this, &MainWindow::onApplyBackgroundRemoval);
    connect(ui->brightnessButton, &QPushButton::clicked, this, &MainWindow::onApplyBrightness);
    connect(ui->resizeButton, &QPushButton::clicked, this, &MainWindow::onApplyResize);

    connect(ui->loadButton, &QPushButton::clicked, this, &MainWindow::onLoadImage);
    connect(ui->saveButton, &QPushButton::clicked, this, &MainWindow::onSaveImage);
}

MainWindow::~MainWindow() {
    delete iface;
    delete img;
    delete ui;
}

QImage matToQImage(cv::Mat& mat) {
    cv::Mat rgb;
    if (mat.channels() == 1) {
        cv::cvtColor(mat, rgb, cv::COLOR_GRAY2RGB);
    } else {
        cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
    }
    return QImage((const uchar *)rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888).copy();
}

void MainWindow::onLoadImage() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open Image", "", "Images (*.png *.jpg *.bmp)");
    if (!fileName.isEmpty()) {
        if (img) delete img;
        if (iface) delete iface;

        img = new image(fileName.toStdString());
        iface = new interface(img);

        showImage(img->getImage());
    }
}

void MainWindow::openParameterDialog() {
    QDialog dialog(this);
    dialog.setWindowTitle("My Mini Dialog");

    QVBoxLayout* layout = new QVBoxLayout(&dialog);
    QLabel* label = new QLabel("Hello, this is a mini dialog!", &dialog);
    QPushButton* okButton = new QPushButton("OK", &dialog);

    layout->addWidget(label);
    layout->addWidget(okButton);

    connect(okButton, &QPushButton::clicked, &dialog, &QDialog::accept);

    dialog.exec();
}


void MainWindow::onSaveImage() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save Image", "", "Images (*.png *.jpg *.bmp)");

    if (fileName.isEmpty()) {
        return; // User cancelled
    }

    std::string stdFileName = fileName.toStdString();

    if (!cv::imwrite(stdFileName, img->getImage())) {
        std::cout << "Failed to save image." << std::endl;
    } else {
        std::cout << "Image saved." << std::endl;
    }
}

// Erosion

void MainWindow::onApplyErosionGrayScale() {
    if (!iface) return;

    erosion er;
    cv::Mat newImg = er.erodeGrayScale(iface->getCurrentImage());
    iface->setCurrentImage(newImg);
    img->addImageToHistorique(newImg);

    showImage(newImg);
}

void MainWindow::onApplyErosionColor() {
    if (!iface) return;

    erosion er;
    cv::Mat newImg = er.erodeColor(iface->getCurrentImage());
    iface->setCurrentImage(newImg);
    img->addImageToHistorique(newImg);
    showImage(newImg);
}

void MainWindow::onApplyErosion() {
    if (!iface) return;

    erosion ero;
    cv::Mat result;

    QDialog dialog(this);
    dialog.setWindowTitle("Erosion / Dilation");
    dialog.resize(800, 600);

    QVBoxLayout* layout = new QVBoxLayout(&dialog);
    QLabel* imageLabel = new QLabel(&dialog);
    imageLabel->setFixedSize(700, 500);
    layout->addWidget(imageLabel);

    QSlider* sizeSlider = new QSlider(Qt::Horizontal, &dialog);
    sizeSlider->setRange(0, 10);
    sizeSlider->setValue(1);
    layout->addWidget(new QLabel("Kernel Size"));
    layout->addWidget(sizeSlider);

    // Mode selection with radio buttons
    QGroupBox* modeGroup = new QGroupBox("Mode", &dialog);
    QHBoxLayout* modeLayout = new QHBoxLayout();

    QRadioButton* grayErodeBtn = new QRadioButton("Gray Erode");
    QRadioButton* colorErodeBtn = new QRadioButton("Color Erode");
    QRadioButton* grayDilateBtn = new QRadioButton("Gray Dilate");
    QRadioButton* colorDilateBtn = new QRadioButton("Color Dilate");

    grayErodeBtn->setChecked(true);  // Default

    modeLayout->addWidget(grayErodeBtn);
    modeLayout->addWidget(colorErodeBtn);
    modeLayout->addWidget(grayDilateBtn);
    modeLayout->addWidget(colorDilateBtn);
    modeGroup->setLayout(modeLayout);
    layout->addWidget(modeGroup);

    QPushButton* applyButton = new QPushButton("Apply", &dialog);
    layout->addWidget(applyButton);

    auto getSelectedMode = [&]() -> int {
        if (grayErodeBtn->isChecked()) return 0;
        if (colorErodeBtn->isChecked()) return 1;
        if (grayDilateBtn->isChecked()) return 2;
        if (colorDilateBtn->isChecked()) return 3;
        return 0;  // fallback
    };

    auto updateImage = [&]() {
        int mode = getSelectedMode();
        result = ero.erodeGUI(iface->getCurrentImage(), sizeSlider->value(), mode);
        imageLabel->setPixmap(QPixmap::fromImage(matToQImage(result)).scaled(
            imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    };

    connect(sizeSlider, &QSlider::valueChanged, &dialog, updateImage);
    connect(grayErodeBtn, &QRadioButton::toggled, &dialog, updateImage);
    connect(colorErodeBtn, &QRadioButton::toggled, &dialog, updateImage);
    connect(grayDilateBtn, &QRadioButton::toggled, &dialog, updateImage);
    connect(colorDilateBtn, &QRadioButton::toggled, &dialog, updateImage);

    connect(applyButton, &QPushButton::clicked, [&]() {
        iface->setCurrentImage(result.clone());
        img->addImageToHistorique(result.clone());
        showImage(result.clone());
        dialog.accept();
    });

    updateImage();
    dialog.exec();
}


// Dilatation

void MainWindow::onApplyDilatationColor() {
    if (!iface) return;

    erosion er;
    cv::Mat newImg = er.dilateColor(iface->getCurrentImage());
    iface->setCurrentImage(newImg);
    img->addImageToHistorique(newImg);
    showImage(newImg);
}

void MainWindow::onApplyDilatationGrayScale() {

    if (!iface) return;

    erosion er;
    cv::Mat newImg = er.dilateGrayScale(iface->getCurrentImage());
    iface->setCurrentImage(newImg);
    img->addImageToHistorique(newImg);
    showImage(newImg);

}

// Remove background

void MainWindow::onApplyForegroundExtraction() {

    if (!iface) return;

    backgroundRemover bg(iface->getCurrentImage());
    cv::Mat newImg = bg.foregroundExtraction();
    iface->setCurrentImage(newImg);
    img->addImageToHistorique(newImg);
    showImage(newImg);

}

void MainWindow::onApplyBackgroundRemoval() {
    if (!iface) return;

    backgroundRemover remover(iface->getCurrentImage());
    cv::Mat result;

    QDialog dialog(this);
    dialog.setWindowTitle("Background Removal");
    dialog.resize(800, 650);

    QVBoxLayout* layout = new QVBoxLayout(&dialog);
    QLabel* imageLabel = new QLabel(&dialog);
    imageLabel->setFixedSize(700, 500);
    layout->addWidget(imageLabel);

    // Mode selection radio buttons
    QGroupBox* modeGroup = new QGroupBox("Mode", &dialog);
    QHBoxLayout* modeLayout = new QHBoxLayout();

    QRadioButton* fgExtractBtn = new QRadioButton("Foreground Extraction");
    QRadioButton* thresholdingBtn = new QRadioButton("Thresholding");
    QRadioButton* chromaKeyBtn = new QRadioButton("Chroma Key");
    chromaKeyBtn->setChecked(true);

    modeLayout->addWidget(fgExtractBtn);
    modeLayout->addWidget(thresholdingBtn);
    modeLayout->addWidget(chromaKeyBtn);
    modeGroup->setLayout(modeLayout);
    layout->addWidget(modeGroup);

    // Threshold slider (for Thresholding mode)
    QLabel* thresholdLabel = new QLabel("Threshold Value");
    QSlider* thresholdSlider = new QSlider(Qt::Horizontal, &dialog);
    thresholdSlider->setRange(0, 255);
    thresholdSlider->setValue(100);
    layout->addWidget(thresholdLabel);
    layout->addWidget(thresholdSlider);

    // HSV sliders (for Chroma Key mode)
    QLabel* hueLabel = new QLabel("Hue Tolerance");
    QSlider* hueSlider = new QSlider(Qt::Horizontal, &dialog);
    hueSlider->setRange(0, 30);
    hueSlider->setValue(10);

    QLabel* satLabel = new QLabel("Saturation Tolerance");
    QSlider* satSlider = new QSlider(Qt::Horizontal, &dialog);
    satSlider->setRange(0, 100);
    satSlider->setValue(60);

    QLabel* valLabel = new QLabel("Value Tolerance");
    QSlider* valSlider = new QSlider(Qt::Horizontal, &dialog);
    valSlider->setRange(0, 100);
    valSlider->setValue(60);

    layout->addWidget(hueLabel);
    layout->addWidget(hueSlider);
    layout->addWidget(satLabel);
    layout->addWidget(satSlider);
    layout->addWidget(valLabel);
    layout->addWidget(valSlider);

    QPushButton* colorButton = new QPushButton("Select Color (for Chroma Key)", &dialog);
    layout->addWidget(colorButton);

    QPushButton* applyButton = new QPushButton("Apply", &dialog);
    layout->addWidget(applyButton);

    // Initialize selectedColor from center pixel of the current image
    cv::Mat currentImage = iface->getCurrentImage();
    cv::Vec3b bgr = currentImage.at<cv::Vec3b>(currentImage.rows / 2, currentImage.cols / 2);
    QColor selectedColor(bgr[2], bgr[1], bgr[0]);  // Convert BGR to RGB for QColor

    // Update UI visibility depending on mode
    auto updateUIVisibility = [&]() {
        bool isThresholding = thresholdingBtn->isChecked();
        bool isChromaKey = chromaKeyBtn->isChecked();

        thresholdLabel->setVisible(isThresholding);
        thresholdSlider->setVisible(isThresholding);

        hueLabel->setVisible(isChromaKey);
        hueSlider->setVisible(isChromaKey);
        satLabel->setVisible(isChromaKey);
        satSlider->setVisible(isChromaKey);
        valLabel->setVisible(isChromaKey);
        valSlider->setVisible(isChromaKey);
        colorButton->setVisible(isChromaKey);
    };

    auto updateImage = [&]() {
        updateUIVisibility();

        if (fgExtractBtn->isChecked()) {
            result = remover.foregroundExtraction();
        } else if (thresholdingBtn->isChecked()) {
            result = remover.thresholdingGUI(thresholdSlider->value());
        } else if (chromaKeyBtn->isChecked()) {
            result = remover.chromaKeyGUI(selectedColor, hueSlider->value(), satSlider->value(), valSlider->value());
        }

        imageLabel->setPixmap(QPixmap::fromImage(matToQImage(result)).scaled(
            imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    };

    // Connect signals to update preview
    connect(thresholdSlider, &QSlider::valueChanged, &dialog, updateImage);
    connect(hueSlider, &QSlider::valueChanged, &dialog, updateImage);
    connect(satSlider, &QSlider::valueChanged, &dialog, updateImage);
    connect(valSlider, &QSlider::valueChanged, &dialog, updateImage);

    connect(fgExtractBtn, &QRadioButton::toggled, &dialog, updateImage);
    connect(thresholdingBtn, &QRadioButton::toggled, &dialog, updateImage);
    connect(chromaKeyBtn, &QRadioButton::toggled, &dialog, updateImage);

    // Color picker button updates selectedColor and refreshes preview
    connect(colorButton, &QPushButton::clicked, [&]() {
        QColor color = QColorDialog::getColor(selectedColor, &dialog, "Pick a Chroma Key Color");
        if (color.isValid()) {
            selectedColor = color;
            updateImage();
        }
    });

    connect(applyButton, &QPushButton::clicked, [&]() {
        iface->setCurrentImage(result.clone());
        img->addImageToHistorique(result.clone());
        showImage(result.clone());
        dialog.accept();
    });

    updateImage();
    dialog.exec();
}


void MainWindow::onApplyThresholding() {
    if (!iface) return;

    backgroundRemover bg(iface->getCurrentImage());
    cv::Mat newImg = bg.thresholding();
    iface->setCurrentImage(newImg);
    img->addImageToHistorique(newImg);
    showImage(newImg);
}

void MainWindow::onApplyChromaKey() {
    if (!iface) return;

    backgroundRemover bg(iface->getCurrentImage());
    cv::Mat newImg = bg.chromaKey();
    iface->setCurrentImage(newImg);
    img->addImageToHistorique(newImg);
    showImage(newImg);
}

// Resize

// void MainWindow::onApplyResize() {
//     if (!iface) return;
//
//     resize size;
//     cv::Mat newImg = size.apply(iface->getCurrentImage());
//     iface->setCurrentImage(newImg);
//     img->addImageToHistorique(newImg);
//     showImage(newImg);
//
// }

void MainWindow::onApplyResize() {
    if (!iface) return;

    gimpsep::resize resizer;
    cv::Mat inputImage = iface->getCurrentImage();
    cv::Mat result;

    QDialog dialog(this);
    dialog.setWindowTitle("Resize Image");
    dialog.resize(800, 600);

    QVBoxLayout* layout = new QVBoxLayout(&dialog);
    QLabel* imageLabel = new QLabel(&dialog);
    imageLabel->setFixedSize(700, 500);
    layout->addWidget(imageLabel);

    QGroupBox* modeGroup = new QGroupBox("Resize Mode", &dialog);
    QHBoxLayout* modeLayout = new QHBoxLayout();

    QRadioButton* factorBtn = new QRadioButton("By Scale Factor");
    QRadioButton* dimensionBtn = new QRadioButton("By Width & Height");
    factorBtn->setChecked(true);

    modeLayout->addWidget(factorBtn);
    modeLayout->addWidget(dimensionBtn);
    modeGroup->setLayout(modeLayout);
    layout->addWidget(modeGroup);

    QSlider* scaleSlider = new QSlider(Qt::Horizontal, &dialog);
    scaleSlider->setRange(1, 300);  // 1% to 300%
    scaleSlider->setValue(100);
    layout->addWidget(new QLabel("Scale Factor (%)"));
    layout->addWidget(scaleSlider);

    QSpinBox* widthSpin = new QSpinBox(&dialog);
    widthSpin->setRange(1, inputImage.cols * 5);
    widthSpin->setValue(inputImage.cols);
    layout->addWidget(new QLabel("Width (pixels)"));
    layout->addWidget(widthSpin);

    QSpinBox* heightSpin = new QSpinBox(&dialog);
    heightSpin->setRange(1, inputImage.rows * 5);
    heightSpin->setValue(inputImage.rows);
    layout->addWidget(new QLabel("Height (pixels)"));
    layout->addWidget(heightSpin);

    // Initially disable width/height because factor mode is checked
    widthSpin->setEnabled(false);
    heightSpin->setEnabled(false);

    QPushButton* applyButton = new QPushButton("Apply", &dialog);
    layout->addWidget(applyButton);

    auto updateImage = [&]() {
        bool byFactor = factorBtn->isChecked();
        scaleSlider->setEnabled(byFactor);
        widthSpin->setEnabled(!byFactor);
        heightSpin->setEnabled(!byFactor);

        double scaleFactor = scaleSlider->value() / 100.0;
        int width = widthSpin->value();
        int height = heightSpin->value();

        result = resizer.resizeAndShowGUI(inputImage, byFactor, scaleFactor, width, height);

        imageLabel->setPixmap(QPixmap::fromImage(matToQImage(result)).scaled(
            imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    };

    connect(factorBtn, &QRadioButton::toggled, &dialog, updateImage);
    connect(dimensionBtn, &QRadioButton::toggled, &dialog, updateImage);

    connect(scaleSlider, &QSlider::valueChanged, &dialog, updateImage);
    connect(widthSpin, QOverload<int>::of(&QSpinBox::valueChanged), &dialog, updateImage);
    connect(heightSpin, QOverload<int>::of(&QSpinBox::valueChanged), &dialog, updateImage);

    connect(applyButton, &QPushButton::clicked, [&]() {
        iface->setCurrentImage(result.clone());
        img->addImageToHistorique(result.clone());
        showImage(result.clone());
        dialog.accept();
    });

    updateImage();
    dialog.exec();
}


// Brighten

void MainWindow::onApplyBrighten() {
    if (!iface) return;

    brightness br;
    cv::Mat newImg = br.apply(iface->getCurrentImage());
    iface->setCurrentImage(newImg);
    img->addImageToHistorique(newImg);
    showImage(newImg);

}

void MainWindow::onApplyBrightness() {
    if (!iface) return;

    brightness bright;
    cv::Mat inputImage = iface->getCurrentImage();
    cv::Mat result;

    QDialog dialog(this);
    dialog.setWindowTitle("Brightness Adjustment");
    dialog.resize(800, 600);

    QVBoxLayout* layout = new QVBoxLayout(&dialog);
    QLabel* imageLabel = new QLabel(&dialog);
    imageLabel->setFixedSize(700, 500);
    layout->addWidget(imageLabel);

    QGroupBox* modeGroup = new QGroupBox("Mode", &dialog);
    QHBoxLayout* modeLayout = new QHBoxLayout();

    QRadioButton* brighterBtn = new QRadioButton("Make Brighter");
    QRadioButton* darkerBtn = new QRadioButton("Make Darker");
    brighterBtn->setChecked(true);

    modeLayout->addWidget(brighterBtn);
    modeLayout->addWidget(darkerBtn);
    modeGroup->setLayout(modeLayout);
    layout->addWidget(modeGroup);

    QSlider* valueSlider = new QSlider(Qt::Horizontal, &dialog);
    valueSlider->setRange(0, 100);
    valueSlider->setValue(50);
    layout->addWidget(new QLabel("Brightness Value"));
    layout->addWidget(valueSlider);

    QPushButton* applyButton = new QPushButton("Apply", &dialog);
    layout->addWidget(applyButton);

    auto updateImage = [&]() {
        int mode = brighterBtn->isChecked() ? 1 : 2;
        int value = valueSlider->value();

        result = bright.changeAndShowGUI(inputImage, mode, value);
        imageLabel->setPixmap(QPixmap::fromImage(matToQImage(result)).scaled(
            imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    };

    connect(valueSlider, &QSlider::valueChanged, &dialog, updateImage);
    connect(brighterBtn, &QRadioButton::toggled, &dialog, updateImage);
    connect(darkerBtn, &QRadioButton::toggled, &dialog, updateImage);

    connect(applyButton, &QPushButton::clicked, [&]() {
        iface->setCurrentImage(result.clone());
        img->addImageToHistorique(result.clone());
        showImage(result.clone());
        dialog.accept();
    });

    updateImage();
    dialog.exec();
}


// Canny edge detection

void MainWindow::onApplyCanny() {
    if (!iface) return;

    canny can;
    // cv::Mat newImg = can.detectEdges(iface->getCurrentImage());
    // iface->setCurrentImage(newImg);
    // img->addImageToHistorique(newImg);
    // showImage(newImg);

    QDialog dialog(this);
    dialog.setWindowTitle("Canny edge detection");
    dialog.resize(800, 600);

    QVBoxLayout* layout = new QVBoxLayout(&dialog);
    QLabel* imageLabel = new QLabel(&dialog);
    layout->addWidget(imageLabel);

    QPushButton* applyButton = new QPushButton("Apply", &dialog);
    layout->addWidget(applyButton);

    QSlider* lowSlider = new QSlider(Qt::Horizontal, &dialog);
    lowSlider->setRange(0, 300);
    lowSlider->setValue(50);
    layout->addWidget(lowSlider);

    QSlider* highSlider = new QSlider(Qt::Horizontal, &dialog);
    highSlider->setRange(0, 300);
    highSlider->setValue(150);
    layout->addWidget(highSlider);

    cv::Mat result;
    auto updateImage = [&]() {
        result = can.detectEdgesGUI(iface->getCurrentImage(), lowSlider->value(), highSlider->value());

        QImage qimg = matToQImage(result);

        QSize displaySize = imageLabel->size();
        if (displaySize.width() == 0 || displaySize.height() == 0) {
            displaySize = dialog.size();
        }

        imageLabel->setPixmap(QPixmap::fromImage(qimg).scaled(displaySize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    };

    connect(lowSlider, &QSlider::valueChanged, &dialog, updateImage);
    connect(highSlider, &QSlider::valueChanged, &dialog, updateImage);

    connect(applyButton, &QPushButton::clicked, [&]() {

        iface->setCurrentImage(result.clone());
        img->addImageToHistorique(result.clone());
        showImage(result.clone());

        dialog.accept();
    });

    // Force the image to resize before we modify a value
    dialog.show();
    QCoreApplication::processEvents();

    updateImage();

    dialog.exec();

}

// Detect Face

void MainWindow::onDetectFaceFromImage() {
    if (!iface) return;

    faceDetection fd;
    cv::Mat newImg = fd.detectFromImage(iface->getCurrentImage());
    iface->setCurrentImage(newImg);
    img->addImageToHistorique(newImg);

    showImage(newImg);

}

void MainWindow::onDetectFaceFromWebcam() {

    if (!iface) return;

    faceDetection fd;
    cv::Mat newImg = fd.detectFromWebcam();
    iface->setCurrentImage(newImg);
    img->addImageToHistorique(newImg);

    showImage(newImg);

}

void MainWindow::onApplyFaceDetection() {
    faceDetection fd;
    cv::Mat result = fd.detectFacesGUI(iface->getCurrentImage());
    iface->setCurrentImage(result.clone());
    img->addImageToHistorique(result.clone());
    showImage(result.clone());
}

// Panorama

void MainWindow::onApplyPanorama() {
    if (!iface) return;

    panorama pano(iface->getCurrentImage());

    // Add the current image as the starting point
    //pano.addImage(iface->getCurrentImage());

    QDialog dialog(this);
    dialog.setWindowTitle("Panorama Stitching");
    dialog.resize(800, 600);

    QVBoxLayout* layout = new QVBoxLayout(&dialog);
    QLabel* imageLabel = new QLabel(&dialog);
    imageLabel->setFixedSize(700, 500);
    layout->addWidget(imageLabel);

    QPushButton* addImageButton = new QPushButton("Add Image", &dialog);
    layout->addWidget(addImageButton);

    QPushButton* stitchButton = new QPushButton("Stitch Panorama", &dialog);
    layout->addWidget(stitchButton);

    QPushButton* applyButton = new QPushButton("Apply", &dialog);
    layout->addWidget(applyButton);

    cv::Mat result;

    auto updateImage = [&]() {
        if (result.empty()) return;

        cv::Mat rgb;
        cv::cvtColor(result, rgb, cv::COLOR_BGR2RGB);
        QImage qimg(rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888);
        imageLabel->setPixmap(QPixmap::fromImage(qimg).scaled(
            imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    };

    connect(addImageButton, &QPushButton::clicked, [&]() {
        QString fileName = QFileDialog::getOpenFileName(&dialog, "Select Image", "", "Images (*.png *.jpg *.jpeg *.bmp)");
        if (!fileName.isEmpty()) {
            cv::Mat img = cv::imread(fileName.toStdString());
            if (!img.empty()) {
                pano.addImage(img);
                QMessageBox::information(&dialog, "Image Added", "Image added to panorama stack.");
            } else {
                QMessageBox::warning(&dialog, "Error", "Failed to load image.");
            }
        }
    });

    connect(stitchButton, &QPushButton::clicked, [&]() {
        result = pano.stitchImagesGUI();
        if (result.empty()) {
            QMessageBox::warning(&dialog, "Error", "Failed to stitch images.");
        } else {
            updateImage();
        }
    });

    connect(applyButton, &QPushButton::clicked, [&]() {
        if (result.empty()) {
            QMessageBox::warning(&dialog, "Warning", "No stitched result to apply.");
            return;
        }
        iface->setCurrentImage(result.clone());
        img->addImageToHistorique(result.clone());
        showImage(result.clone());
        dialog.accept();
    });

    dialog.exec();
}


void MainWindow::showImage(const cv::Mat& mat) {
    imageLabel->setPixmap(QPixmap::fromImage(matToQImage(mat)).scaled(
    imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

}

QImage MainWindow::matToQImage(const cv::Mat& mat) {
    if (mat.type() == CV_8UC3) {
        return QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888).rgbSwapped();
    } else if (mat.type() == CV_8UC1) {
        return QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8);
    }
    return QImage();
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    if (iface)
        showImage(iface->getCurrentImage());
}
