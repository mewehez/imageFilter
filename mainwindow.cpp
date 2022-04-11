#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "imagedisplay.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::displayErrorMessage(const QString &message)
{
    QMessageBox errorMessageBox(this);
    errorMessageBox.setText(message);
    errorMessageBox.exec();
}

static FilterTypes qStringToFilter(const QString& filter)
{
    if(filter == "Box Filter")
        return FilterTypes::BOX_FILTER;
    if(filter == "Gaussian Filter")
        return FilterTypes::GAUSSIAN_FILTER;
    if(filter == "Median Filter")
        return FilterTypes::MEDIAN_FILTER;
    if(filter == "Bilateral Filter")
        return FilterTypes::BILATERAL_FILTER;
    return FilterTypes::UNKNOWN;
}

void MainWindow::on_inputButton_pressed()
{
    QString basePath = ui->imageInput->text();
    QFileInfo fileInfo(basePath);

    if(!basePath.isEmpty())
    {
        if(!fileInfo.isDir() && fileInfo.isFile())
            basePath = fileInfo.absolutePath();
        else {
            displayErrorMessage("The input path" + basePath + " should point to a valid folder or image file.");
            return;
        }
    }
    else // set base path to home
        basePath =  QDir::homePath();

    QString fileName = QFileDialog::getOpenFileName(this, "Select input file", basePath, "Image (*.jpg *.png *.bmp)");
    // if the file exist set into filepath input
    if(QFile::exists(fileName))
    {
        ui->imageInput->setText(fileName);
    }
}


void MainWindow::on_outputButton_pressed()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Select output image", QDir::homePath(), "Image (*.jpg *.png *.bmp)");
    if(!fileName.isEmpty())
    {
        ui->saveInput->setText(fileName);
    }
}


// run filter on image
void MainWindow::on_runFilterButton_pressed()
{
    // check input path ok
    QString basePath = ui->imageInput->text();
    QFileInfo fileInfo(basePath);

    if(!fileInfo.isFile())
    {
        displayErrorMessage("The input path" + basePath + " should point to an existing image file. Please make sure the file exists.");
        return;
    }

    // check output path ok
    basePath = ui->saveInput->text();
    fileInfo.setFile(basePath);

    if(!fileInfo.absoluteDir().exists())
    {
        displayErrorMessage("The output path" + basePath + " should point to existing folder.");
        return;
    }

    // read input image
    std::string inputImagePath = ui->imageInput->text().toStdString();
    cv::Mat inputImage = cv::imread(inputImagePath);

    // get the filter type
    QString filter = ui->filterComboBox->currentText();
    FilterTypes filterType = qStringToFilter(filter);

    cv::Mat outputImage; // output image
    // filter the image
    switch(filterType)
    {
    case FilterTypes::BOX_FILTER:
        qDebug("Calling Box Filter");
        cv::boxFilter(inputImage, outputImage, -1, cv::Size(5, 5));
        break;
    case FilterTypes::GAUSSIAN_FILTER:
        qDebug("Calling Gaussian Filter");
        cv::GaussianBlur(inputImage, outputImage, cv::Size(5, 5), 1.25);
        break;
    case FilterTypes::MEDIAN_FILTER:
        qDebug("Calling Median Filter");
        cv::medianBlur(inputImage, outputImage, 5);
        break;
    case FilterTypes::BILATERAL_FILTER:
        qDebug("Calling Bilateral Filter");
        cv::bilateralFilter(inputImage, outputImage, 15, 80, 80);
       break;
    default:
        displayErrorMessage("Unknown filter " + filter + ".");
        return;
    }

    // save the output image
    std::string outputImagePath = ui->saveInput->text().toStdString();
    cv::imwrite(outputImagePath, outputImage); // save the image

    // show the output image
    if(ui->showImageCheckBox->isChecked())
    {
        // create dialog to display output
        ImageDisplay dialog(this);
        int height = dialog.size().height();
        int width = dialog.size().width();

        cv::resize(outputImage, outputImage, cv::Size(width, height), 0, 0, cv::INTER_LINEAR);
        cv::cvtColor(outputImage, outputImage, CV_BGR2RGB); //Qt reads in RGB whereas CV in BGR
        //Converts the CV image into Qt standard format
        QImage imdisplay((uchar*)outputImage.data, outputImage.cols, outputImage.rows, outputImage.step, QImage::Format_RGB888);
        dialog.getImageLabel()->setPixmap(QPixmap::fromImage(imdisplay));//display the image in label that is created earlier
        dialog.exec();
    }
}

