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

private:
    void ipl2QImage();

    Ui::MainWindow *ui;

    QGraphicsScene *scene;
    QGraphicsPixmapItem *item;
    QPixmap *image;
    QString *fileName;
    QString lastPath;

    cv::Mat cv_img;
    cv::Mat cv_img_tmp;
    QImage::Format format;
};

#endif // MAINWINDOW_H
