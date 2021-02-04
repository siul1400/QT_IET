#ifndef CERCLE_H
#define CERCLE_H


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

private:
    int x[2];
    int y[2];
    int minRadius[2];
    int maxRadius[2];
};

#endif // CERCLE_H
