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
    /**
     * @brief Calibration, Le constructeur permet d'initialiser différente variables.
     */
    Calibration();

    /**
     * @brief setCalib, Permet de choisir si la calibration est faite.
     * @param calib boolean.
     */
    void setCalib(bool calib);

    /**
     * @brief getCalib, Permet de savoir si la calibration est faite.
     * @return boolean.
     */
    bool getCalib();

    /**
     * @brief getXYR, Permet de retourné la variable demandé.
     * @param varParam, Permet de choisir qu'elle variable est retourné (x, y ou rmin, rmax), en QString.
     * @return
     */
    int* getRayon(QString varParam);

    /**
     * @brief searchCircle, Permet d'avoir les coordonnées xy d'un ballon situé en bas à gauche et en bas à droite d'une image.
     * @param fileNameParam, Prend le chemin du fichier en QString.
     * @param minRadiusParam, Prend un nombre pour le rayon minimum en Int.
     * @param maxRadiusParam, Prend un nombre pour le rayon maximum en Int.
     * @param leftOrRightParam, Prend soit 'Left' ou 'Right' en fonction du ballon s'il est situé en bas à gauche ou à droite en QString.
     * @return boolean.
     */
    bool searchCircle(QString fileNameParam, int minRadiusParam, int maxRadiusParam, QString leftOrRightParam);

    /**
     * @brief getError, Permet d'avoir une erreur si searchCircles() return false.
     * @return QString.
     */
    QString getError();

    /**
     * @brief resetCalib, Permet de renitialiser le système de calibration.
     */
    void resetCalib();

    /**
     * @brief getLimits, Permet de retourner un pointeur de la variable limits.
     * @return std::vector<int>*
     */
    std::vector<int>* getLimits();

private:

    /**
     * @brief HEIGHT_BUT, Constante pour la hauteur du but.
     */
    int HEIGHT_BUT = 420;
    /**
     * @brief precision, Permet de choisir le niveau de précision de la détéction (ici la précision est de 5 px) en Int.
     */
    int precision = 5;

    /**
     * @brief isCalib, Permet de choisir si le système de calibration est active ou non, en Boolean.
     */
    bool isCalib = false;

    /**
     * @brief minRadius, Stocke le rayon minimum de détection en Int. [0] => "Ballon en bas à gauche", [1] => "Ballon en bas à droite".
     */
    int minRadius[2];

    /**
     * @brief maxRadius, Stocke le rayon maximum de détection en Int. [0] => "Ballon en bas à gauche", [1] => "Ballon en bas à droite".
     */
    int maxRadius[2];

    /**
     * @brief limits, Stocke les positions pour tout les points.
     */
    std::vector<int> limits[12];

    /**
     * @brief fileNamePath, Stocke les chemins de l'image en QString. [0] => "Ballon en bas à gauche", [1] => "Ballon en bas à droite".
     */
    QString fileNamePath[2];

    /**
     * @brief error, Stocke l'erreur si searchCircle retourne false, en QString.
     */
    QString error = "";

    /**
     * @brief verifFileName, Permet de faire des vérification sur le chemin de l'image.
     * @param fileNameParam, Prend le chemin de l'image en QString.
     * @return boolean.
     */
    bool checkFileName(QString fileNameParam);
};

#endif // CALIBRATION_H
