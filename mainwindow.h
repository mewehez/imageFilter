#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_inputButton_pressed();
    void on_outputButton_pressed();
    void on_runFilterButton_pressed();

private:
    Ui::MainWindow *ui;
    void displayErrorMessage(const QString &message);
};

enum class  FilterTypes
{
    BOX_FILTER=0,
    GAUSSIAN_FILTER,
    MEDIAN_FILTER,
    BILATERAL_FILTER,
    UNKNOWN
};

#endif // MAINWINDOW_H
