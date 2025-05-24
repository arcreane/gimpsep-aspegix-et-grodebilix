#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "erosion/erosion.h"
#include <QFileDialog>
#include <QVBoxLayout>
#include <QPixmap>

#include "brightness/brightness.h"
#include "canny/canny.h"
#include "face-detection/faceDetection.h"
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

    connect(ui->loadButton, &QPushButton::clicked, this, &MainWindow::onLoadImage);
    connect(ui->saveButton, &QPushButton::clicked, this, &MainWindow::onSaveImage);
}

MainWindow::~MainWindow() {
    delete iface;
    delete img;
    delete ui;
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

// Brighten

void MainWindow::onApplyBrighten() {
    if (!iface) return;

    brightness br;
    cv::Mat newImg = br.apply(iface->getCurrentImage());
    iface->setCurrentImage(newImg);
    img->addImageToHistorique(newImg);
    showImage(newImg);

}

void MainWindow::onApplyCanny() {
    if (!iface) return;

    canny can;
    cv::Mat newImg = can.detectEdges(iface->getCurrentImage());
    iface->setCurrentImage(newImg);
    img->addImageToHistorique(newImg);
    showImage(newImg);
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

void MainWindow::showImage(const cv::Mat& mat) {
    imageLabel->setPixmap(QPixmap::fromImage(matToQImage(mat)).scaled(imageLabel->size(), Qt::KeepAspectRatio));
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
