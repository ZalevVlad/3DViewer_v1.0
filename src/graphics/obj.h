#ifndef OBJ_H_
#define OBJ_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "vertex.h"

#define MAX_SIZE 1024

enum obj_parse_errs { OK, ERR, FILE_OPEN_ERR };

// Массив вершин
typedef struct s21_vertexes {
    int len;
    s21_vertex* vertexes;
} s21_vertexes;

typedef struct s21_facet {
    int len;
    int* numbers;
} s21_facet;

// Массив поверхностей
typedef struct s21_facets {
    int len;
    s21_facet* facets;
} s21_facets;

typedef struct s21_obj {
    s21_vertexes vertexes;
    s21_facets facets;
} s21_obj;

s21_obj create_obj();
int parse_obj(const char* filename, s21_obj* obj);
void free_obj(s21_obj* obj);

#endif  // OBF_H_
