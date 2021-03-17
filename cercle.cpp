 #include "cercle.h"

using namespace cv;
using namespace std;

Cercle::Cercle(int* rminParam, int* rmaxParam, std::vector<int>* limitsParam){
    minRadius[0] = rminParam[0];
    minRadius[1] = rminParam[1];

    maxRadius[0] = rmaxParam[0];
    maxRadius[1] = rmaxParam[1];

    for(int i = 0; i < 12; i++){
        limits[i] = limitsParam[i];
    }
}

QString Cercle::getError()
{
    return error;
}

bool Cercle::searchCircle(QString dirNamePathParam, QProgressBar* progressBarParam)
{
    if(!checkDir(dirNamePathParam)){
        return false;
    }

    int nbImages = 0;
    std::vector<QString> imagesPath;
    if(!checkImagePath(dirNamePathParam, nbImages, imagesPath)){
        return false;
    }

    qDebug() << imagesPath.size();

    if(!createFile(dirNamePathParam)){
        return false;
    }

    std::vector<QString> area;
    progressBarParam->setMaximum(nbImages);
    for(int i = 0; i < nbImages; i++){
        area.push_back(treatmentImage(dirNamePathParam + "/" + imagesPath.at(i)));
        qDebug() << dirNamePathParam + "/" + imagesPath.at(i);
        qDebug() <<  area.at(i); // Changer nom de l'image.
        progressBarParam->setValue(i+1);
        qApp->processEvents();
        qDebug() << "------------------------------------------------------";
    }

    writeFile(imagesPath, area, nbImages);
    return true;

}

bool Cercle::checkDir(QString dirNamePathParam)
{
   QDir dir(dirNamePathParam);

    if(!dir.exists()){
        error = "Le dossier n'existe pas.";
        return false;
    }

    return true;
}

bool Cercle::checkImagePath(QString dirNamePathParam, int &nbImagesParam, std::vector<QString> &imagesPathParam)
{
    QDir dir(dirNamePathParam);
    QFileInfoList listRepertoire = dir.entryInfoList(QDir::Dirs | QDir::Files);
       int numberFiles = 0;

       for (int i = 0; i < listRepertoire.size(); ++i) {
           QFileInfo fileInfos = listRepertoire.at(i);
           if(fileInfos.isFile() && (fileInfos.completeSuffix() == "png" || fileInfos.completeSuffix() == "jpg" || fileInfos.completeSuffix() == "PNG" || fileInfos.completeSuffix() == "JPG" ) && fileInfos.fileName() != "." && fileInfos.fileName() != "..")
           {
               qDebug() << fileInfos.completeSuffix();
               numberFiles++;
               imagesPathParam.push_back(listRepertoire.at(i).fileName()); ///////////////// PAS TESTER
           }
           else if(fileInfos.isDir()) {
             //numberFiles += countMusicInDir(fileInfos.absolutePath());
             qDebug() << fileInfos.absolutePath();


           }
       }
       qDebug()  << "Nombre de fichier dans ce dossier : " + QString().number(numberFiles) + " !";

       if(numberFiles <= 0){
           error = "Nombre de fichier non valide: " + QString().number(numberFiles);
           return false;
       }
       nbImagesParam = numberFiles;

       return true;
}

QString Cercle::treatmentImage(QString imagePathParam)
{

    const char* filename = imagePathParam.toUtf8(); // On enregistre le lien local de l'image.

    Mat src = imread( samples::findFile(filename), IMREAD_COLOR ); // Permet de charger l'image, indiqué dans filename.
    if(src.empty()){ // Vérification si l'image peut-être ouverte.
        return "Images non trouvé !";
    }

    Mat gray; // Créer une matrice nommé gray qui sera allouée dynamiquement.
    cvtColor(src, gray, COLOR_BGR2GRAY); // Permet de convertir l'image Blue Green Red en gray.

    medianBlur(gray, gray, 5); // Permet d'ajouter un filtre median (pixelise une image).
    vector<Vec3f> circles;
    HoughCircles(gray, circles, HOUGH_GRADIENT, 1, // Utilisation de la transformation de Hough, technique de reconnaissance de formes.
                 gray.rows/16,  // change this value to detect circles with different distances to each other
                 100, 30, minRadius[0], maxRadius[0] // change the last two parameters
                 // (min_radius & max_radius) to detect larger circles
                 );
    qDebug() << "Nombre de cercle détecté: " + QString::number(circles.size());
    if(circles.size() > 1){
        return "Plus d'un seul cercle détecté !";
    }else if (circles.size() < 1){
        return "Aucun cercle détecté !";
    }

    Vec3i c = circles[0]; // Vector 3 int c = ..., [0]->x, [1]->y, [2]->rayon.
    Point center = Point(c[0], c[1]); // coordonnées du centre du cercle.
    qDebug() << "Pos; x: " + QString::number(c[0]) + ", y: " + QString::number(c[1]);
    circle( src, center, 1, Scalar(0,100,100), 3, LINE_AA); // Permet l'affichage d'un point au centre du cercle.
    int radius = c[2];
    circle( src, center, radius, Scalar(255,0,255), 3, LINE_AA); // Permet l'affichage du cercle.

    if(isZ1(c[0], c[1])){
        return "Z1";
    }else if(isZ2(c[0], c[1])){
        return "Z2";
    }else if(isZ3(c[0], c[1])){
        return "Z3";
    }else if(isZ4(c[0], c[1])){
        return "Z4";
    }else if(isZ5(c[0], c[1])){
        return "Z5";
    }else if(isZ6(c[0], c[1])){
        return "Z6";
    }else if(isZ7(c[0], c[1])){
        return "Z7";
    }
    return "Hors du but !";
}

Mat Cercle::addLines(Mat srcParam)
{
    Scalar colorLine(0,255,0); // Green
    int thicknessLine = 5;

    ////////////////////////////// Line du bas //////////////////////////////
    Point p8(limits[8][0],limits[8][1]), p11(limits[11][0],limits[11][1]);
    line(srcParam, p8, p11, colorLine, thicknessLine);

    ////////////////////////////// Line du haut //////////////////////////////
    Point p0(limits[0][0],limits[0][1]), p3(limits[3][0],limits[3][1]);
    line(srcParam, p0, p3, colorLine, thicknessLine);

    ////////////////////////////// Line Vertcial gauche //////////////////////////////
    line(srcParam, p0, p8, colorLine, thicknessLine);


    ////////////////////////////// Line Vertical droite //////////////////////////////
    line(srcParam, p3, p11, colorLine, thicknessLine);

    ////////////////////////////// Line Horizontal du haut //////////////////////////////
    Point p4(limits[4][0],limits[4][1]), p5(limits[5][0],limits[5][1]);
    line(srcParam, p4, p5, colorLine, thicknessLine);

    ////////////////////////////// Line Horizontal du bas //////////////////////////////
    Point p6(limits[6][0],limits[6][1]), p7(limits[7][0],limits[7][1]);
    line(srcParam, p6, p7, colorLine, thicknessLine);

    ////////////////////////////// Line vertical 2ème en partant de la gauche //////////////////////////////
    Point p1(limits[1][0],limits[1][1]), p9(limits[9][0],limits[9][1]);
    line(srcParam, p1, p9, colorLine, thicknessLine);

    ////////////////////////////// Line Horizontal 3ème en partant de la gauche //////////////////////////////
    Point p2(limits[2][0],limits[2][1]), p10(limits[10][0],limits[10][1]);
    line(srcParam, p2, p10, colorLine, thicknessLine);

    return srcParam;
}

bool Cercle::isZ1(int x, int y)
{
    if(x > limits[0][0] && x < limits[9][0] && y > limits[6][1]){
        return true;
    }
    return false;
}
bool Cercle::isZ2(int x, int y)
{
    if(x > limits[0][0] && x < limits[9][0] && y > limits[4][1] && y < limits[6][1]){
        return true;
    }
    return false;
}
bool Cercle::isZ3(int x, int y)
{
    if(x < limits[1][0] && y > limits[0][1] && y < limits[4][1]){
        qDebug() << "Limites x < " + QString::number(limits[1][0]) + " && y > " + QString::number(limits[0][1]) + " && y < " + QString::number(limits[4][1]) + ".";
        return true;
    }
    return false;
}
bool Cercle::isZ4(int x, int y)
{
    if(x > limits[1][0] && x < limits[2][0] && y > limits[1][1] && y < limits[4][1]){
        return true;
    }
    return false;
}
bool Cercle::isZ5(int x, int y)
{
    if(x > limits[2][0] && x < limits[3][0] && y > limits[2][1] && y < limits[5][1]){
        return true;
    }
    return false;
}
bool Cercle::isZ6(int x, int y)
{
    if(x > limits[2][0] && x < limits[3][0] && y > limits[5][1] && y < limits[7][1]){
        return true;
    }
    return false;
}
bool Cercle::isZ7(int x, int y)
{
    if(x > limits[10][0] && x < limits[11][0] && y > limits[7][1] && y < limits[11][1]){
        return true;
    }
    return false;
}

bool Cercle::createFile(QString dirNamePathParam)
{
    QString tableFileString = dirNamePathParam + "/" +QDate::currentDate().toString() + "-" + QTime::currentTime().toString() + ".xls";
    qDebug() << tableFileString;
    this->tableFile = new QFile( tableFileString );
    if ( this->tableFile->open(QIODevice::ReadWrite) )
    {
        return true;
    }
    error = "Erreur sur l'ouverture du fichier !";
    return false;
}

bool Cercle::writeFile(std::vector<QString> imageNameParam, std::vector<QString> areaParam, int nbImagesParam)
{
    QTextStream stream( this->tableFile );
    for(int i = 0; i < nbImagesParam; i++){
        stream << imageNameParam.at(i) << ", " << areaParam.at(i) << endl;
        qDebug() << "index: " + QString::number(i);
    }
    return true;
}
