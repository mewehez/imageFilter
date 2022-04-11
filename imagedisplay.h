#ifndef IMAGEDISPLAY_H
#define IMAGEDISPLAY_H

#include <QDialog>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class ImageDisplay; }
QT_END_NAMESPACE

class ImageDisplay : public QDialog
{
    Q_OBJECT
public:
    ImageDisplay(QWidget *parent = nullptr);
    ~ImageDisplay();
    QLabel* getImageLabel();

private:
    Ui::ImageDisplay *ui;
};

#endif // IMAGEDISPLAY_H
