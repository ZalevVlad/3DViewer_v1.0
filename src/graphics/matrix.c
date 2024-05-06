#include "matrix.h"
#define EPS 1e-7

int s21_create_matrix(int rows, int columns, matrix_t* result) {
    int res = CORRECT;
    if (rows > 0 && columns > 0) {
        result->rows = rows;
        result->columns = columns;

        result->matrix =
            malloc(sizeof(double) * rows * columns + sizeof(double*) * rows);
        double* ptr = (double*)(result->matrix + rows);
        for (int i = 0; i < rows; i++) {
            result->matrix[i] = ptr + i * columns;
        }

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                result->matrix[i][j] = 0;
            }
        }

    } else {
        res = INCORRECT_MATRIX;
    }
    return res;
}

void s21_remove_matrix(matrix_t* A) {
    if (A) {
        if (A->matrix) {
            free(A->matrix);
            A->matrix = NULL;
        }
        A->rows = 0;
        A->columns = 0;
    }
}
