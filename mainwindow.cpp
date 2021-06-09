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
    delete circle;
    delete ui;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////// Partie calibration de l'IHM ///////////////////////////////////////////////////////////
//////// Permet de demander à l'utilisateur la photo du ballon situé en bas à gauche ////////
void MainWindow::on_bottomLeftButton_clicked()
{
    QMessageBox::information(this, "Choisir un fichier", "Veuillez choisir l'image où le ballon est situé en bas à gauche.");
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Text"),
                                                    "/Users/prof/Documents/TS2SN/Luis/Projet/OpenCv",
                                                    tr("Image File (*.jpg *.png)"));
    if(!calibration.searchCircle(fileName, 20, "Left")){
        QMessageBox::warning(this, "Erreur", calibration.getError());
    }else{
        setDisableCalibMenu();
    }
}

//////// Permet de demander à l'utilisateur la photo du ballon situé en bas à droite ////////
void MainWindow::on_bottomRightBottom_clicked()
{
    QMessageBox::information(this, "Choisir un fichier", "Veuillez choisir l'image où le ballon est situé en bas à droite.");
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Text"),
                                                    "/Users/prof/Documents/TS2SN/Luis/Projet/OpenCv",
                                                    tr("Image File (*.jpg *.png)"));
    if(!calibration.searchCircle(fileName, 20, "Right")){
        QMessageBox::warning(this, "Erreur", calibration.getError());
    }else{
        setDisableCalibMenu();
    }
}

//////// Permet de demander à l'utilisateur une image pour tester la calibration ////////
void MainWindow::on_testImageButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"),
                                                    "/Users/prof/Documents/TS2SN/Luis/Projet/OpenCv",
                                                    tr("Image File (*.jpg *.png)"));

    circle = new circleDetection(calibration.getRayon("rmin"), calibration.getRayon("rmax"), calibration.getLimits());

    ui->imageTestLabel->setPixmap(circle->foundCircle(fileName, ui->centralwidget->geometry().right(), ui->centralwidget->geometry().bottom()));
    ui->validNoButton->setEnabled(true);
    ui->validYesButton->setEnabled(true);
}

//////// Bouton "Non" du système de calibration ////////
void MainWindow::on_validNoButton_clicked()
{
    calibration.resetCalib();
    setActiveCalibMenu();
    delete circle;
}

//////// Bouton "Oui" du système de calibration ////////
void MainWindow::on_validYesButton_clicked()
{
    ui->tabWidget->setTabEnabled(1, calibration.getCalib());
    ui->validYesButton->setEnabled(false);
}

//////// Permet d'activer le système de calibration ////////
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

//////// Permet de désactiver le système de calibration ////////
void MainWindow::setDisableCalibMenu()
{
    if(calibration.getCalib()){
        ui->bottomLeftButton->setEnabled(false);
        ui->bottomRightBottom->setEnabled(false);
        ui->testImageButton->setEnabled(true);
    }
}

//////// Permet de changer l'état de l'onglet "traitement" ////////
void MainWindow::setOpenFileButton(bool calib)
{
    ui->openFileButton->setEnabled(calib);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////// Partie traitement de l'IHM ////////////////////////////////////////////////////////////

//////// Permet de demander à l'utilisateur le dossier contenant toutes les images ////////
void MainWindow::on_openFileButton_clicked()
{
    QString fileName = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                         "/Users/prof/Documents/TS2SN/Luis/Projet/OpenCv",
                                                         QFileDialog::ShowDirsOnly
                                                         | QFileDialog::DontResolveSymlinks);

    ui->label_6->setText("Dossier: " + fileName);

    pathDirImage = fileName;

    treatmentDir();


}

//////// Permet de demander à l'utilisateur le chemin vers la base de donnée ////////
void MainWindow::on_openDbButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open DataBase"),
                                                    "/Users/prof/Documents/TS2SN/Luis/Projet/OpenCv",
                                                    tr("Sql File (*.db)"));

    ui->label_5->setText("Base de donnée: " + fileName);

    pathDbFile = fileName;

    treatmentDir();
}

//////// Permet de lancer le système de traitement ////////
bool MainWindow::treatmentDir()
{
    if(pathDirImage.isNull()){
        return false;
    }else if(pathDbFile.isNull()){
        return false;
    }

    if(!circle->searchCircle(pathDirImage, pathDbFile, ui->progressBar, ui->tableWidget, ui ->tableWidget_2)){
        QMessageBox::warning(this, "Erreur", circle->getError());
    }
    return true;
}

//////// Permet de changer la valeur de la bar de chargement, sur le traitement des images ////////
void MainWindow::setProgressBar(int valueParam)
{
    ui->progressBar->setValue(valueParam);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
