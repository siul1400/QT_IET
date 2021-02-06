#ifndef CERCLE_H
#define CERCLE_H

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "vector"

#include <QString>
#include <QDir>
#include <QFileInfoList>
#include <QDebug>


class Cercle
{
public:
    /**
     * @brief Cercle, Permet d'initialiser les différents attribus.
     * @param xParam, Pointer vers un tableau.
     * @param yParam, Pointer vers un tableau.
     * @param rminParam, Pointer vers un tableau.
     * @param rmaxParam, Pointer vers un tableau.
     */
    Cercle(int* xParam, int* yParam, int* rminParam, int* rmaxParam);

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
    bool searchCircle(QString dirNamePathParam);


private:
    /**
     * @brief x, Permet de stocker la position en x du ballon sur les deux images de calibration. [0] => 'Ballon en bas à gauche', [1] => 'Ballon en bas à droite'.
     * @brief y, Permet de stocker la postion en y du ballon sur les deux images de calibration. [0] => 'Ballon en bas à gauche', [1] => 'Ballon en bas à droite'.
     * @brief minRadius, Permet de stocker le rayon minimum pour la détéction du ballon. [0] => 'Ballon en bas à gauche', [1] => 'Ballon en bas à droite'.
     * @brief maxRadius, Permet de stocker le rayon maxium pour la détéction du ballon. [0] => 'Ballon en bas à gauche', [1] => 'Ballon en bas à droite'.
     */
    int x[2];
    int y[2];
    int minRadius[2];
    int maxRadius[2];
    /**
     * @brief nbImages, Permet de stocker le nombre d'images présent dans le dossier.
     */
    int nbImages;

    /**
     * @brief error, Permet de stocker les différentes erreurs généré.
     */
    QString error;

    /**
     * @brief dirNamePath, Permet de stocker le chemin vers le dossier contenant toutes les images.
     */
    QString dirNamePath;

    /**
     * @brief dir, Création de l'objet QDir, pour manipuler le dossier contenant toutes les images.
     */
    QDir *dir;

    /**
     * @brief verifDir, Permet de vérifier le chemin du dossier.
     * @param dirNamePathParam, Chemin vers le dossier contenant les images.
     * @return Boolean
     */
    bool verifDir(QString dirNamePathParam);

    /**
     * @brief treatmentImagePath, Permet de faire une vérification du bon format des images.
     * @return Boolean.
     */
    bool treatmentImagePath();

};

#endif // CERCLE_H
