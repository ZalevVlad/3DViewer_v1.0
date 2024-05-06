#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QFileDialog>
#include <QMessageBox>

#include <QOpenGLWidget>
#include <QTimer>
#include <QTime>

#include "openglwindow.h"
#include "color_pick.h"
#include "../../3rdParty/gifimage/qgifimage.h"

extern "C" {
#include "../../graphics/obj.h"

}

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

    void on_action_triggered();

    void on_pushButton_backgroundColor_clicked();

    void on_pushButton_edgeColor_clicked();

    void on_pushButton_vertexColor_clicked();

    void on_moveX_doubleSpinBox_valueChanged(double arg1);

    void on_moveY_doubleSpinBox_valueChanged(double arg1);

    void on_moveZ_doubleSpinBox_valueChanged(double arg1);

    void on_move_x_slider_valueChanged(int value);

    void on_move_y_slider_valueChanged(int value);

    void on_move_z_slider_valueChanged(int value);

    void on_rotateX_doubleSpinBox_valueChanged(double arg1);

    void on_rotateY_doubleSpinBox_valueChanged(double arg1);

    void on_rotateZ_doubleSpinBox_valueChanged(double arg1);

    void on_rotate_x_slider_valueChanged(int value);

    void on_rotate_y_slider_valueChanged(int value);

    void on_rotate_z_slider_valueChanged(int value);

    void on_scaleDoubleSpinBox_valueChanged(double arg1);

    void on_scale_slider_valueChanged(int value);

    void on_comboBox_projection_currentTextChanged(const QString &arg1);

    void on_comboBox_edge_type_currentTextChanged(const QString &arg1);

    void on_doubleSpinBox_edge_size_valueChanged(double arg1);

    void on_comboBox_vertex_type_currentTextChanged(const QString &arg1);

    void on_doubleSpinBox_vertex_size_valueChanged(double arg1);

    void on_checkBox_showAxes_clicked(bool checked);

    void on_pushButton_saveImage_clicked();
    void on_pushButton_saveGif_clicked();
    void recordGif_image();

private:
    Ui::MainWindow *ui;

private:
    void setParametersRanges();
    QString m_sSettingsFile; // Settings file path
    void load_settings();
    void save_settings();

    QTimer *timer;
    void makeGif(QVector<QImage> &images);

private:
    Color_pick color_pick_ui;
    QColor background_color;
    QColor vertex_color;
    QColor edge_color;

    s21_obj obj;
signals:
    void send_color(QColor* pcolor);

private:

};
#endif // MAINWINDOW_H
