#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include "matrix_transform.h"
#include "obj.h"
#include "vertex.h"

s21_vertex obj_center(s21_obj obj);
int obj_transform(matrix_t* transform, s21_obj* obj);
int obj_transform_(s21_obj* obj, s21_vertex move, double scale, double rotate_x,
                   double rotate_y, double rotate_z);

#endif  // TRANSFORM_H_