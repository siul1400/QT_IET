#ifndef CIRCLEDETECTION_H
#define CIRCLEDETECTION_H

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "vector"

#include <QString>
#include <QDir>
#include <QFileInfoList>
#include <QDebug>
#include <QApplication>

#include <QDate>
#include <QTime>
#include <QProgressBar>
#include <QTableWidget>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>


using namespace cv;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class circleDetection
{
public:
    /**
     * @brief Cercle, Permet d'initialiser les différents attribus.
     * @param xParam, Pointer vers un tableau.
     * @param yParam, Pointer vers un tableau.
     * @param rminParam, Pointer vers un tableau.
     * @param rmaxParam, Pointer vers un tableau.
     */
    circleDetection(int* rminParam, int* rmaxParam, std::vector<int>* limitsParam);


    /**
     * @brief getError, Permet d'avoir l'erreur si elle est géneré.
     * @return QString.
     */
    QString getError();

    /**
     * @brief searchCircle, Permet de chercher un cercle dans un groupe d'image.
     * @param dirNamePathParam, Chemin vers le dossier contenant les images.
     * @return Boolean.
     */
    bool searchCircle(QString dirNamePathParam, QString dbFilePathParam, QProgressBar* progressBarParam, QTableWidget* viewParam, QTableWidget *viewStatParam);

    /**
     * @brief foundImage, Permet de retourner une image avec la un cercle.
     * @param fileNameParam, Chemin du fichier.
     * @return Retourne une image.
     */
     QPixmap foundCircle(QString fileNameParam, int centralWidgetGeometryRight, int centralWidgetGeometryBottom);


private:
    /**
     * @brief minRadius, Permet de stocker le rayon minimum pour la détéction du ballon. [0] => 'Ballon en bas à gauche', [1] => 'Ballon en bas à droite'.
     * @brief maxRadius, Permet de stocker le rayon maxium pour la détéction du ballon. [0] => 'Ballon en bas à gauche', [1] => 'Ballon en bas à droite'.
     */
    int minRadius[2];
    int maxRadius[2];
    /**
     * @brief imageNumber, Permet de stocker le nombre d'images présent dans le dossier.
     */
    int imageNumber;

    /**
     * @brief error, Permet de stocker les différentes erreurs généré.
     */
    QString error;

    /**
     * @brief dirNamePath, Permet de stocker le chemin vers le dossier contenant toutes les images.
     */
    QString dirNamePath;

    /**
     * @brief verifDir, Permet de vérifier le chemin du dossier.
     * @param dirNamePathParam, Chemin vers le dossier contenant les images.
     * @return Boolean
     */
    bool checkDir(QString dirNamePathParam);

    /**
     * @brief treatmentImagePath, Permet de faire une vérification du bon format des images.
     * @return Boolean.
     */
    bool checkImagePath(QString dirNamePathParam, int &nbImagesParam, std::vector<QString> &imagesPathParam);

    /**
     * @brief limits, Stocke tout les points.
     */
    std::vector<int> goalLimits[12];

    QString treatmentImage(QString imagePathParam);

    Mat addLines(Mat srcParam);

    bool isZ1(int x, int y);
    bool isZ2(int x, int y);
    bool isZ3(int x, int y);
    bool isZ4(int x, int y);
    bool isZ5(int x, int y);
    bool isZ6(int x, int y);
    bool isZ7(int x, int y);

    bool createFile(QString dirNamePathParam);
    bool writeFile(std::vector<QString> imageNameParam, std::vector<QString> areaParam, int nbImagesParam, QString dbFilePathParam);

    bool showTable(QTableWidget* viewParam, QTableWidget *viewStatParam);

    QFile *tableFile;

    bool isDetected(QString areaParam);

    //////// Partie pour le traitement de l'image ////////

    Mat getImage(QString fileNameParam);

    Mat getModifyImage(Mat srcParam);

    void getNumberCircle(Mat grayParam, std::vector<Vec3f> &circleParam);

    Mat showCircle(Mat srcParam, Vec3i cParam);

    QSqlQuery *model;

};

#endif // CIRCLEDETECTION_H
