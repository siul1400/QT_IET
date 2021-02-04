#ifndef CERCLE_H
#define CERCLE_H

#include <QString>

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

    /**
     * @brief verifDir, Permet de vérifier le chemin du dossier.
     * @param dirNamePathParam, Chemin vers le dossier contenant les images.
     * @return Boolean
     */
    bool verifDir(QString dirNamePathParam);

private:
    int x[2];
    int y[2];
    int minRadius[2];
    int maxRadius[2];

    QString error;
};

#endif // CERCLE_H
