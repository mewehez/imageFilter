#include "imagedisplay.h"
#include "./ui_image_display.h"

ImageDisplay::ImageDisplay(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ImageDisplay)
{
    ui->setupUi(this);
    setFixedSize(size());
    ui->mainHLayout->setContentsMargins(0, 0, 0, 0);
}

ImageDisplay::~ImageDisplay()
{
    delete ui;
}

QLabel* ImageDisplay::getImageLabel()
{
    return ui->displayLabel;
}
