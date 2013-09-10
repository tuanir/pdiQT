#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QGraphicsPixmapItem>
#include <QScrollBar>
#include <QShortcut>

using namespace cv;
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QShortcut *shortcutOpen = new QShortcut(QKeySequence("Ctrl+O"),this);

    // Connect the signals with the slots
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openAction()));
    QObject::connect(shortcutOpen, SIGNAL(activated()), this, SLOT(openAction()));

    connect(ui->actionSalvar, SIGNAL(triggered()), this, SLOT(actionSalvar()));
    connect(ui->actionHistograma, SIGNAL(triggered()), this, SLOT(actionHistograma()));
    connect(ui->actionGaussiano, SIGNAL(triggered()), this, SLOT(actionGaussiano()));
    connect(ui->actionLaplaciano, SIGNAL(triggered()), this, SLOT(actionLaplaciano()));
    connect(ui->actionMedia, SIGNAL(triggered()), this, SLOT(actionMedia()));
    connect(ui->actionMediana, SIGNAL(triggered()), this, SLOT(actionMediana()));
    connect(ui->actionTons_de_cinza, SIGNAL(triggered()), this, SLOT(actionTons_de_cinza()));
    connect(ui->actionLimiarizacao, SIGNAL(triggered()), this, SLOT(actionLimiarizacao()));
    connect(ui->actionLimiarizacao_automatica, SIGNAL(triggered()), this, SLOT(actionLimiarizacao_automatica()));
    connect(ui->actionOtsu, SIGNAL(triggered()), this, SLOT(actionOtsu()));
    connect(ui->actionWatershed, SIGNAL(triggered()), this, SLOT(actionWatershed()));

    scene = new QGraphicsScene(this);
    item = new QGraphicsPixmapItem(NULL);
    scene->addItem(item);
    ui->graphicsView->setScene(scene);
    fileName = new QString();
    format = QImage::Format_RGB888;
    cv_img = Mat();
    cv_img_tmp = Mat();
    lastPath = "/home";

}

MainWindow::~MainWindow()
{
    delete ui;
}

// Open an ipl image in the cv_img
void MainWindow::openAction()
{
    *fileName = QFileDialog::getOpenFileName(this,"Open Image File",lastPath);
    lastPath = *fileName;
    if(!fileName->isEmpty())
    {
        //Open the image and check
        cv_img = imread(fileName->toStdString().c_str(), 1);//cv_img  = cvLoadImage(fileName->toStdString().c_str(), CV_LOAD_IMAGE_UNCHANGED);
        if(!cv_img.data)
        {
            std::cout << "ERROR: cvLoadImage failed" << std::endl;
            exit(0);
        }

        // Create a temp image with the same size of the opened image
        cv_img_tmp = cv_img.clone();//cv_img_tmp = cvCreateImage(cvGetSize(cv_img),cv_img->depth,1);

        //Set the RGB format
        format = QImage::Format_RGB888;

        // Convert and show the image
        ipl2QImage();
        ui->graphicsView->setSceneRect(scene->itemsBoundingRect());
        ui->graphicsView->show();
    }
}

// Convert the ipl cv_img to QImage
void MainWindow::ipl2QImage()
{
    IplImage *ipl = new IplImage(cv_img);
    int height = ipl->height;
    int width = ipl->width;
    const uchar *qImageBuffer = (const uchar*)ipl->imageData;
    QImage img(qImageBuffer, width, height, format);
    QImage qt_img = img.rgbSwapped();

    item->setPixmap(QPixmap::fromImage(qt_img));

    delete ipl;
}

// Save an ipl to file
void MainWindow::actionSalvar()
{
}

void MainWindow::actionHistograma()
{

}

void MainWindow::actionGaussiano()
{

}

void MainWindow::actionLaplaciano()
{

}

void MainWindow::actionMediana()
{
    if(cv_img.data)
    {
        medianBlur(cv_img, cv_img_tmp, 7);
        cv_img = cv_img_tmp.clone();
        ipl2QImage();
    }
}

void MainWindow::actionMedia()
{
    if(cv_img.data)
    {
        blur(cv_img, cv_img_tmp, Size(7,7) );
        cv_img = cv_img_tmp.clone();
        ipl2QImage();
    }
}

void MainWindow::actionTons_de_cinza()
{
    if(cv_img.data && format != QImage::Format_Indexed8)
    {
        // RGB2GRAY
        cvtColor(cv_img,cv_img_tmp,CV_RGB2GRAY);
        //cv_img = cv_img_tmp.clone();
        //blur(cv_img, cv_img_tmp, Size(7,7) );
        format = QImage::Format_Indexed8;
        cv_img = cv_img_tmp.clone();

        ipl2QImage();
    }
}

void MainWindow::actionLimiarizacao()
{

}

void MainWindow::actionLimiarizacao_automatica()
{

}

void MainWindow::actionOtsu()
{

}

void MainWindow::actionWatershed()
{

}
