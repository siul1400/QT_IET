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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setOpenFileButton(bool calib)
{
    ui->openFileButton->setEnabled(calib);
}


void MainWindow::on_openFileButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Text"),
                                                    "/Users/prof/Documents/TS2SN/Luis/Projet/OpenCv",
                                                    tr("Image File (*.jpg)"));

    ui->imageLabel->setPixmap(traitement_image(fileName));
}


QPixmap MainWindow::traitement_image(QString fileName)
{
    const char* filename = fileName.toUtf8(); // On enregistre le lien local de l'image.

    Mat src = imread( samples::findFile( filename ), IMREAD_COLOR ); // Permet de charger l'image, indiqué dans filename.

    if(src.empty()){ // Vérification si l'image peut-être ouverte.
        printf(" Erreur ouverture image\n");
        printf(" Program Arguments: [image_name -- default %s] \n", filename);

        QPixmap jvPhoto(":/img/error.png");
        return jvPhoto.scaled(ui->centralwidget->geometry().right(), ui->centralwidget->geometry().bottom(), Qt::KeepAspectRatioByExpanding);
    }

    Mat gray; // Créer une matrice nommé gray qui sera allouée dynamiquement.
    cvtColor(src, gray, COLOR_BGR2GRAY); // Permet de convertir l'image Blue Green Red en gray.

    medianBlur(gray, gray, 5); // Permet d'ajouter un filtre median (pixelise une image).
    vector<Vec3f> circles;
    HoughCircles(gray, circles, HOUGH_GRADIENT, 1, // Utilisation de la transformation de Hough, technique de reconnaissance de formes.
                 gray.rows/16,  // change this value to detect circles with different distances to each other
                 100, 30, calibration.getXYR("r", 0), calibration.getXYR("r", 1) // change the last two parameters
                 // (min_radius & max_radius) to detect larger circles
                 );
    qDebug() << "Nombre de cercle détecté: " + QString::number(circles.size());
    for( size_t i = 0; i < circles.size(); i++ )
    {
        Vec3i c = circles[i]; // Vector 3 int c = ..., [0]->x, [1]->y, [2]->rayon.
        Point center = Point(c[0], c[1]); // coordonnées du centre du cercle.
        circle( src, center, 1, Scalar(0,100,100), 3, LINE_AA); // Permet l'affichage d'un point au centre du cercle.
        int radius = c[2];
        circle( src, center, radius, Scalar(255,0,255), 3, LINE_AA); // Permet l'affichage du cercle.
    }
       Point p1(calibration.getXYR("x", 0),calibration.getXYR("y", 0)), p2(calibration.getXYR("x", 1),calibration.getXYR("y", 1));
       Scalar colorLine(0,255,0); // Green
       int thicknessLine = 2;

       line(src, p1, p2, colorLine, thicknessLine);

    QPixmap jvPhoto(QPixmap::fromImage(QImage(src.data, src.cols, src.rows, src.step, QImage::Format_RGB888)));
    return jvPhoto.scaled(ui->centralwidget->geometry().right(), ui->centralwidget->geometry().bottom(), Qt::KeepAspectRatioByExpanding);
}

void MainWindow::on_validationButton_clicked()
{
    calibration.setCalib(!calibration.getCalib());
    ui->tabWidget->setTabEnabled(1, calibration.getCalib());
}

void MainWindow::on_bottomLeftButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Text"),
                                                    "/Users/prof/Documents/TS2SN/Luis/Projet/OpenCv",
                                                    tr("Image File (*.jpg)"));
    if(!calibration.searchCircle(fileName, 20, 40, "Left")){
        QMessageBox::warning(this, "Erreur", calibration.getError());
    }
}

void MainWindow::on_bottomRightBottom_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Text"),
                                                    "/Users/prof/Documents/TS2SN/Luis/Projet/OpenCv",
                                                    tr("Image File (*.jpg)"));
    if(!calibration.searchCircle(fileName, 20, 40, "Right")){
        QMessageBox::warning(this, "Erreur", calibration.getError());
    }
}

void MainWindow::on_testImageButton_clicked()
{

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Text"),
                                                    "/Users/prof/Documents/TS2SN/Luis/Projet/OpenCv",
                                                    tr("Image File (*.jpg)"));


    ui->imageTestLabel->setPixmap(traitement_image(fileName));
}
