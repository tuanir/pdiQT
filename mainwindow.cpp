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

    //QShortcut *shortcutOpen = new QShortcut(QKeySequence("Ctrl+O"),this);
    //QShortcut *shortcutQuit = new QShortcut(QKeySequence("Ctrl+Q"),this);

    // Connect the signals with the slots

    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openAction()));
    //connect(shortcutOpen, SIGNAL(activated()), this, SLOT(openAction()));

    connect(ui->actionSair, SIGNAL(triggered()), qApp, SLOT(quit()));
    //connect(shortcutQuit, SIGNAL(activated()), qApp, SLOT(quit()));

    connect(ui->actionDesfazer, SIGNAL(triggered()), this, SLOT(undo()));
    connect(ui->actionRefazer, SIGNAL(triggered()), this, SLOT(redo()));

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
    connect(ui->actionDetect_Circles, SIGNAL(triggered()),this,SLOT(actionDetect_Circles()));

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
    lastPath = getenv("HOME");

    // PopUp settings
    widget = new QWidget;
    gridLayout = new QGridLayout;
    ok = new QPushButton("Ok");
    slider = new QSlider(Qt::Horizontal);
    slider->setMinimum(1);
    slider->setMaximum(30);
    gridLayout->addWidget(slider);
    gridLayout->addWidget(ok);
    widget->setLayout(gridLayout);
    widget->setWindowTitle("Parâmetro");
    connect(slider, SIGNAL(valueChanged(int)),this, SLOT(setFilter(int)));
    connect(ok, SIGNAL(clicked()), this, SLOT(applyFilter()));

    // Main Window settings
    this->setWindowTitle("PDI");

    // History settings
    currentHistory = history.begin();
    HISTORY_MAX_SIZE = 20;
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
        historyAdd();
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
    QString savePath = QFileDialog::getSaveFileName(this,"Save Image File", *fileName, tr("Images (*.png *.jpg)"));
    if (cv_img.data)
    {
        vector<int> compression_params;
        compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
        compression_params.push_back(0);
        try
        {
            imwrite(savePath.toStdString(), cv_img, compression_params);
        } catch (runtime_error& ex)
        {
            fprintf(stderr, "Exception converting image to PNG format: %s\n", ex.what());
            return;
        }
    }

}

void MainWindow::actionHistograma()
{
        //TODO
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

void MainWindow::actionDetect_Circles()
{
    if(cv_img.data)
    {
        //convert it to gray
        actionTons_de_cinza();

        GaussianBlur(cv_img, cv_img_tmp, Size(9,9), 2, 2);
        cv_img = cv_img_tmp.clone();

        vector<Vec3f> circles;

        HoughCircles(cv_img, circles, CV_HOUGH_GRADIENT, 1, cv_img.rows/8, 200, 100, 0, 0);

        // Draw the circles

        for(size_t i = 0; i < circles.size(); i++)
        {
            Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            int radius = cvRound(circles[i][2]);

            //circle center
            circle(cv_img, center, 3, Scalar(0,255,0), -1, 8, 0);

            //circle outline
            circle(cv_img, center, radius, Scalar(0,0,255), 3, 8, 0);
        }

        ipl2QImage(cv_img);
        cout << circles.size() << endl;
        historyAdd();
    }
}

void MainWindow::actionLaplaciano()
{
    if(cv_img.data)
    {
        int kernel_size = 3;
        int scale = 1;
        int delta = 0;
        int ddepth = CV_16S;

        /// Remove noise by blurring with a Gaussian filter

        GaussianBlur( cv_img, cv_img_tmp, Size(3,3), 0, 0, BORDER_DEFAULT );
        cv_img = cv_img_tmp.clone();

        /// Convert the image to grayscale
        actionTons_de_cinza();

        /// Apply Laplace function
        Laplacian( cv_img, cv_img_tmp, ddepth, kernel_size, scale, delta, BORDER_DEFAULT );

        cv_img = cv_img_tmp.clone();

        convertScaleAbs( cv_img, cv_img_tmp );
        cv_img = cv_img_tmp.clone();

        ipl2QImage(cv_img);
        historyAdd();
    }
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
        historyAdd();
    }
}

void MainWindow::actionLimiarizacao()
{
    if(cv_img.data)
    {
        actionTons_de_cinza();
        threshold(cv_img, cv_img_tmp, 128, 255, THRESH_BINARY);
        cv_img = cv_img_tmp.clone();

        ipl2QImage(cv_img);
        historyAdd();
    }
}

void MainWindow::actionLimiarizacao_automatica()
{
    if(cv_img.data)
    {
        actionTons_de_cinza();
        adaptiveThreshold(cv_img, cv_img_tmp, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 15, 1);
        cv_img = cv_img_tmp.clone();

        ipl2QImage(cv_img);
        historyAdd();
    }
}

void MainWindow::actionOtsu()
{
    if(cv_img.data)
    {
        actionTons_de_cinza();
        threshold(cv_img, cv_img_tmp, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
        cv_img = cv_img_tmp.clone();

        ipl2QImage(cv_img);
        historyAdd();
    }
}

void MainWindow::actionWatershed()
{
    if(cv_img.data)
    {
        Mat gray, otsu, eroded, dilated, bg;

        cvtColor(cv_img,gray,CV_RGB2GRAY);
        threshold(gray, otsu, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
        erode(otsu, eroded, cv::Mat(), cv::Point(-1,-1), 2);
        dilate(otsu, dilated, cv::Mat(), cv::Point(-1,-1), 3);
        threshold(dilated,bg,1,128,cv::THRESH_BINARY_INV);

        Mat markers(otsu.size(),CV_8U,cv::Scalar(0));
        markers = eroded + bg;
        markers.convertTo(markers, CV_32S);
        watershed(cv_img, markers);
        markers.convertTo(markers,CV_8U);

        cv_img = markers.clone();
        format = QImage::Format_Indexed8;
        ipl2QImage(cv_img);
        historyAdd();
    }
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
    historyAdd();
}

void MainWindow::historyAdd()
{
    if (history.size() >= HISTORY_MAX_SIZE)
        history.pop_front();
    history.push_back(make_pair(cv_img,format));
    currentHistory = history.end();
    --currentHistory;
}

void MainWindow::undo()
{
    if(cv_img.data)
    {
        if (currentHistory != history.begin())
            --currentHistory;
        cv_img = (*currentHistory).first;
        format = (*currentHistory).second;
        ipl2QImage(cv_img);
    }
}

void MainWindow::redo()
{
    if(cv_img.data)
    {
        if (currentHistory != history.end())
            ++currentHistory;
        if (currentHistory == history.end())
            --currentHistory;
        cv_img = (*currentHistory).first;
        format = (*currentHistory).second;
        ipl2QImage(cv_img);
    }
}
