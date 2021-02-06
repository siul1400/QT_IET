#include "calibration.h"
// Voir calibration.h pour la documentation.


Calibration::Calibration()
{
    resetCalib();
}

void Calibration::setCalib(bool calib)
{
    isCalib = calib;
}

bool Calibration::getCalib()
{
    return isCalib;
}

int* Calibration::getXYR(QString varParam)
{
    if(varParam == "x"){
        return x;
    }else if (varParam == "y"){
        return y;
    }else if (varParam == "rmin"){
        return minRadius;
    }else if(varParam == "rmax"){
        return maxRadius;
    }

    return {};
}


bool Calibration::searchCircle(QString fileNameParam, int minRadiusParam, int maxRadiusParam, QString leftOrRightParam)
{

    if(!verifFileName(fileNameParam)){
        return false;
    }

    const char* filename = fileNameParam.toUtf8(); // On enregistre le lien local de l'image.
    Mat src = imread( samples::findFile( filename ), IMREAD_COLOR ); // Permet de charger l'image, indiqué dans filename.

    src = src.rowRange(src.size().height/2, src.size().height); // On coupe l'image en deux.
    int index = 0;
    if(leftOrRightParam == "Left"){
        src = src.colRange(0, src.size().width/2);
        index = 0;
    }else if (leftOrRightParam == "Right"){
        src = src.colRange(src.size().width/2, src.size().width);
        index = 1;
    }

    fileNamePath[index] = fileNameParam;

    Mat gray; // Créer une matrice nommé gray qui sera allouée dynamiquement.
    cvtColor(src, gray, COLOR_BGR2GRAY); // Permet de convertir l'image Blue Green Red en gray.

    medianBlur(gray, gray, 5); // Permet d'ajouter un filtre median (pixelise une image).

    vector<Vec3f> circles;
    Vec3i c;

    while(circles.size() != 1){
        HoughCircles(gray, circles, HOUGH_GRADIENT, 1, // Utilisation de la transformation de Hough, technique de reconnaissance de formes.
                        gray.rows/16,  // change this value to detect circles with different distances to each other
                        100, 30, minRadiusParam, maxRadiusParam // change the last two parameters
                        // (min_radius & max_radius) to detect larger circles
                        );
        minRadiusParam = minRadiusParam+precision;
        maxRadiusParam = maxRadiusParam+precision;
        qDebug() << "Nombre de cercle: " + QString::number(circles.size());

        if(maxRadiusParam >= src.size().width){

            minRadius[index] = 0;
            maxRadius[index] = 0;
            error = "Aucun cerlce de detécté";
            return false;
        }
    }
    c = circles[0];

    if(leftOrRightParam == "Left"){
        x[0] = c[0]-maxRadiusParam;
        y[0] = c[1]+src.size().height+maxRadiusParam;
    }else if (leftOrRightParam == "Right"){
        x[1] = c[0]+src.size().width+maxRadiusParam;
        y[1] = c[1]+src.size().height+maxRadiusParam;
    }

    if(maxRadiusParam < src.size().width){
        minRadius[index] = minRadiusParam+precision;
        maxRadius[index] = maxRadiusParam+precision;
        qDebug() << "minRadius[" + QString::number(index) + "]: " + QString::number(minRadius[index]) + ", maxRadius[" + QString::number(index) + "]: " + QString::number(maxRadius[index]);

        if(minRadius[0] != 0 && minRadius[1] != 0 && maxRadius[0] != 0 && maxRadius[1] != 0){
            if(minRadius[0] < minRadius[1]-precision){
                qDebug() << "Différence de -" + QString::number(precision) + " avec minRadius[0]";
                searchCircle(fileNamePath[0], minRadius[1]-precision, maxRadius[1]-precision, "Left");
            }else if(minRadius[0] > minRadius[1]+precision){
                qDebug() << "Différence de +" + QString::number(precision) + " avec minRadius[1]";
                searchCircle(fileNamePath[1], minRadius[0]-precision, maxRadius[0]-precision, "Right");

            }else{
                isCalib = true;
            }
        }
    }
    qDebug() << "minRadius: " + QString::number(minRadius[0])  + ", maxRadius: " + QString::number(maxRadius[1]) + ", x1: " + QString::number(x[0]) + ", y1: " + QString::number(y[0]) + ", x2: " + QString::number(x[1]) + ", y2: " + QString::number(y[1]);
    return true;
}

QString Calibration::getError()
{
    return error;
}

void Calibration::resetCalib()
{
    isCalib = false;
    for(int i = 0; i < 2; i++){
        fileNamePath[i] = "";
        x[i] = 0;
        y[i] = 0;
        minRadius[i] = 0;
        maxRadius[i] = 0;
    }
}

bool Calibration::verifFileName(QString fileNameParam)
{
    if(isCalib){
        error = "La calibration est déjà faite";
        return false;
    }

    if(fileNameParam == NULL){
        error = "Aucun fichier chargé";
        return false;
    }

    if(!QFile::exists(fileNameParam)){
        error = "Fichier non trouvé: " + fileNameParam;
        return false;
    }
    return true;
}
