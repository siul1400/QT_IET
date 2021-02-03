#ifndef CALIBRATION_H
#define CALIBRATION_H


#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <QString>
#include <QDebug>
#include <vector>
#include <QFile>


using namespace cv;
using namespace std;

class Calibration
{
public:
    Calibration();

    // Getter/Setter pour Calib
    void setCalib(bool calib);
    bool getCalib();

    int getXYR(QString var, int number);

    bool searchCircle(QString fileNameParam, int minRadiusParam, int maxRadiusParam, QString pos);

    QString getError();

private:
    int precision = 10;
    bool isCalib = false;
    int minRadius[2];
    int maxRadius[2];
    int x[2];
    int y[2];
    QString fileNamePath[2];
    QString error = "";
};

#endif // CALIBRATION_H
