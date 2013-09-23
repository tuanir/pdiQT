#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <qimage.h>
#include <QImage>
#include <QAction>
#include <QFileDialog>
#include <QGridLayout>
#include <QSlider>
#include <list>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();    

private slots:
    void openAction();
    void actionSalvar();
    void actionHistograma();
    void actionGaussiano();
    void actionLaplaciano();
    void actionMediana();
    void actionMedia();
    void actionTons_de_cinza();
    void actionLimiarizacao();
    void actionLimiarizacao_automatica();
    void actionOtsu();
    void actionWatershed();

    void setFilter(int i);
    void applyFilter();
    //void closeFilter();

    void historyAdd();
    void undo();
    void redo();

private:
    int currentFilter; //1 - median, 2 - normalized box filter (media), 3 - Gaussian
    void ipl2QImage(cv::Mat image);

    Ui::MainWindow *ui;

    QGraphicsScene *scene;
    QGraphicsPixmapItem *item;
    QPixmap *image;
    QString *fileName;
    QString lastPath;

    cv::Mat cv_img;
    cv::Mat cv_img_tmp;
    QImage::Format format;

    QWidget *widget;
    QGridLayout *gridLayout;
    QPushButton *ok;
    QSlider *slider;

    std::list<std::pair<cv::Mat,QImage::Format> > history;
    std::list<std::pair<cv::Mat,QImage::Format> >::iterator currentHistory;
    unsigned int HISTORY_MAX_SIZE;


};

#endif // MAINWINDOW_H
