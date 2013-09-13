#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QGraphicsPixmapItem>
#include <QScrollBar>
#include <QShortcut>
#include <QPushButton>
#include <iostream>

using namespace cv;
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QShortcut *shortcutOpen = new QShortcut(QKeySequence("Ctrl+O"),this);
    QShortcut *shortcutQuit = new QShortcut(QKeySequence("Ctrl+Q"),this);

    // Connect the signals with the slots

    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openAction()));
    connect(shortcutOpen, SIGNAL(activated()), this, SLOT(openAction()));

    connect(ui->actionSair, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(shortcutQuit, SIGNAL(activated()), qApp, SLOT(quit()));

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

    // Image settings
    scene = new QGraphicsScene(this);
    item = new QGraphicsPixmapItem(NULL);
    scene->addItem(item);
    ui->graphicsView->setScene(scene);    
    format = QImage::Format_RGB888;
    cv_img = Mat();
    cv_img_tmp = Mat();

    // File path and last opened folder path
    fileName = new QString();
    lastPath = "~";

    // PopUp settings
    widget = new QWidget;
    gridLayout = new QGridLayout;
    ok = new QPushButton("Ok");
    slider = new QSlider(Qt::Horizontal);
    slider->setMinimum(1);
    slider->setMaximum(10);
    gridLayout->addWidget(slider);
    gridLayout->addWidget(ok);
    widget->setLayout(gridLayout);
    widget->setWindowTitle("Parâmetro");
    connect(slider, SIGNAL(valueChanged(int)),this, SLOT(setFilter(int)));
    connect(ok, SIGNAL(clicked()), this, SLOT(applyFilter()));

    // Main Window settings
    this->setWindowTitle("PDI");
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
            std::cout << "ERROR: imread failed" << std::endl;
            exit(0);
        }

        // Create a temp image with the same size of the opened image
        cv_img_tmp = cv_img.clone();//cv_img_tmp = cvCreateImage(cvGetSize(cv_img),cv_img->depth,1);

        //Set the RGB format
        format = QImage::Format_RGB888;

        // Convert and show the image
        ipl2QImage(cv_img);
        ui->graphicsView->setSceneRect(scene->itemsBoundingRect());
        ui->graphicsView->show();
    }
}

// Convert the ipl cv_img to QImage
void MainWindow::ipl2QImage(Mat image)
{
    IplImage *ipl = new IplImage(image);
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
    currentFilter = 3;
    if(cv_img.data)
    {
        slider->setValue(1);
        widget->show();
    }
}

void MainWindow::actionLaplaciano()
{

}

void MainWindow::actionMediana()
{
    currentFilter = 1;
    if(cv_img.data)
    {        
        slider->setValue(1);
        widget->show();
    }
}

void MainWindow::actionMedia()
{
    currentFilter = 2;
    if(cv_img.data)
    {        
        slider->setValue(1);
        widget->show();
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

        ipl2QImage(cv_img);
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

void MainWindow::setFilter(int i)
{
    int odd = 2*i+1;
    switch(currentFilter)
    {
        case 1:
            medianBlur(cv_img, cv_img_tmp, odd);
            break;
        case 2:
            blur(cv_img, cv_img_tmp, Size(i,i) );
            break;
        case 3:
            GaussianBlur(cv_img, cv_img_tmp, Size(odd,odd), 10, 10);
            break;
    }
    ipl2QImage(cv_img_tmp);
}

void MainWindow::applyFilter()
{
    cv_img = cv_img_tmp.clone();
    widget->close();
}
