#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "vector"
#include "calibration.h"

#include <QFileDialog>
#include <QPixmap>
#include <QDebug>
#include <QMessageBox>

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

private slots:
    void on_openFileButton_clicked();

    QPixmap traitement_image(QString filename);

    void on_validationButton_clicked();

    void on_bottomLeftButton_clicked();

    void on_bottomRightBottom_clicked();

    void on_testImageButton_clicked();

private:
    Ui::MainWindow *ui;
    Calibration calibration;
};
#endif // MAINWINDOW_H
