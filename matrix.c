#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define _CRT_SECURE_NO_WARNINGS

Matrix* matrix_create(size_t r, size_t c) {
    if (r == 0 || c == 0) return NULL;
    Matrix* m = malloc(sizeof(Matrix));
    if (!m) return NULL;
    m->rows = r;
    m->cols = c;
    m->data = calloc(r * c, sizeof(double));
    if (!m->data) { free(m); return NULL; }
    return m;
}

void matrix_free(Matrix* m) {
    if (!m) return;
    free(m->data);
    free(m);
}

int matrix_input(Matrix* m) {
    if (!m) return 0;
    printf("Unesite elemente matrice %zux%zu (redosled po redovima):\n", m->rows, m->cols);
    for (size_t i = 0; i < m->rows; ++i) {
        for (size_t j = 0; j < m->cols; ++j) {
            printf("m[%zu][%zu] = ", i, j);
            if (scanf("%lf", &m->data[i * m->cols + j]) != 1) {
                int ch;
                while ((ch = getchar()) != '\n' && ch != EOF);
                return 0;
            }
        }
    }
    return 1;
}

void matrix_print(const Matrix* m) {
    if (!m) return;
    for (size_t i = 0; i < m->rows; ++i) {
        for (size_t j = 0; j < m->cols; ++j) {
            printf("%8.2f ", m->data[i * m->cols + j]);
        }
        printf("\n");
    }
}

Matrix* matrix_clone(const Matrix* m) {
    if (!m) return NULL;
    Matrix* r = matrix_create(m->rows, m->cols);
    if (!r) return NULL;
    memcpy(r->data, m->data, sizeof(double) * m->rows * m->cols);
    return r;
}

int matrix_is_same_size(const Matrix* a, const Matrix* b) {
    if (!a || !b) return 0;
    return a->rows == b->rows && a->cols == b->cols;
}

Matrix* matrix_add(const Matrix* a, const Matrix* b) {
    if (!matrix_is_same_size(a, b)) return NULL;
    Matrix* r = matrix_create(a->rows, a->cols);
    if (!r) return NULL;
    size_t n = a->rows * a->cols;
    for (size_t i = 0; i < n; ++i) r->data[i] = a->data[i] + b->data[i];
    return r;
}

Matrix* matrix_sub(const Matrix* a, const Matrix* b) {
    if (!matrix_is_same_size(a, b)) return NULL;
    Matrix* r = matrix_create(a->rows, a->cols);
    if (!r) return NULL;
    size_t n = a->rows * a->cols;
    for (size_t i = 0; i < n; ++i) r->data[i] = a->data[i] - b->data[i];
    return r;
}

Matrix* matrix_mul(const Matrix* a, const Matrix* b) {
    if (!a || !b) return NULL;
    if (a->cols != b->rows) return NULL;
    Matrix* r = matrix_create(a->rows, b->cols);
    if (!r) return NULL;
    for (size_t i = 0; i < a->rows; ++i) {
        for (size_t j = 0; j < b->cols; ++j) {
            double sum = 0.0;
            for (size_t k = 0; k < a->cols; ++k) {
                sum += a->data[i * a->cols + k] * b->data[k * b->cols + j];
            }
            r->data[i * r->cols + j] = sum;
        }
    }
    return r;
}

Matrix* matrix_transpose(const Matrix* a) {
    if (!a) return NULL;
    Matrix* r = matrix_create(a->cols, a->rows);
    if (!r) return NULL;
    for (size_t i = 0; i < a->rows; ++i)
        for (size_t j = 0; j < a->cols; ++j)
            r->data[j * r->cols + i] = a->data[i * a->cols + j];
    return r;
}

Matrix* matrix_scalar_mul(const Matrix* a, double scalar) {
    if (!a) return NULL;
    Matrix* r = matrix_create(a->rows, a->cols);
    if (!r) return NULL;
    size_t n = a->rows * a->cols;
    for (size_t i = 0; i < n; ++i) r->data[i] = a->data[i] * scalar;
    return r;
}
