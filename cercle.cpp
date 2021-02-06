#include "cercle.h"

using namespace cv;
using namespace std;

Cercle::Cercle(int* xParam, int* yParam, int* rminParam, int* rmaxParam)
{
    x[0] = xParam[0];
    x[1] = xParam[1];

    y[0] = yParam[0];
    y[1] = yParam[1];

    minRadius[0] = rminParam[0];
    minRadius[1] = rminParam[1];

    maxRadius[0] = rmaxParam[0];
    maxRadius[1] = rmaxParam[1];
}

QString Cercle::getError()
{
    return error;
}

bool Cercle::searchCircle(QString dirNamePathParam)
{
    if(!verifDir(dirNamePathParam)){
        return false;
    }

    /*if(!treatmentImagePath()){
        return false;
    }*/

    return true;

}

bool Cercle::verifDir(QString dirNamePathParam)
{
   // QDir *dir = new QDir(dirNamePathParam);

    /*if(dir->exists()){
        error = "Le dossier n'existe pas.";
        return false;
    }*/

    return true;
}

bool Cercle::treatmentImagePath()
{
    QFileInfoList listRepertoire = dir->entryInfoList(QDir::Dirs | QDir::Files);
       int numberFiles = 0;

       for (int i = 0; i < listRepertoire.size(); ++i) {
           QFileInfo fileInfos = listRepertoire.at(i);
           if(fileInfos.isFile())
           {
               numberFiles++;
           }
           else if(fileInfos.isDir()) {
             //numberFiles += countMusicInDir(fileInfos.absolutePath());
             qDebug() << fileInfos.absolutePath();


           }
       }
       qDebug()  << "Nombre de fichier dans ce dossier : " + QString().number(numberFiles) + " !";

       return true;
}
