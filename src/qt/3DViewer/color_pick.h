#ifndef COLOR_PICK_H
#define COLOR_PICK_H

#include <QDialog>

namespace Ui {
class Color_pick;
}

class Color_pick : public QDialog
{
    Q_OBJECT

public:
    explicit Color_pick(QWidget *parent = nullptr);
    ~Color_pick();

public slots:
    void recieve_color(QColor* pcolor);

private slots:
    void on_buttonBox_accepted();

    void on_horizontalSlider_red_valueChanged(int value);
    void change_color();


    void on_horizontalSlider_green_valueChanged(int value);

    void on_horizontalSlider_blue_valueChanged(int value);

    void on_spinBox_red_valueChanged(int arg1);

    void on_spinBox_green_valueChanged(int arg1);

    void on_spinBox_blue_valueChanged(int arg1);

private:
    Ui::Color_pick *ui;
    QColor *pcolor;
    QColor newcolor;
};

#endif // COLOR_PICK_H
