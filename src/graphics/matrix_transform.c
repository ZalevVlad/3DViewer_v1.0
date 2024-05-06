#include "matrix_transform.h"

void matrix_transform_move(matrix_t* A, s21_vertex dest) {
    A->matrix[0][0] = 1.;
    A->matrix[0][1] = 0.;
    A->matrix[0][2] = 0.;
    A->matrix[0][3] = dest.x;
    A->matrix[1][0] = 0.;
    A->matrix[1][1] = 1.;
    A->matrix[1][2] = 0.;
    A->matrix[1][3] = dest.y;
    A->matrix[2][0] = 0.;
    A->matrix[2][1] = 0.;
    A->matrix[2][2] = 1.;
    A->matrix[2][3] = dest.z;
    A->matrix[3][0] = 0.;
    A->matrix[3][1] = 0.;
    A->matrix[3][2] = 0.;
    A->matrix[3][3] = 1.;
}
void matrix_transform_rotate_x(matrix_t* A, double angle) {
    A->matrix[0][0] = 1.;
    A->matrix[0][1] = 0.;
    A->matrix[0][2] = 0.;
    A->matrix[0][3] = 0.;
    A->matrix[1][0] = 0.;
    A->matrix[1][1] = cos(angle);
    A->matrix[1][2] = -sin(angle);
    A->matrix[1][3] = 0.;
    A->matrix[2][0] = 0.;
    A->matrix[2][1] = sin(angle);
    A->matrix[2][2] = cos(angle);
    A->matrix[2][3] = 0.;
    A->matrix[3][0] = 0.;
    A->matrix[3][1] = 0.;
    A->matrix[3][2] = 0.;
    A->matrix[3][3] = 1.;
}
void matrix_transform_rotate_y(matrix_t* A, double angle) {
    A->matrix[0][0] = cos(angle);
    A->matrix[0][1] = 0.;
    A->matrix[0][2] = sin(angle);
    A->matrix[0][3] = 0.;
    A->matrix[1][0] = 0.;
    A->matrix[1][1] = 1.;
    A->matrix[1][2] = 0.;
    A->matrix[1][3] = 0.;
    A->matrix[2][0] = -sin(angle);
    A->matrix[2][1] = 0.;
    A->matrix[2][2] = cos(angle);
    A->matrix[2][3] = 0;
    A->matrix[3][0] = 0.;
    A->matrix[3][1] = 0.;
    A->matrix[3][2] = 0.;
    A->matrix[3][3] = 1.;
}
void matrix_transform_rotate_z(matrix_t* A, double angle) {
    A->matrix[0][0] = cos(angle);
    A->matrix[0][1] = -sin(angle);
    A->matrix[0][2] = 0.;
    A->matrix[0][3] = 0.;
    A->matrix[1][0] = sin(angle);
    A->matrix[1][1] = cos(angle);
    A->matrix[1][2] = 0.;
    A->matrix[1][3] = 0.;
    A->matrix[2][0] = 0.;
    A->matrix[2][1] = 0.;
    A->matrix[2][2] = 1.;
    A->matrix[2][3] = 0.;
    A->matrix[3][0] = 0.;
    A->matrix[3][1] = 0.;
    A->matrix[3][2] = 0.;
    A->matrix[3][3] = 1.;
}
void matrix_transform_scale(matrix_t* A, double k) {
    A->matrix[0][0] = k;
    A->matrix[0][1] = 0.;
    A->matrix[0][2] = 0.;
    A->matrix[0][3] = 0.;
    A->matrix[1][0] = 0.;
    A->matrix[1][1] = k;
    A->matrix[1][2] = 0.;
    A->matrix[1][3] = 0.;
    A->matrix[2][0] = 0.;
    A->matrix[2][1] = 0.;
    A->matrix[2][2] = k;
    A->matrix[2][3] = 0.;
    A->matrix[3][0] = 0.;
    A->matrix[3][1] = 0.;
    A->matrix[3][2] = 0.;
    A->matrix[3][3] = 1.;
}

void matrix_transform_mult(matrix_t* A, matrix_t* B, matrix_t* C) {
    for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < B->columns; j++) {
            C->matrix[i][j] = 0;
            for (int k = 0; k < B->rows; k++) {
                C->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
            }
        }
    }
}