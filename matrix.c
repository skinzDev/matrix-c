#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
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

int matrix_save_result_to_file(const Matrix* m, const char* operation_name, const char* filename) {
    if (!m || !operation_name || !filename) return 0;
    
    FILE* file = fopen(filename, "a");
    if (!file) {
        printf("Greska: Ne mogu da otvorim fajl '%s' za pisanje.\n", filename);
        return 0;
    }
    
    time_t now = time(NULL);
    char* time_str = ctime(&now);
    time_str[strlen(time_str) - 1] = '\0';
    
    fprintf(file, "\n========================================\n");
    fprintf(file, "Operacija: %s\n", operation_name);
    fprintf(file, "Vreme: %s\n", time_str);
    fprintf(file, "Dimenzije: %zux%zu\n", m->rows, m->cols);
    fprintf(file, "========================================\n");
    
    for (size_t i = 0; i < m->rows; ++i) {
        for (size_t j = 0; j < m->cols; ++j) {
            fprintf(file, "%8.2f ", m->data[i * m->cols + j]);
        }
        fprintf(file, "\n");
    }
    fprintf(file, "\n");
    
    fclose(file);
    printf("Rezultat je sacuvan u fajl '%s'.\n", filename);
    return 1;
}

int matrix_delete_all_data(const char* filename) {
    if (!filename) return 0;
    
    FILE* file = fopen(filename, "w"); // Write mode truncates the file
    if (!file) {
        printf("Greska: Ne mogu da otvorim fajl '%s' za brisanje.\n", filename);
        return 0;
    }
    
    fclose(file);
    printf("Svi podaci iz fajla '%s' su obrisani.\n", filename);
    return 1;
}

void matrix_show_saved_results(const char* filename) {
    if (!filename) return;
    
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Fajl '%s' ne postoji ili ne moze da se otvori.\n", filename);
        return;
    }
    
    printf("\n=== Sadrzaj fajla '%s' ===\n", filename);
    
    char buffer[1000];
    int empty_file = 1;
    
    while (fgets(buffer, sizeof(buffer), file)) {
        printf("%s", buffer);
        empty_file = 0;
    }
    
    if (empty_file) {
        printf("Fajl je prazan.\n");
    }
    
    printf("\n=== Kraj fajla ===\n");
    fclose(file);
}