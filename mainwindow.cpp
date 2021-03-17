#include "mainwindow.h"
#include "ui_mainwindow.h"


using namespace cv;
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->setTabEnabled(1, calibration.getCalib());
    setActiveCalibMenu();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setOpenFileButton(bool calib)
{
    ui->openFileButton->setEnabled(calib);
}

void MainWindow::setActiveCalibMenu()
{
    if(!calibration.getCalib()){
        ui->bottomLeftButton->setEnabled(true);
        ui->bottomRightBottom->setEnabled(true);
        ui->testImageButton->setEnabled(false);
        ui->validNoButton->setEnabled(false);
        ui->validYesButton->setEnabled(false);
    }
}

void MainWindow::setDisableCalibMenu()
{
    if(calibration.getCalib()){
        ui->bottomLeftButton->setEnabled(false);
        ui->bottomRightBottom->setEnabled(false);
        ui->testImageButton->setEnabled(true);
    }
}

void MainWindow::setProgressBar(int valueParam)
{
    ui->progressBar->setValue(valueParam);
}


void MainWindow::on_openFileButton_clicked()
{
    QString fileName = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                         "/Users/prof/Documents/TS2SN/Luis/Projet/OpenCv",
                                                         QFileDialog::ShowDirsOnly
                                                         | QFileDialog::DontResolveSymlinks);
    if(!this->cercle->searchCircle(fileName, ui->progressBar)){
        QMessageBox::warning(this, "Erreur", this->cercle->getError());
    }

}


QPixmap MainWindow::traitement_image(QString fileName)
{
    const char* filename = fileName.toUtf8(); // On enregistre le lien local de l'image.

    Mat src = imread( samples::findFile(filename), IMREAD_COLOR ); // Permet de charger l'image, indiqué dans filename.

    if(src.empty()){ // Vérification si l'image peut-être ouverte.
        //printf(" Erreur ouverture image\n");
        //printf(" Program Arguments: [image_name -- default %s] \n", filename);

        QPixmap jvPhoto(":/img/error.png");
        return jvPhoto.scaled(ui->centralwidget->geometry().right(), ui->centralwidget->geometry().bottom(), Qt::KeepAspectRatioByExpanding);
    }

    Mat gray; // Créer une matrice nommé gray qui sera allouée dynamiquement.
    cvtColor(src, gray, COLOR_BGR2GRAY); // Permet de convertir l'image Blue Green Red en gray.

    medianBlur(gray, gray, 5); // Permet d'ajouter un filtre median (pixelise une image).
    vector<Vec3f> circles;
    HoughCircles(gray, circles, HOUGH_GRADIENT, 1, // Utilisation de la transformation de Hough, technique de reconnaissance de formes.
                 gray.rows/16,  // change this value to detect circles with different distances to each other
                 100, 30, calibration.getRayon("rmin")[0], calibration.getRayon("rmax")[0] // change the last two parameters
                 // (min_radius & max_radius) to detect larger circles
                 );
    qDebug() << "Nombre de cercle détecté: " + QString::number(circles.size());
    for( size_t i = 0; i < circles.size(); i++ )
    {
        Vec3i c = circles[i]; // Vector 3 int c = ..., [0]->x, [1]->y, [2]->rayon.
        Point center = Point(c[0], c[1]); // coordonnées du centre du cercle.
        qDebug() << "Pos; x: " + QString::number(c[0]) + ", y: " + QString::number(c[1]);
        circle( src, center, 1, Scalar(0,100,100), 3, LINE_AA); // Permet l'affichage d'un point au centre du cercle.
        int radius = c[2];
        circle( src, center, radius, Scalar(255,0,255), 3, LINE_AA); // Permet l'affichage du cercle.
    }

    Scalar colorLine(0,255,0); // Green
    int thicknessLine = 5;

    ////////////////////////////// Line du bas //////////////////////////////
    Point p8(calibration.getLimits()[8][0],calibration.getLimits()[8][1]), p11(calibration.getLimits()[11][0],calibration.getLimits()[11][1]);
    line(src, p8, p11, colorLine, thicknessLine);

    ////////////////////////////// Line du haut //////////////////////////////
    Point p0(calibration.getLimits()[0][0],calibration.getLimits()[0][1]), p3(calibration.getLimits()[3][0],calibration.getLimits()[3][1]);
    line(src, p0, p3, colorLine, thicknessLine);

    ////////////////////////////// Line Vertcial gauche //////////////////////////////
    line(src, p0, p8, colorLine, thicknessLine);


    ////////////////////////////// Line Vertical droite //////////////////////////////
    line(src, p3, p11, colorLine, thicknessLine);

    ////////////////////////////// Line Horizontal du haut //////////////////////////////
    Point p4(calibration.getLimits()[4][0],calibration.getLimits()[4][1]), p5(calibration.getLimits()[5][0],calibration.getLimits()[5][1]);
    line(src, p4, p5, colorLine, thicknessLine);

    ////////////////////////////// Line Horizontal du bas //////////////////////////////
    Point p6(calibration.getLimits()[6][0],calibration.getLimits()[6][1]), p7(calibration.getLimits()[7][0],calibration.getLimits()[7][1]);
    line(src, p6, p7, colorLine, thicknessLine);

    ////////////////////////////// Line vertical 2ème en partant de la gauche //////////////////////////////
    Point p1(calibration.getLimits()[1][0],calibration.getLimits()[1][1]), p9(calibration.getLimits()[9][0],calibration.getLimits()[9][1]);
    line(src, p1, p9, colorLine, thicknessLine);

    ////////////////////////////// Line Horizontal 3ème en partant de la gauche //////////////////////////////
    Point p2(calibration.getLimits()[2][0],calibration.getLimits()[2][1]), p10(calibration.getLimits()[10][0],calibration.getLimits()[10][1]);
    line(src, p2, p10, colorLine, thicknessLine);

    QPixmap jvPhoto(QPixmap::fromImage(QImage(src.data, src.cols, src.rows, src.step, QImage::Format_RGB888)));
    return jvPhoto.scaled(ui->centralwidget->geometry().right(), ui->centralwidget->geometry().bottom(), Qt::KeepAspectRatioByExpanding);
}

void MainWindow::on_bottomLeftButton_clicked()
{
    QMessageBox::information(this, "Choisir un fichier", "Veuillez choisir l'image où le ballon est situé en bas à gauche.");
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Text"),
                                                    "/Users/prof/Documents/TS2SN/Luis/Projet/OpenCv",
                                                    tr("Image File (*.jpg *.png)"));
    if(!calibration.searchCircle(fileName, 20, 40, "Left")){
        QMessageBox::warning(this, "Erreur", calibration.getError());
    }else{
        setDisableCalibMenu();
    }
}

void MainWindow::on_bottomRightBottom_clicked()
{
    QMessageBox::information(this, "Choisir un fichier", "Veuillez choisir l'image où le ballon est situé en bas à droite.");
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Text"),
                                                    "/Users/prof/Documents/TS2SN/Luis/Projet/OpenCv",
                                                    tr("Image File (*.jpg *.png)"));
    if(!calibration.searchCircle(fileName, 20, 40, "Right")){
        QMessageBox::warning(this, "Erreur", calibration.getError());
    }else{
        setDisableCalibMenu();
    }
}

void MainWindow::on_testImageButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"),
                                                    "/Users/prof/Documents/TS2SN/Luis/Projet/OpenCv",
                                                    tr("Image File (*.jpg *.png)"));


    ui->imageTestLabel->setPixmap(traitement_image(fileName));
    ui->validNoButton->setEnabled(true);
    ui->validYesButton->setEnabled(true);
}

void MainWindow::on_validNoButton_clicked()
{
    calibration.resetCalib();
    setActiveCalibMenu();
}

void MainWindow::on_validYesButton_clicked()
{
    ui->tabWidget->setTabEnabled(1, calibration.getCalib());
    this->cercle = new Cercle(calibration.getRayon("rmin"), calibration.getRayon("rmax"), calibration.getLimits());
}
