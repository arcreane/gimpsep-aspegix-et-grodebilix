#pragma once

#include <QMainWindow>
#include <QImage>
#include <QLabel>
#include "interface.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    private slots:
        void onLoadImage();

    void onSaveImage();

    void onApplyErosionGrayScale();

    void onApplyErosionColor();

    void onApplyDilatationColor();

    void onApplyDilatationGrayScale();

    void onApplyForegroundExtraction();

    void onApplyThresholding();

    void onApplyChromaKey();

    // void onApplyResize();

    void onApplyBrighten();

    void onApplyCanny();

    void onDetectFaceFromImage();

    void onDetectFaceFromWebcam();

private:
    Ui::MainWindow *ui;
    QLabel* imageLabel;
    image* img;
    interface* iface;


    void showImage(const cv::Mat& mat);
    QImage matToQImage(const cv::Mat& mat);
    void resizeEvent(QResizeEvent* event) override;
};
