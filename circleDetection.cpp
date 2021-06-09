#include "circleDetection.h"


circleDetection::circleDetection(int* rminParam, int* rmaxParam, std::vector<int>* goalLimitsParam){
    minRadius[0] = rminParam[0];
    minRadius[1] = rminParam[1];

    maxRadius[0] = rmaxParam[0];
    maxRadius[1] = rmaxParam[1];

    for(int i = 0; i < 12; i++){
        goalLimits[i] = goalLimitsParam[i];
    }
}

QString circleDetection::getError()
{
    return error;
}

bool circleDetection::searchCircle(QString dirNamePathParam, QString dbFilePathParam, QProgressBar* progressBarParam, QTableWidget* viewParam, QTableWidget *viewStatParam)
{

    if(!checkDir(dirNamePathParam)){
        return false;
    }

    int nbImages = 0;
    std::vector<QString> imagesPath;
    if(!checkImagePath(dirNamePathParam, nbImages, imagesPath)){
        return false;
    }

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

    writeFile(imagesPath, area, nbImages, dbFilePathParam);
    if(!showTable(viewParam, viewStatParam)){
        return false;
    }
    return true;

}

QPixmap circleDetection::foundCircle(QString fileNameParam, int centralWidgetGeometryRight, int centralWidgetGeometryBottom)
{
    Mat src = getImage(fileNameParam); // Permet de charger l'image, indiqué dans filename.
    if(src.empty()){ // Vérification si l'image peut-être ouverte.
        QPixmap jvPhoto(":/img/error.png");
        return jvPhoto.scaled(centralWidgetGeometryRight, centralWidgetGeometryBottom, Qt::KeepAspectRatioByExpanding);
    }

    Mat gray = getModifyImage(src); // Créer une matrice nommé gray qui sera allouée dynamiquement.

    //src.convertTo(src, -1, 0.5, 0);
    std::vector<Vec3f> circles;
    getNumberCircle(gray, circles);
    qDebug() << "Nombre de cercle détecté: " + QString::number(circles.size());
    for( size_t i = 0; i < circles.size(); i++ )
    {
        Vec3i c = circles[i]; // Vector 3 int c = ..., [0]->x, [1]->y, [2]->rayon.
        src = showCircle(src , c);
    }

    /*for(int y=0;y<src.rows;y++)
    {
        if((y <= goalLimits[4][1]+5 && y >= goalLimits[4][1]-5) || (y <= goalLimits[6][1]+5 && y >= goalLimits[6][1]-5)){
            for(int x=0;x<src.cols;x++)
            {
                if((x <= goalLimits[1][0]+5 && x >= goalLimits[1][0]-5) || (x <= goalLimits[2][0]+5 && x >= goalLimits[2][0]-5)){
                    Vec3b color = src.at<Vec3b>(Point(x,y));
                    qDebug() << "goalLimit-x1: " << goalLimits[1][0] << ", goalLimits-y1: " << goalLimits[1][1] << ", goalLimit-x2: " << goalLimits[2][0] << ", goalLimits-y2: " << goalLimits[2][1] << ", y: " << y << ", x: " << x << ", color0: " << color[0] << ", color1: " << color[1] << ", color2: " << color[2];
                }
            }
        }
    }*/

    src = addLines(src);


    QPixmap jvPhoto(QPixmap::fromImage(QImage(src.data, src.cols, src.rows, src.step, QImage::Format_RGB888)));
    return jvPhoto.scaled(centralWidgetGeometryRight-10, centralWidgetGeometryBottom-10, Qt::KeepAspectRatioByExpanding);

}

bool circleDetection::checkDir(QString dirNamePathParam)
{
    QDir dir(dirNamePathParam);

    if(!dir.exists()){
        error = "Le dossier n'existe pas.";
        return false;
    }

    return true;
}

bool circleDetection::checkImagePath(QString dirNamePathParam, int &nbImagesParam, std::vector<QString> &imagesPathParam)
{
    QDir dir(dirNamePathParam);
    QFileInfoList listRepertoire = dir.entryInfoList(QDir::Dirs | QDir::Files);
    int numberFiles = 0;

    for (int i = 0; i < listRepertoire.size(); ++i) {
        QFileInfo fileInfos = listRepertoire.at(i);
        if(fileInfos.isFile() && (fileInfos.completeSuffix() == "png" || fileInfos.completeSuffix() == "jpg" || fileInfos.completeSuffix() == "PNG" || fileInfos.completeSuffix() == "JPG" ) && fileInfos.fileName() != "." && fileInfos.fileName() != "..")
        {
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

QString circleDetection::treatmentImage(QString imagePathParam)
{
    Mat src = getImage(imagePathParam);

    //////// Verification si l'image existe ////////
    if(src.empty()){                        ////////
        return "Images non trouvé !";       ////////
    }                                       ////////
    ////////////////////////////////////////////////

    Mat gray = getModifyImage(src); // Crée une matrice et ajoute un filtre sur l'image.

    std::vector<Vec3f> circles; // Cette variable stoque le nombre de cercle.

    getNumberCircle(gray, circles);


    qDebug() << "Nombre de cercle détecté: " + QString::number(circles.size());
    if(circles.size() > 1){
        return "Plus d'un seul cercle détecté !";
    }else if (circles.size() < 1){
        return "Aucun cercle détecté !";
    }

    Vec3i c = circles[0]; // Vector 3 int c = ..., [0]->x, [1]->y, [2]->rayon.

    src = showCircle(src, c);
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

Mat circleDetection::addLines(Mat srcParam)
{
    Scalar colorLine(0,255,0); // Green
    int thicknessLine = 5;

    ////////////////////////////// Line du bas //////////////////////////////
    Point p8(goalLimits[8][0],goalLimits[8][1]), p11(goalLimits[11][0],goalLimits[11][1]);
    line(srcParam, p8, p11, colorLine, thicknessLine);

    ////////////////////////////// Line du haut //////////////////////////////
    Point p0(goalLimits[0][0],goalLimits[0][1]), p3(goalLimits[3][0],goalLimits[3][1]);
    line(srcParam, p0, p3, colorLine, thicknessLine);

    ////////////////////////////// Line Vertcial gauche //////////////////////////////
    line(srcParam, p0, p8, colorLine, thicknessLine);


    ////////////////////////////// Line Vertical droite //////////////////////////////
    line(srcParam, p3, p11, colorLine, thicknessLine);

    ////////////////////////////// Line Horizontal du haut //////////////////////////////
    Point p4(goalLimits[4][0],goalLimits[4][1]), p5(goalLimits[5][0],goalLimits[5][1]);
    line(srcParam, p4, p5, colorLine, thicknessLine);

    ////////////////////////////// Line Horizontal du bas //////////////////////////////
    Point p6(goalLimits[6][0],goalLimits[6][1]), p7(goalLimits[7][0],goalLimits[7][1]);
    line(srcParam, p6, p7, colorLine, thicknessLine);

    ////////////////////////////// Line vertical 2ème en partant de la gauche //////////////////////////////
    Point p1(goalLimits[1][0],goalLimits[1][1]), p9(goalLimits[9][0],goalLimits[9][1]);
    line(srcParam, p1, p9, colorLine, thicknessLine);

    ////////////////////////////// Line Horizontal 3ème en partant de la gauche //////////////////////////////
    Point p2(goalLimits[2][0],goalLimits[2][1]), p10(goalLimits[10][0],goalLimits[10][1]);
    line(srcParam, p2, p10, colorLine, thicknessLine);

    return srcParam;
}

bool circleDetection::isZ1(int x, int y)
{
    if(x > goalLimits[0][0] && x < goalLimits[9][0] && y > goalLimits[6][1]){
        return true;
    }
    return false;
}
bool circleDetection::isZ2(int x, int y)
{
    if(x > goalLimits[0][0] && x < goalLimits[9][0] && y > goalLimits[4][1] && y < goalLimits[6][1]){
        return true;
    }
    return false;
}
bool circleDetection::isZ3(int x, int y)
{
    if(x < goalLimits[1][0] && y > goalLimits[0][1] && y < goalLimits[4][1]){
        qDebug() << "Limites x < " + QString::number(goalLimits[1][0]) + " && y > " + QString::number(goalLimits[0][1]) + " && y < " + QString::number(goalLimits[4][1]) + ".";
        return true;
    }
    return false;
}
bool circleDetection::isZ4(int x, int y)
{
    if(x > goalLimits[1][0] && x < goalLimits[2][0] && y > goalLimits[1][1] && y < goalLimits[4][1]){
        return true;
    }
    return false;
}
bool circleDetection::isZ5(int x, int y)
{
    if(x > goalLimits[2][0] && x < goalLimits[3][0] && y > goalLimits[2][1] && y < goalLimits[5][1]){
        return true;
    }
    return false;
}
bool circleDetection::isZ6(int x, int y)
{
    if(x > goalLimits[2][0] && x < goalLimits[3][0] && y > goalLimits[5][1] && y < goalLimits[7][1]){
        return true;
    }
    return false;
}
bool circleDetection::isZ7(int x, int y)
{
    if(x > goalLimits[10][0] && x < goalLimits[11][0] && y > goalLimits[7][1] && y < goalLimits[11][1]){
        return true;
    }
    return false;
}

bool circleDetection::createFile(QString dirNamePathParam)
{
    QString tableFileString = dirNamePathParam + "/" +QDate::currentDate().toString() + "-" + QTime::currentTime().toString() + ".xls";
    qDebug() << tableFileString;
    tableFile = new QFile( tableFileString );
    if ( this->tableFile->open(QIODevice::ReadWrite) )
    {
        return true;
    }
    error = "Erreur sur l'ouverture du fichier !";
    return false;
}

bool circleDetection::writeFile(std::vector<QString> imageNameParam, std::vector<QString> areaParam, int nbImagesParam, QString dbFilePathParam)
{
    //////// Temp ////////
    QString goodArea[7];
    int temp = 0;
    for(int i = 1; i <= 7; i++){
        goodArea[i-1] = "Z" + QString::number(i);
    }


    // Create database.
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "Connection");
    db.setDatabaseName(dbFilePathParam);
    if (!db.open()) {
        qDebug("Error occurred opening the database.");
        qDebug("%s.", qPrintable(db.lastError().text()));
        return -1;
    }

    // Insert table.
    QSqlQuery* model = new QSqlQuery(db);

    // Query.
    model->prepare("SELECT * FROM Training");
    if (!model->exec()) {
        error = "Problème avec la base de donnée !";
        return false;
    }


    QTextStream stream( tableFile );
    QString area;
    for(int i = 0; i < nbImagesParam; i++){
        if(!isDetected(areaParam.at(i))){
            area = "Hors du but !";
        }else if(areaParam[i] == goodArea[temp]){
            area = "oui";
        }else{
            area = "non";
        }
        model->next();
        stream << imageNameParam.at(i) << ", " << model->value(1).toString() << ", " << areaParam.at(i) << ", " << goodArea[temp] << ", " << area << endl;
        temp++;
        if(temp > 6){
            temp = 0;
        }
    }
    tableFile->close();
    return true;
}

bool circleDetection::showTable(QTableWidget *viewParam, QTableWidget *viewStatParam)
{
    std::vector<std::vector<QString>> players;
    bool playerExist = true;
    int shoot = 0;
    int goodShot = 0;

    viewParam->setColumnCount(5);
    viewParam->setHorizontalHeaderItem(0, new QTableWidgetItem("Nom de la photo"));
    viewParam->setHorizontalHeaderItem(1, new QTableWidgetItem("Joueur"));
    viewParam->setHorizontalHeaderItem(2, new QTableWidgetItem("Zone"));
    viewParam->setHorizontalHeaderItem(3, new QTableWidgetItem("Zone attendue"));
    viewParam->setHorizontalHeaderItem(4, new QTableWidgetItem("Valide"));

    if (!tableFile->open(QIODevice::ReadOnly | QIODevice::Text)){
        error = "Le tableau ne peut pas être chargé !";
        return false;
    }

    QString textSplit[5];
    while (!tableFile->atEnd()) {
        QByteArray text = tableFile->readLine();
        for(int i = 0; i < 5; i++){
            textSplit[i] = text.split(',')[i];
        }

        playerExist = true;
        for(int i = 0; i < players.size(); i++){
            if(textSplit[1] == players.at(i)[0]){
                goodShot = players.at(i)[1].toInt();
                shoot = players.at(i)[2].toInt()+1;

                if(textSplit[2] == textSplit[3]){
                    goodShot++;
                }

                players.at(i) = {players.at(i)[0], QString::number(goodShot), QString::number(shoot)};
            playerExist = false;
        }
    }

    if(playerExist || players.size() < 1){
        goodShot = 0;
        shoot = 1;

        if(textSplit[2] == textSplit[3]){
            goodShot++;
        }

        players.push_back({textSplit[1], QString::number(goodShot), QString::number(shoot)});
    }

    qDebug() << "Sortie du For........................................";


    viewParam->insertRow(viewParam->rowCount());
    viewParam->setItem(viewParam->rowCount()-1, 0, new QTableWidgetItem(textSplit[0]));
    viewParam->setItem(viewParam->rowCount()-1, 1, new QTableWidgetItem(textSplit[1]));
    viewParam->setItem(viewParam->rowCount()-1, 2, new QTableWidgetItem(textSplit[2]));
    viewParam->setItem(viewParam->rowCount()-1, 3, new QTableWidgetItem(textSplit[3]));
    viewParam->setItem(viewParam->rowCount()-1, 4, new QTableWidgetItem(textSplit[4]));

}
viewParam->resizeColumnsToContents();
viewParam->resizeRowsToContents();

viewStatParam->setColumnCount(4);
viewStatParam->setHorizontalHeaderItem(0, new QTableWidgetItem("Nom du joueur"));
viewStatParam->setHorizontalHeaderItem(1, new QTableWidgetItem("Nombre de tir réussi"));
viewStatParam->setHorizontalHeaderItem(2, new QTableWidgetItem("Nombre de tir"));
viewStatParam->setHorizontalHeaderItem(3, new QTableWidgetItem("Pourcentage de réussite"));

for(int i = 0; i < players.size(); i++){
    viewStatParam->insertRow(viewStatParam->rowCount());
    viewStatParam->setItem(viewStatParam->rowCount()-1, 0, new QTableWidgetItem(players.at(i)[0]));
    viewStatParam->setItem(viewStatParam->rowCount()-1, 1, new QTableWidgetItem(players.at(i)[1]));
    viewStatParam->setItem(viewStatParam->rowCount()-1, 2, new QTableWidgetItem(players.at(i)[2]));
    viewStatParam->setItem(viewStatParam->rowCount()-1, 3, new QTableWidgetItem(QString::number(players.at(i)[1].toInt()*100/players.at(i)[2].toInt()) + " %"));
}

qDebug() << players;

viewStatParam->resizeColumnsToContents();
viewStatParam->resizeRowsToContents();

return true;
}

bool circleDetection::isDetected(QString areaParam)
{
    if(areaParam != "Z1" && areaParam != "Z2" && areaParam != "Z3" && areaParam != "Z4" && areaParam != "Z5" && areaParam != "Z6" && areaParam != "Z7"){
        qDebug() << areaParam;
        return false;
    }
    return true;
}



//////// Partie pour le traitement de l'image ////////

Mat circleDetection::getImage(QString fileNameParam)
{
    const char* filename = fileNameParam.toUtf8(); // On enregistre le lien local de l'image.
    Mat src = imread( samples::findFile(filename), IMREAD_COLOR ); // Permet de charger l'image, indiqué dans filename.
    return src; /// TROUVER POURQUOI L'IMAGE CHANGE DE COULEUR /////////////////////////////////////////////////////////////////////////////
}

Mat circleDetection::getModifyImage(Mat srcParam)
{

    Mat gray; // Créer une matrice nommé gray qui sera allouée dynamiquement.
    cvtColor(srcParam, gray, COLOR_BGR2GRAY); // Permet de convertir l'image Blue Green Red en gray.
    //medianBlur(gray, gray, 5); // Permet d'ajouter un filtre median (pixelise une image).
    gray.convertTo(gray, -1, 1, -100);
    //Laplacian(gray, gray, CV_16S, 3);
    return gray;
}

void circleDetection::getNumberCircle(Mat grayParam, std::vector<Vec3f> &circleParam)
{
    HoughCircles(grayParam, circleParam, HOUGH_GRADIENT, 1, // Utilisation de la transformation de Hough, technique de reconnaissance de formes.
                 grayParam.rows/16,  // change this value to detect circles with different distances to each other
                 100, 30, minRadius[0], maxRadius[0] // change the last two parameters
            // (min_radius & max_radius) to detect larger circles
            );
}

Mat circleDetection::showCircle(Mat srcParam, Vec3i cParam)
{
    Point center = Point(cParam[0], cParam[1]); // coordonnées du centre du cercle.
    qDebug() << "Pos; x: " + QString::number(cParam[0]) + ", y: " + QString::number(cParam[1]);
    circle( srcParam, center, 1, Scalar(0,100,100), 3, LINE_AA); // Permet l'affichage d'un point au centre du cercle.
    int radius = cParam[2];
    circle( srcParam, center, radius, Scalar(255,0,255), 3, LINE_AA); // Permet l'affichage du cercle.
    return srcParam;
}
