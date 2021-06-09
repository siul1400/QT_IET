#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "vector"
#include "calibration.h"
#include "circleDetection.h"

#include <QFileDialog>
#include <QPixmap>
#include <QDebug>
#include <QMessageBox>


#include <QDate>
#include <QTime>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

    void setOpenFileButton(bool calib);

    void setActiveCalibMenu();

    void setDisableCalibMenu();

    void setProgressBar(int valueParam);

private slots:
    void on_openFileButton_clicked();

    void on_bottomLeftButton_clicked();

    void on_bottomRightBottom_clicked();

    void on_testImageButton_clicked();

    void on_validNoButton_clicked();

    void on_validYesButton_clicked();

    bool treatmentDir();


    void on_openDbButton_clicked();

private:
    Ui::MainWindow *ui;
    Calibration calibration;
    circleDetection* circle;
    QString pathDirImage;
    QString pathDbFile;
};
#endif // MAINWINDOW_H
