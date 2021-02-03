#ifndef CALIBRATION_H
#define CALIBRATION_H


#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <QString>
#include <QDebug>
#include <vector>
#include <QMessageBox>


using namespace cv;
using namespace std;

class Calibration
{
public:
    Calibration();


    int x1, x2, y1, y2, radius1, radius2 = 0;

    // Getter/Setter pour Calib
    void setCalib(bool calib);
    bool getCalib();

    int getXYR(QString var, int number);

    bool searchCircle(QString filename, int minRadiusParam, int maxRadiusParam, QString pos);

    QString getError();

private:
    int precision = 10;
    bool isCalib = false;
    int minRadius[2];
    int maxRadius[2];
    int x[2];
    int y[2];
    QString fileNamePath[3];
    QString error = "";
};

#endif // CALIBRATION_H
