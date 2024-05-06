#include "obj_transform.h"

// Геометрический центр объекта
#define EPS 1e-15
s21_vertex obj_center(s21_obj obj) {
    // Алогритм Вайсфельда
    s21_vertex y0 = {1, 1, 1, 0};
    s21_vertex y1 = {2 + 1e-10, 2 + 1e-10, 2 + 1e-10, 0};

    double s;
    while (fabs(s21_vertex_norm(y1) - s21_vertex_norm(y0)) > EPS) {
        y0 = y1;

        y1.x = 0;
        y1.y = 0;
        y1.z = 0;

        s = 0;
        for (int i = 0; i < obj.vertexes.len; i++) {
            y1 = s21_vertex_add(
                y1, s21_vertex_div_num(obj.vertexes.vertexes[i],
                                       s21_vertex_norm(s21_vertex_diff(
                                           obj.vertexes.vertexes[i], y0))));
            s += 1. /
                 s21_vertex_norm(s21_vertex_diff(obj.vertexes.vertexes[i], y0));
        }
        y1 = s21_vertex_div_num(y1, s);
    }
    return y1;
}

#define A transform->matrix
#define b obj->vertexes.vertexes[i]
int obj_transform(matrix_t* transform, s21_obj* obj) {
    s21_vertex res;
    for (int i = 0; i < obj->vertexes.len; i++) {
        res.x = A[0][0] * b.x + A[0][1] * b.y + A[0][2] * b.z + A[0][3] * b.w;
        res.y = A[1][0] * b.x + A[1][1] * b.y + A[1][2] * b.z + A[1][3] * b.w;
        res.z = A[2][0] * b.x + A[2][1] * b.y + A[2][2] * b.z + A[2][3] * b.w;
        res.w = A[3][0] * b.x + A[3][1] * b.y + A[3][2] * b.z + A[3][3] * b.w;
        b.x = res.x;
        b.y = res.y;
        b.z = res.z;
        b.w = res.w;
    }
    return 0;
}

#define T_MATRIX_SIZE 4
int obj_transform_(s21_obj* obj, s21_vertex move, double scale, double rotate_x,
                   double rotate_y, double rotate_z) {
    matrix_t M1, M2, M3;
    s21_create_matrix(T_MATRIX_SIZE, T_MATRIX_SIZE, &M1);
    s21_create_matrix(T_MATRIX_SIZE, T_MATRIX_SIZE, &M2);
    s21_create_matrix(T_MATRIX_SIZE, T_MATRIX_SIZE, &M3);
    s21_vertex center = obj_center(*obj);

    matrix_transform_move(&M1, s21_vertex_neg(center));
    matrix_transform_rotate_x(&M2, rotate_x);
    matrix_transform_mult(&M2, &M1, &M3);  // M3 = M2*M1
    matrix_transform_rotate_y(&M2, rotate_y);
    matrix_transform_mult(&M2, &M3, &M1);  // M1 = M2*M3
    matrix_transform_rotate_z(&M2, rotate_z);
    matrix_transform_mult(&M2, &M1, &M3);  // M3 = M2*M1
    matrix_transform_move(&M2, center);
    matrix_transform_mult(&M2, &M3, &M1);  // M1 = M2*M3
    matrix_transform_move(&M2, move);
    matrix_transform_mult(&M2, &M1, &M3);  // M3 = M2*M1
    matrix_transform_scale(&M2, scale);
    matrix_transform_mult(&M2, &M3, &M1);  // M1 = M2*M3

    obj_transform(&M1, obj);

    s21_remove_matrix(&M1);
    s21_remove_matrix(&M2);
    s21_remove_matrix(&M3);
    return 0;
}
