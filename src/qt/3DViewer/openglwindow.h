#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <GL/gl.h>

extern "C"{
#include "../../graphics/obj.h"
#include "../../graphics/matrix_transform.h"
}

class OpenGLWindow: public QOpenGLWidget
{
    Q_OBJECT
public:
    OpenGLWindow(QWidget *parent = nullptr);
    ~OpenGLWindow();

    void set_object(s21_obj *obj);
    void object_to_arrays();

    void create_facet_arrays();
    void destroy_facet_arrays();

    void set_background_color(QColor background_color);
    void set_vertex_color(QColor vertex_color);
    void set_edge_color(QColor edge_color);

    void set_move_x(double move);
    void set_move_y(double move);
    void set_move_z(double move);

    void set_rotate_x(double rotate);
    void set_rotate_y(double rotate);
    void set_rotate_z(double rotate);

    void set_scale(double scale);

    void set_projection_type(int projection_type);
    void set_edge_type(int edge_type);
    void set_vertex_type(int vertex_type);

    void set_line_width(double line_width);
    void set_vertex_size(double vertex_size);

    void set_axes(bool is_checked);

    enum projection_types{
        PARALLEL,
        CENTRAL
    };

    enum edge_types{
        SOLID,
        DOTTED
    };

    enum vertex_types{
        NONE,
        CIRCLE,
        SQUARE
    };

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    s21_obj *obj;

    typedef struct facet_array_t{
        int len;
        int facets_num;
        GLuint* numbers; // Vertex numbers

    }facet_array_t;

//    QVector<facet_array_t> fArrsVec;

    QMap<int,facet_array_t> fArrsMap;

    double background_colorRd;
    double background_colorGd;
    double background_colorBd;
    double vertex_colorRd;
    double vertex_colorGd;
    double vertex_colorBd;
    double edge_colorRd;
    double edge_colorGd;
    double edge_colorBd;

    int projection_type;
    int edge_type;
    int vertex_type;

    double line_width;
    double vertex_size;

    double move_x;
    double move_y;
    double move_z;

    double rotate_x;
    double rotate_y;
    double rotate_z;

    double scale;

    bool axes;

    matrix_t matrix;
    GLdouble *matrix_str;

    double pcam_edge_size = 700; // Parallel projection cam size
    double pcam_dist = 2000; // Distantion to camera for parallel projection

    double ccam_edge_size = 2; // Central projection cam size
    double ccam_dist_x = 500; // Distantion to camera for central projection
    double ccam_dist_y = ccam_dist_x;
    double ccam_dist_z = ccam_dist_x*0.4;
    double x,y,z;
};

#endif // OPENGLWINDOW_H
