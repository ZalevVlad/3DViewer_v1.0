#include "vertex.h"

s21_vertex s21_vertex_neg(s21_vertex v) {
    v.x = -v.x;
    v.y = -v.y;
    v.z = -v.z;
    return v;
}

double s21_vertex_norm(s21_vertex v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

s21_vertex s21_vertex_add(s21_vertex a, s21_vertex b) {
    s21_vertex res = {.x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z};
    return res;
}

s21_vertex s21_vertex_diff(s21_vertex a, s21_vertex b) {
    s21_vertex res = {.x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z};
    return res;
}

s21_vertex s21_vertex_div(s21_vertex a, s21_vertex b) {
    s21_vertex res = {.x = a.x / b.x, .y = a.y / b.y, .z = a.z / b.z};
    return res;
}

s21_vertex s21_vertex_div_num(s21_vertex v, double x) {
    s21_vertex res = {.x = v.x / x, .y = v.y / x, .z = v.z / x};
    return res;
}