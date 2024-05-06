#ifndef VERTEX_H_
#define VERTEX_H_

#include "math.h"

typedef struct s21_vertex {
    double x;
    double y;
    double z;
    double w;
} s21_vertex;

s21_vertex s21_vertex_neg(s21_vertex v);
double s21_vertex_norm(s21_vertex v);
s21_vertex s21_vertex_add(s21_vertex a, s21_vertex b);
s21_vertex s21_vertex_diff(s21_vertex a, s21_vertex b);
s21_vertex s21_vertex_div(s21_vertex a, s21_vertex b);
s21_vertex s21_vertex_div_num(s21_vertex v, double x);

#endif  // VERTEX_H_