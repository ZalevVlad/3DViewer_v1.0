#ifndef MATRIX_TRANSFORM_H_
#define MATRIX_TRANSFORM_H_

#include "matrix.h"
#include "vertex.h"

void matrix_transform_move(matrix_t* A, s21_vertex dest);
void matrix_transform_rotate_x(matrix_t* A, double angle);
void matrix_transform_rotate_y(matrix_t* A, double angle);
void matrix_transform_rotate_z(matrix_t* A, double angle);
void matrix_transform_scale(matrix_t* A, double k);

void matrix_transform_mult(matrix_t* A, matrix_t* B, matrix_t* C);

#endif  // MATRIX_TRANSFORM_H_