#include "calibration.h"
// Voir calibration.h pour la documentation.

//////// Initialise l'objet ////////
Calibration::Calibration()
{
    resetCalib();
}

//////// Permet de mettre une valeur pour isCalib ////////
void Calibration::setCalib(bool calib)
{
    isCalib = calib;
}

//////// Permet de retourner la valeur de isCalib ////////
bool Calibration::getCalib()
{
    return isCalib;
}

//////// Permet de retourner la valeur pour minRadius ou maxRadius ////////
int* Calibration::getRayon(QString varParam)
{
    if (varParam == "rmin"){
        return minRadius;
    }else if(varParam == "rmax"){
        return maxRadius;
    }

    return {};
}

//////// Permet de retourner la valeur de limits ////////
std::vector<int>* Calibration::getLimits(){
    return goalLimits;
}


bool Calibration::searchCircle(QString fileNameParam, int minRadiusParam, QString leftOrRightParam)
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
                        gray.rows/16,  // modifiez cette valeur pour détecter les cercles avec des distances différentes les uns des autres
                        100, 30, minRadiusParam, minRadiusParam+precision // Changer les deux derniers paramètres
                        // (minRadiusParam & minRadiusParam+precision) pour changer l'intervalle de détection
                        );
        qDebug() << "Nombre de cercle: " + QString::number(circles.size());
        minRadiusParam = minRadiusParam+precision;

        if(minRadiusParam+precision >= src.size().width){

            minRadius[index] = 0;
            maxRadius[index] = 0;
            error = "Aucun cerlce de detécté";
            return false;
        }
    }
    minRadiusParam = minRadiusParam-precision;
    c = circles[0];


    if(leftOrRightParam == "Left"){
        // Pour le ballon en bas à gauche.
        goalLimits[8] = {c[0]-minRadiusParam+precision, c[1]+src.size().height+minRadiusParam+precision};
    }else if (leftOrRightParam == "Right"){
        // Pour le ballon en bas à droite.
        goalLimits[11] = {c[0]+src.size().width+minRadiusParam+precision, c[1]+src.size().height+minRadiusParam+precision};
    }

    if(minRadiusParam+precision < src.size().width){
        minRadius[index] = minRadiusParam;
        maxRadius[index] = minRadiusParam+precision;
        qDebug() << "minRadius[" + QString::number(index) + "]: " + QString::number(minRadius[index]) + ", maxRadius[" + QString::number(index) + "]: " + QString::number(maxRadius[index]);

        if(minRadius[0] != 0 && minRadius[1] != 0 && maxRadius[0] != 0 && maxRadius[1] != 0){
            if(minRadius[0] < minRadius[1]-precision){
                qDebug() << "Différence de -" + QString::number(precision) + " avec minRadius[0]";
                searchCircle(fileNamePath[0], minRadius[1]-precision, "Left");
            }else if(minRadius[0] > minRadius[1]+precision){
                qDebug() << "Différence de +" + QString::number(precision) + " avec minRadius[1]";
                searchCircle(fileNamePath[1], minRadius[0]-precision, "Right");

            }else{

                //////// Création de coin supérieur gauche et droite du but ////////
                // En haut à gauche.
                goalLimits[0] = {goalLimits[8][0], HEIGHT_GOAL-maxRadius[0]};

                // En haut à droite.
                goalLimits[3] = {goalLimits[11][0], HEIGHT_GOAL-maxRadius[1]};
                ////////////////////////////////////////////////////////////////////

                //////// Création des lines vertical ////////
                goalLimits[1] = {goalLimits[3][0]/3, goalLimits[0][1]};
                goalLimits[2] = {goalLimits[3][0]/3*2, goalLimits[3][1]};
                goalLimits[9] = {goalLimits[11][0]/3, goalLimits[8][1]};
                goalLimits[10] = {goalLimits[11][0]/3*2, goalLimits[11][1]};
                /////////////////////////////////////////////

                //////// Création des lines horizontal ////////
                goalLimits[4] = {goalLimits[0][0], goalLimits[8][1]/3};
                goalLimits[6] = {goalLimits[8][0], goalLimits[8][1]/3*2};
                goalLimits[5] = {goalLimits[3][0], goalLimits[11][1]/3};
                goalLimits[7] = {goalLimits[11][0], goalLimits[11][1]/3*2};
                ///////////////////////////////////////////////



                isCalib = true;
            }
        }
    }
    qDebug() << "minRadius: " + QString::number(minRadius[0])  + ", maxRadius: " + QString::number(maxRadius[1]) + ". En bas à gauche: x:" + QString::number(goalLimits[8][0]) + ", y: " + QString::number(goalLimits[8][1]) + ". En bas à droite: x: " + QString::number(goalLimits[11][0]) + ", y: " + QString::number(goalLimits[11][1]);


    return true;
}

//////// Permet de retourner la valeur de error (L'erreur généré si problème) ////////
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
        goalLimits[i] = {0,0};
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
