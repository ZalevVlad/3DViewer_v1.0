#include "color_pick.h"
#include "ui_color_pick.h"

Color_pick::Color_pick(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Color_pick)
{
    ui->setupUi(this);
    this->setWindowTitle("Color pick");
}

Color_pick::~Color_pick()
{
    delete ui;
}

void Color_pick::recieve_color(QColor* pcolor){
    this->pcolor = pcolor;
    QPalette pal = QPalette();
    pal.setColor(QPalette::Window, *pcolor);
    ui->widget_color->setPalette(pal);
    ui->widget_color->setAutoFillBackground(true);


    ui->spinBox_red->setValue(pcolor->red());
    ui->spinBox_green->setValue(pcolor->green());
    ui->spinBox_blue->setValue(pcolor->blue());

    ui->horizontalSlider_red->setValue(pcolor->red());
    ui->horizontalSlider_green->setValue(pcolor->green());
    ui->horizontalSlider_blue->setValue(pcolor->blue());
}

void Color_pick::on_buttonBox_accepted()
{
    *this->pcolor = newcolor;
}


void Color_pick::on_horizontalSlider_red_valueChanged(int value)
{
    ui->spinBox_red->setValue(value);
    change_color();
}


void Color_pick::on_horizontalSlider_green_valueChanged(int value)
{
    ui->spinBox_green->setValue(value);
    change_color();
}


void Color_pick::on_horizontalSlider_blue_valueChanged(int value)
{
    ui->spinBox_blue->setValue(value);
    change_color();
}

void Color_pick::change_color(){
    newcolor.setRed(ui->spinBox_red->value());
    newcolor.setGreen(ui->spinBox_green->value());
    newcolor.setBlue(ui->spinBox_blue->value());
    QPalette pal = QPalette();
    pal.setColor(QPalette::Window, newcolor);
    ui->widget_color->setPalette(pal);
}

void Color_pick::on_spinBox_red_valueChanged(int arg1)
{
    ui->horizontalSlider_red->setValue(arg1);
    change_color();
}


void Color_pick::on_spinBox_green_valueChanged(int arg1)
{
    ui->horizontalSlider_green->setValue(arg1);
    change_color();
}


void Color_pick::on_spinBox_blue_valueChanged(int arg1)
{
    ui->horizontalSlider_blue->setValue(arg1);
    change_color();
}

