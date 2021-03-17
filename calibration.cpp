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

int* Calibration::getRayon(QString varParam)
{
    if (varParam == "rmin"){
        return minRadius;
    }else if(varParam == "rmax"){
        return maxRadius;
    }

    return {};
}

std::vector<int>* Calibration::getLimits(){
    return limits;
}


bool Calibration::searchCircle(QString fileNameParam, int minRadiusParam, int maxRadiusParam, QString leftOrRightParam)
{

    if(!checkFileName(fileNameParam)){
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
        // Pour le ballon en bas à gauche.
        limits[8] = {c[0]-maxRadiusParam, c[1]+src.size().height+maxRadiusParam};
    }else if (leftOrRightParam == "Right"){
        // Pour le ballon en bas à droite.
        limits[11] = {c[0]+src.size().width+maxRadiusParam, c[1]+src.size().height+maxRadiusParam};
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

                //////// Création de coin supérieur gauche et droite du but ////////
                // En haut à gauche.
                limits[0] = {limits[8][0], HEIGHT_BUT-maxRadius[0]};

                // En haut à droite.
                limits[3] = {limits[11][0], HEIGHT_BUT-maxRadius[1]};
                ////////////////////////////////////////////////////////////////////

                //////// Création des lines vertical ////////
                limits[1] = {limits[3][0]/3, limits[0][1]};
                limits[2] = {limits[3][0]/3*2, limits[3][1]};
                limits[9] = {limits[11][0]/3, limits[8][1]};
                limits[10] = {limits[11][0]/3*2, limits[11][1]};
                /////////////////////////////////////////////

                //////// Création des lines horizontal ////////
                limits[4] = {limits[0][0], limits[8][1]/3};
                limits[6] = {limits[8][0], limits[8][1]/3*2};
                limits[5] = {limits[3][0], limits[11][1]/3};
                limits[7] = {limits[11][0], limits[11][1]/3*2};
                ///////////////////////////////////////////////



                isCalib = true;
            }
        }
    }
    qDebug() << "minRadius: " + QString::number(minRadius[0])  + ", maxRadius: " + QString::number(maxRadius[1]) + ". En bas à gauche: x:" + QString::number(limits[8][0]) + ", y: " + QString::number(limits[8][1]) + ". En bas à droite: x: " + QString::number(limits[11][0]) + ", y: " + QString::number(limits[11][1]);


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
        minRadius[i] = 0;
        maxRadius[i] = 0;
    }
    for(int i = 0; i < 12; i++){
        limits[i] = {0,0};
    }
}

bool Calibration::checkFileName(QString fileNameParam)
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
