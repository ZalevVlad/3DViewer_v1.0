#include "openglwindow.h"
#include "QOpenGLFunctions_3_1"

OpenGLWindow::OpenGLWindow(QWidget *parent) : QOpenGLWidget { parent }
{
    s21_create_matrix(4,4,&matrix);
    matrix_str=(GLdouble *)calloc(16,sizeof(GLdouble));
    obj = nullptr;

    move_x = 0;
    move_y = 0;
    move_z = 0;

    rotate_x = 0;
    rotate_y = 0;
    rotate_z = 0;

    scale = 1;

    projection_type=CENTRAL;
    edge_type=SOLID;
    vertex_type=NONE;

    line_width = 1;
    vertex_size = 1;
}

OpenGLWindow::~OpenGLWindow()
{
    s21_remove_matrix(&matrix);
    destroy_facet_arrays();
}

/* Установить матричную строку единичной*/
void matrixStr_setIdentity(double* matrix_str){
    for(int i=0; i<16;i++){
        matrix_str[i]=0;
    }
    for(int i=0; i<4; i++){
        matrix_str[4*i+i]=1.;
    }
}

// Копировать matrix_t в matrix_str
void matrixT_to_matrixStr(matrix_t matrixT, double* matrixStr){
    for(int i=0; i<4; i++){
        for (int j=0; j<4; j++){
            matrixStr[i*4+j]=matrixT.matrix[j][i];
        }
    }
}

void OpenGLWindow::initializeGL()
{
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    repaint();
}

void OpenGLWindow::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
}

void paint_cube(){
    glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
        // Top face (y = 1.0f)
        // Define vertices in counter-clockwise (CCW) order with normal pointing out
    glColor3f(0.0f, 1.0f, 0.0f);     // Green
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    // Bottom face (y = -1.0f)
    glColor3f(1.0f, 0.5f, 0.0f);     // Orange
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    // Front face  (z = 1.0f)
    glColor3f(1.0f, 0.0f, 0.0f);     // Red
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    // Back face (z = -1.0f)
    glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    // Left face (x = -1.0f)
    glColor3f(0.0f, 0.0f, 1.0f);     // Blue
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    // Right face (x = 1.0f)
    glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glEnd();  // End of drawing color-cube
}

void paint_coordinate_lines(){
    // Coordinate Lines
    double size = 1000;
    glDisable(GL_LINE_STIPPLE);
    glDisable(GL_POINT_SMOOTH);
    glPointSize(1);
    glLineWidth(1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glBegin(GL_LINES);
    glColor3d(1,0,0);
    glVertex3d(0,0,0);
    glVertex3d(size,0,0);
    glColor3d(0,1,0);
    glVertex3d(0,0,0);
    glVertex3d(0,size,0);
    glColor3d(0,0,1);
    glVertex3d(0,0,0);
    glVertex3d(0,0,size);
    glEnd();
    glBegin(GL_POINTS);
    glColor3d(1,0,0);
    glVertex3d(size,0,0);
    glColor3d(0,1,0);
    glVertex3d(0,size,0);
    glColor3d(0,0,1);
    glVertex3d(0,0,size);
    glEnd();
}

void OpenGLWindow::paintGL()
{
    glClearColor(background_colorRd,background_colorGd,background_colorBd,1.);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



    // Cam set
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if(projection_type==PARALLEL){
        glOrtho(-pcam_edge_size,pcam_edge_size,-pcam_edge_size,pcam_edge_size,1,pcam_dist*2);

        glTranslated(0,0,-pcam_dist);
        glRotated(-90,1,0,0);
    }
    else if(projection_type==CENTRAL){
        double ccam_dist = sqrt(ccam_dist_x*ccam_dist_x+ccam_dist_y*ccam_dist_y+ccam_dist_z*ccam_dist_z);
        glFrustum(-ccam_edge_size,ccam_edge_size,-ccam_edge_size,ccam_edge_size,2,ccam_dist*2);

        double angle = sin(ccam_dist_z/ccam_dist)*90;
        glRotated(-(90-angle),1,0,0);
        glRotated(-135,0,0,1);
        glTranslated(-ccam_dist_x,-ccam_dist_y,-ccam_dist_z);

    }

    if(axes){paint_coordinate_lines();}


    // Object transformations
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    matrix_transform_move(&matrix,{move_x,move_y,move_z,1.}); // Translate
    matrixT_to_matrixStr(matrix,matrix_str);
    glMultMatrixd(matrix_str);
    matrix_transform_rotate_x(&matrix,rotate_x/180*M_PI); // Rotate x
    matrixT_to_matrixStr(matrix,matrix_str);
    glMultMatrixd(matrix_str);
    matrix_transform_rotate_y(&matrix,rotate_y/180*M_PI); // Rotate x
    matrixT_to_matrixStr(matrix,matrix_str);
    glMultMatrixd(matrix_str);
    matrix_transform_rotate_z(&matrix,rotate_z/180*M_PI); // Rotate x
    matrixT_to_matrixStr(matrix,matrix_str);
    glMultMatrixd(matrix_str);
    matrix_transform_scale(&matrix, scale); // Scale
    matrixT_to_matrixStr(matrix,matrix_str);
    glMultMatrixd(matrix_str);

    // Edges paint
    if(edge_type==SOLID){glDisable(GL_LINE_STIPPLE);}
    else if(edge_type==DOTTED){
            glEnable(GL_LINE_STIPPLE);
            glLineStipple(1,0x000f);
    }
    glColor3d(edge_colorRd,edge_colorGd,edge_colorBd);
    glLineWidth(line_width);
#define PAINT_IN_LOOP 0
#if PAINT_IN_LOOP
// Mega slow
    if (obj!=nullptr && line_width!=0){
        for (int i=0; i<obj->facets.len; i++){
            glBegin(GL_LINE_LOOP);
            for (int j=0; j<obj->facets.facets[i].len; j++){
                x = obj->vertexes.vertexes[obj->facets.facets[i].numbers[j]-1].x;
                y = obj->vertexes.vertexes[obj->facets.facets[i].numbers[j]-1].y;
                z = obj->vertexes.vertexes[obj->facets.facets[i].numbers[j]-1].z;
                glVertex3d(x,y,z);
            }
            glEnd();
        }
    }
#else
// Fast
    if (obj!=nullptr && line_width!=0){
            s21_vertex* vertecies = obj->vertexes.vertexes;;
            s21_vertex v;
            int shift;
            glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
            glVertexPointer(4, GL_DOUBLE, 0, (GLvoid*)obj->vertexes.vertexes);
            glEnableClientState(GL_VERTEX_ARRAY);
            for (QMap<int,facet_array_t> :: iterator it = fArrsMap.begin(); it!=fArrsMap.end(); it++){
                if(it.key()==2){
                    glDrawElements(GL_LINES,it.value().len,GL_UNSIGNED_INT,(GLvoid*)it.value().numbers);
                }
                else if(it.key()==3){
                    glDrawElements(GL_TRIANGLES,it.value().len,GL_UNSIGNED_INT,(GLvoid*)it.value().numbers);
                }
//                else if(it.key()==4){
//                    glDrawElements(GL_QUADS,it.value().len,GL_UNSIGNED_INT,(GLvoid*)it.value().numbers);
//                }
                else{
                    GLuint* nums = it.value().numbers;;
                    for(int i=0; i<it.value().facets_num; i++){
                        shift = i*it.key();
                        glBegin(GL_LINE_LOOP);
                        for(int j=0; j<it.key(); j++){
                            v = vertecies[nums[shift+j]];
                            glVertex3dv((GLdouble*)&v);
                        }
                        glEnd();
                    }
                }
            }
            glDisableClientState(GL_VERTEX_ARRAY);
    }

// Slow
//    if (obj!=nullptr && line_width!=0){
//        GLuint* nums;
//        s21_vertex* vertecies = obj->vertexes.vertexes;;
//        int shift;
//        for (QMap<int,facet_array_t> :: iterator it = fArrsMap.begin(); it!=fArrsMap.end(); it++){

//            nums = it.value().numbers;
//            if(it.key()==3){
//                glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
//                glBegin(GL_TRIANGLES);
//                for(int i=0; i<it.value().len; i++) {
//                    glVertex3d(vertecies[nums[i]].x,
//                               vertecies[nums[i]].y,
//                               vertecies[nums[i]].z);}
//                glEnd();
//            }else{
//                for(int i=0; i<it.value().facets_num; i++){
//                    shift = i*it.key();
//                    glBegin(GL_LINE_LOOP);
//                    for(int j=0; j<it.key(); j++){
//                        glVertex3d(vertecies[nums[shift+j]].x,
//                                   vertecies[nums[shift+j]].y,
//                                   vertecies[nums[shift+j]].z);
//                    }
//                    glEnd();
//                }
//            }
//        }
//    }
#endif

    // Vertex paint
    if(vertex_type==NONE){glDisable(GL_POINT_SMOOTH);}
    if(vertex_type==CIRCLE){glEnable(GL_POINT_SMOOTH);}
    if(vertex_type==SQUARE){glDisable(GL_POINT_SMOOTH);}
    glColor3d(vertex_colorRd,vertex_colorGd,vertex_colorBd);
    glPointSize(vertex_size);
    if(obj!=nullptr&& vertex_size!=0 && vertex_type!=NONE){
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(4, GL_DOUBLE, 0, (GLvoid*)obj->vertexes.vertexes);
        glDrawArrays(GL_POINTS,0,obj->vertexes.len);
        glDisableClientState(GL_VERTEX_ARRAY);
    }

    // Cube
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//    glColor3d(1,1,1);
//    paint_cube();
}

void OpenGLWindow::set_object(s21_obj *obj){
    this->obj=obj;
    destroy_facet_arrays();
    if(obj){
        object_to_arrays();
    }
    repaint();
}
// Optimization of an object by partitioning it into arrays of indices
// for different numbers of vertices on the surface.
void OpenGLWindow::object_to_arrays(){
//    QHash<int,int> len_counts; // < count of vertecies in facet, amount of facets >
    int vs_n; // Vertecies num
    if(obj->facets.facets){
        for( int i=0; i<obj->facets.len;i++){
            vs_n = obj->facets.facets[i].len; // Length of facet
            if(fArrsMap.contains(vs_n)){
                fArrsMap[vs_n].facets_num++;
            } else {
                fArrsMap.insert(vs_n,{.len=0,.facets_num=1,.numbers=NULL});
            }
        }

        create_facet_arrays();

        for( int i=0; i<obj->facets.len;i++){
            vs_n = obj->facets.facets[i].len;
            for(int j=0; j<vs_n; j++){
                fArrsMap[vs_n].numbers[fArrsMap[vs_n].len]=(GLuint)(obj->facets.facets[i].numbers[j]-1);
                fArrsMap[vs_n].len++;
            }
        }
    }
}

void OpenGLWindow::create_facet_arrays(){
    int vertecies_num;
    for (QMap<int,facet_array_t> :: iterator it = fArrsMap.begin(); it!=fArrsMap.end(); it++){
        vertecies_num = it.key();
        fArrsMap[vertecies_num].numbers=(GLuint*)calloc(vertecies_num*fArrsMap[vertecies_num].facets_num,sizeof(GLuint));
        assert(fArrsMap[vertecies_num].numbers!=nullptr);
    }
}
void OpenGLWindow::destroy_facet_arrays(){
    int vertecies_num;
    for (QMap<int,facet_array_t> :: iterator it = fArrsMap.begin(); it!=fArrsMap.end(); it++){
        vertecies_num = it.key();
        if(fArrsMap[vertecies_num].numbers){
            free(fArrsMap[vertecies_num].numbers);
        }
    }
    fArrsMap.clear();
}

// Change colors
void OpenGLWindow::set_background_color(QColor background_color){
    background_colorRd=background_color.redF();
    background_colorGd=background_color.greenF();
    background_colorBd=background_color.blueF();
    repaint();
}
void OpenGLWindow::set_vertex_color(QColor vertex_color){
    vertex_colorRd=vertex_color.redF();
    vertex_colorGd=vertex_color.greenF();
    vertex_colorBd=vertex_color.blueF();
    repaint();
}
void OpenGLWindow::set_edge_color(QColor edge_color){
    edge_colorRd=edge_color.redF();
    edge_colorGd=edge_color.greenF();
    edge_colorBd=edge_color.blueF();
    repaint();
}

// Change translate
void OpenGLWindow::set_move_x(double move){
    this->move_x=move;
    repaint();
}
void OpenGLWindow::set_move_y(double move){
    this->move_y=move;
    repaint();
}
void OpenGLWindow::set_move_z(double move){
    this->move_z=move;
    repaint();
}

// Change rotate
void OpenGLWindow::set_rotate_x(double rotate){
    this->rotate_x=rotate;
    repaint();
}
void OpenGLWindow::set_rotate_y(double rotate){
    this->rotate_y=rotate;
    repaint();
}
void OpenGLWindow::set_rotate_z(double rotate){
    this->rotate_z=rotate;
    repaint();
}

// Change scale
void OpenGLWindow::set_scale(double scale){
    this->scale=scale;
    repaint();
}

void OpenGLWindow::set_projection_type(int projection_type){
    this->projection_type=projection_type;
    repaint();
}
void OpenGLWindow::set_edge_type(int edge_type){
    this->edge_type=edge_type;
    repaint();
}
void OpenGLWindow::set_vertex_type(int vertex_type){
    this->vertex_type=vertex_type;
    repaint();
}


void OpenGLWindow::set_line_width(double line_width){
    this->line_width=line_width;
    repaint();
}
void OpenGLWindow::set_vertex_size(double vertex_size){
    this->vertex_size=vertex_size;
    repaint();
}

void OpenGLWindow::set_axes(bool is_checked){
    axes=is_checked;
    repaint();
}
