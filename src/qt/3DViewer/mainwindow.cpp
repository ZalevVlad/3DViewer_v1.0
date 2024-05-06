#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::setParametersRanges(){
    double moveXrange = 500;
    double moveYrange = moveXrange;
    double moveZrange = moveXrange;
    double moveSingleStep=0.1;
    ui->moveX_doubleSpinBox->setMinimum(-moveXrange);
    ui->moveX_doubleSpinBox->setMaximum(moveXrange);
    ui->moveX_doubleSpinBox->setSingleStep(moveSingleStep);

    ui->moveY_doubleSpinBox->setMinimum(-moveYrange);
    ui->moveY_doubleSpinBox->setMaximum(moveYrange);
    ui->moveY_doubleSpinBox->setSingleStep(moveSingleStep);

    ui->moveZ_doubleSpinBox->setMinimum(-moveZrange);
    ui->moveZ_doubleSpinBox->setMaximum(moveZrange);
    ui->moveZ_doubleSpinBox->setSingleStep(moveSingleStep);
    \

        ui->move_x_slider->setMinimum(-(int)(moveXrange/moveSingleStep));
    ui->move_x_slider->setMaximum((int)(moveXrange/moveSingleStep));

    ui->move_y_slider->setMinimum(-(int)(moveYrange/moveSingleStep));
    ui->move_y_slider->setMaximum((int)(moveYrange/moveSingleStep));

    ui->move_z_slider->setMinimum(-(int)(moveZrange/moveSingleStep));
    ui->move_z_slider->setMaximum((int)(moveZrange/moveSingleStep));
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_sSettingsFile = QApplication::applicationDirPath() + "/settings.ini";
    load_settings();

    obj = create_obj();

    setParametersRanges();
}

MainWindow::~MainWindow()
{
    save_settings();
    delete ui;
    free_obj(&obj);
}

void MainWindow::load_settings(){
    QSettings settings(m_sSettingsFile, QSettings::NativeFormat);
    QString sText;
    QStringList list;
    QPalette pal;
    sText = settings.value("projection", "0").toString();
    ui->comboBox_projection->setCurrentIndex(sText.toInt());
    on_comboBox_projection_currentTextChanged(ui->comboBox_projection->currentText());

    sText = settings.value("background_color", "0 0 0").toString();
    list = sText.split(" ");
    background_color.setRed(list[0].toInt());
    background_color.setGreen(list[1].toInt());
    background_color.setBlue(list[2].toInt());
    pal.setColor(QPalette::Button, background_color);
    ui->pushButton_backgroundColor->setPalette(pal);
    ui->glwindow->set_background_color(background_color); // Change gl window

    sText = settings.value("edge_type", "0").toString();
    ui->comboBox_edge_type->setCurrentIndex(sText.toInt());
    on_comboBox_edge_type_currentTextChanged(ui->comboBox_edge_type->currentText());

    sText = settings.value("edge_size", "").toString();
    if(sText.length()){ui->doubleSpinBox_edge_size->setValue(sText.toDouble());}
    on_doubleSpinBox_edge_size_valueChanged(ui->doubleSpinBox_edge_size->value());

    sText = settings.value("edge_color", "255 255 255").toString();
    list = sText.split(" ");
    edge_color.setRed(list[0].toInt());
    edge_color.setGreen(list[1].toInt());
    edge_color.setBlue(list[2].toInt());
    pal.setColor(QPalette::Button, edge_color);
    ui->pushButton_edgeColor->setPalette(pal);
    ui->glwindow->set_edge_color(edge_color);

    sText = settings.value("vertex_type", "0").toString();
    ui->comboBox_vertex_type->setCurrentIndex(sText.toInt());
    on_comboBox_vertex_type_currentTextChanged(ui->comboBox_vertex_type->currentText());

    sText = settings.value("vertex_size", "").toString();
    if(sText.length()){ui->doubleSpinBox_vertex_size->setValue(sText.toDouble());}
    on_doubleSpinBox_vertex_size_valueChanged(ui->doubleSpinBox_vertex_size->value());

    sText = settings.value("vertex_color", "255 0 0").toString();
    list = sText.split(" ");
    vertex_color.setRed(list[0].toInt());
    vertex_color.setGreen(list[1].toInt());
    vertex_color.setBlue(list[2].toInt());
    pal.setColor(QPalette::Button, vertex_color);
    ui->pushButton_vertexColor->setPalette(pal);
    ui->glwindow->set_vertex_color(vertex_color);

    ui->glwindow->set_axes(ui->checkBox_showAxes->isChecked());
}

void MainWindow::save_settings(){
    QSettings settings(m_sSettingsFile, QSettings::NativeFormat);
    QString sText;

    sText = QString::number(ui->comboBox_projection->currentIndex());
    settings.setValue("projection", sText);

    sText= QString::asprintf("%d %d %d", background_color.red(),background_color.green(), background_color.blue());
    settings.setValue("background_color", sText);

    sText = QString::number(ui->comboBox_edge_type->currentIndex());
    settings.setValue("edge_type", sText);

    sText = QString::number(ui->doubleSpinBox_edge_size->value());
    settings.setValue("edge_size", sText.toUtf8());

    sText= QString::asprintf("%d %d %d", edge_color.red(),edge_color.green(), edge_color.blue());
    settings.setValue("edge_color", sText);

    sText = QString::number(ui->comboBox_vertex_type->currentIndex());
    settings.setValue("vertex_type", sText);

    sText = QString::number(ui->doubleSpinBox_vertex_size->value());
    settings.setValue("vertex_size", sText.toUtf8());

    sText= QString::asprintf("%d %d %d", vertex_color.red(),vertex_color.green(), vertex_color.blue());
    settings.setValue("vertex_color", sText);
}

// Open file
void MainWindow::on_action_triggered()
{
    static QString last_directory = "/home/nicolsha/projects/school21/C/C8_3DViewer_v1.0-1/datasets/objs/";
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Object"), last_directory, tr("Object Files (*.obj)"));
    std::string strFileName = fileName.toStdString();
    const char *cStr = strFileName.c_str();

    free_obj(&obj); // In case more than one file opened, need to free previous objects
    ui->glwindow->set_object(NULL);

    this->setEnabled(false);
    this->repaint();
    int parse = parse_obj(cStr,&obj);
    if(parse==OK){
        ui->glwindow->set_object(&obj);
        this->setWindowTitle(fileName);
        ui->object_vertexes->setText(QString::number(obj.vertexes.len));

        int edges_num=0;
        for(int i=0; i<obj.facets.len;i++){
            int len = obj.facets.facets[i].len;
            if(len==2){
                edges_num+=1;
            }
            else{
                edges_num+=len;
            }
        }
        ui->object_edges->setText(QString::number(edges_num));
    }
    else{
        this->setWindowTitle("3DViewer");
        ui->object_vertexes->setText("None");
        ui->object_edges->setText("None");

        QMessageBox msgBox;
        msgBox.setText("Error while parsing obj file.");
        msgBox.setWindowTitle("ERROR");
        msgBox.exec();
    }
    this->setEnabled(true);
    this->repaint();
}

// Color change
void MainWindow::on_pushButton_backgroundColor_clicked()
{
    color_pick_ui.setModal(true);
    connect(this,&MainWindow::send_color,&color_pick_ui,&Color_pick::recieve_color);
    emit send_color(&background_color);
    color_pick_ui.exec();

    QPalette pal;
    pal.setColor(QPalette::Button, background_color);
    ui->pushButton_backgroundColor->setPalette(pal);

    ui->glwindow->set_background_color(background_color);
}
void MainWindow::on_pushButton_edgeColor_clicked()
{
    color_pick_ui.setModal(true);
    connect(this,&MainWindow::send_color,&color_pick_ui,&Color_pick::recieve_color);
    emit send_color(&edge_color);
    color_pick_ui.exec();

    QPalette pal;
    pal.setColor(QPalette::Button, edge_color);
    ui->pushButton_edgeColor->setPalette(pal);

    ui->glwindow->set_edge_color(edge_color);
}
void MainWindow::on_pushButton_vertexColor_clicked()
{
    color_pick_ui.setModal(true);
    connect(this,&MainWindow::send_color,&color_pick_ui,&Color_pick::recieve_color);
    emit send_color(&vertex_color);
    color_pick_ui.exec();

    QPalette pal;
    pal.setColor(QPalette::Button, vertex_color);
    ui->pushButton_vertexColor->setPalette(pal);

    ui->glwindow->set_vertex_color(vertex_color);
}

// Move spin box changed
void MainWindow::on_moveX_doubleSpinBox_valueChanged(double arg1)
{
    ui->move_x_slider->setValue((int)(arg1/ui->moveX_doubleSpinBox->singleStep()));
    ui->glwindow->set_move_x(ui->moveX_doubleSpinBox->value());
}
void MainWindow::on_moveY_doubleSpinBox_valueChanged(double arg1)
{
    ui->move_y_slider->setValue((int)(arg1/ui->moveY_doubleSpinBox->singleStep()));
    ui->glwindow->set_move_y(ui->moveY_doubleSpinBox->value());
}
void MainWindow::on_moveZ_doubleSpinBox_valueChanged(double arg1)
{
    ui->move_z_slider->setValue((int)(arg1/ui->moveZ_doubleSpinBox->singleStep()));
    ui->glwindow->set_move_z(ui->moveZ_doubleSpinBox->value());
}

// Move slider changed
void MainWindow::on_move_x_slider_valueChanged(int value)
{
    ui->moveX_doubleSpinBox->setValue(value*ui->moveX_doubleSpinBox->singleStep());
    ui->glwindow->set_move_x(ui->moveX_doubleSpinBox->value());
}
void MainWindow::on_move_y_slider_valueChanged(int value)
{
    ui->moveY_doubleSpinBox->setValue(value*ui->moveY_doubleSpinBox->singleStep());
    ui->glwindow->set_move_y(ui->moveY_doubleSpinBox->value());
}
void MainWindow::on_move_z_slider_valueChanged(int value)
{
    ui->moveZ_doubleSpinBox->setValue(value*ui->moveZ_doubleSpinBox->singleStep());
    ui->glwindow->set_move_z(ui->moveZ_doubleSpinBox->value());
}

// Rotate spin box changed
void MainWindow::on_rotateX_doubleSpinBox_valueChanged(double arg1)
{
    ui->rotate_x_slider->setValue((int)(arg1/ui->rotateX_doubleSpinBox->singleStep()));
    ui->glwindow->set_rotate_x(ui->rotateX_doubleSpinBox->value());
}
void MainWindow::on_rotateY_doubleSpinBox_valueChanged(double arg1)
{
    ui->rotate_y_slider->setValue((int)(arg1/ui->rotateY_doubleSpinBox->singleStep()));
    ui->glwindow->set_rotate_y(ui->rotateY_doubleSpinBox->value());
}
void MainWindow::on_rotateZ_doubleSpinBox_valueChanged(double arg1)
{
    ui->rotate_z_slider->setValue((int)(arg1/ui->rotateZ_doubleSpinBox->singleStep()));
    ui->glwindow->set_rotate_z(ui->rotateZ_doubleSpinBox->value());
}


// Rotate slider changed
void MainWindow::on_rotate_x_slider_valueChanged(int value)
{
    ui->rotateX_doubleSpinBox->setValue(value*ui->rotateX_doubleSpinBox->singleStep());
    ui->glwindow->set_rotate_x(ui->rotateX_doubleSpinBox->value());
}
void MainWindow::on_rotate_y_slider_valueChanged(int value)
{
    ui->rotateY_doubleSpinBox->setValue(value*ui->rotateY_doubleSpinBox->singleStep());
    ui->glwindow->set_rotate_y(ui->rotateY_doubleSpinBox->value());
}
void MainWindow::on_rotate_z_slider_valueChanged(int value)
{
    ui->rotateZ_doubleSpinBox->setValue(value*ui->rotateZ_doubleSpinBox->singleStep());
    ui->glwindow->set_rotate_z(ui->rotateZ_doubleSpinBox->value());
}

// Scale changed
void MainWindow::on_scaleDoubleSpinBox_valueChanged(double arg1)
{
    ui->scale_slider->setValue((int)(arg1/ui->scaleDoubleSpinBox->singleStep()));
    ui->glwindow->set_scale(ui->scaleDoubleSpinBox->value());
}
void MainWindow::on_scale_slider_valueChanged(int value)
{
    ui->scaleDoubleSpinBox->setValue(value*ui->scaleDoubleSpinBox->singleStep());
    ui->glwindow->set_scale(ui->scaleDoubleSpinBox->value());
}


void MainWindow::on_comboBox_projection_currentTextChanged(const QString &arg1)
{
    if(arg1.toStdString()=="parallel"){ui->glwindow->set_projection_type(OpenGLWindow::PARALLEL);}
    else if(arg1.toStdString()=="central"){ui->glwindow->set_projection_type(OpenGLWindow::CENTRAL);}
}


void MainWindow::on_comboBox_edge_type_currentTextChanged(const QString &arg1)
{
    if(arg1.toStdString()=="solid"){ui->glwindow->set_edge_type(OpenGLWindow::SOLID);}
    else if(arg1.toStdString()=="dotted"){ui->glwindow->set_edge_type(OpenGLWindow::DOTTED);}
}


void MainWindow::on_doubleSpinBox_edge_size_valueChanged(double arg1)
{
    ui->glwindow->set_line_width(arg1);
}


void MainWindow::on_comboBox_vertex_type_currentTextChanged(const QString &arg1)
{
    if(arg1.toStdString()=="none"){ui->glwindow->set_vertex_type(OpenGLWindow::NONE);}
    else if(arg1.toStdString()=="circle"){ui->glwindow->set_vertex_type(OpenGLWindow::CIRCLE);}
    else if(arg1.toStdString()=="square"){ui->glwindow->set_vertex_type(OpenGLWindow::SQUARE);}
}


void MainWindow::on_doubleSpinBox_vertex_size_valueChanged(double arg1)
{
    ui->glwindow->set_vertex_size(arg1);
}


void MainWindow::on_checkBox_showAxes_clicked(bool checked)
{
    ui->glwindow->set_axes(checked);
}


void MainWindow::on_pushButton_saveImage_clicked()
{
    QString file = QFileDialog::getSaveFileName(this, "Save as...", "image", "BMP (*.bmp);; JPEG (*.jpeg)");
    ui->glwindow->grab().save(file, NULL, 100);
}


void MainWindow::on_pushButton_saveGif_clicked()
{
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(recordGif_image()));
    timer->start(100);
    ui->pushButton_saveGif->setDisabled(true);

}

void MainWindow::recordGif_image() {

    static int count = 0;
    static QVector<QImage> images;
    int record_duartion_ms=5000;
    if (count < record_duartion_ms) {
        images.append(ui->glwindow->grabFramebuffer().scaled(640, 480, Qt::IgnoreAspectRatio));

        ui->pushButton_saveGif->setText(QString::number((record_duartion_ms-count)/1000)+"s...");
        count+=100;
    }
    if (count == record_duartion_ms) {
        timer->stop();
        count = 0;
        makeGif(images);
        images.clear();
    }
}

void MainWindow::makeGif(QVector<QImage> &images) {
    QString file = QFileDialog::getSaveFileName(this, "Save as...", "render", "GIF (*.gif)");
    if (!file.isNull()) {
        QGifImage gif(QSize(640,480));
        gif.setDefaultDelay(100);
        for(int i=0; i<images.size();i++){
            gif.addFrame(images[i]);
        }
        gif.save(file);
    }

    ui->pushButton_saveGif->setDisabled(false);
    ui->pushButton_saveGif->setText("Record GIF");
}
