#ifndef MATRIX_H
#define MATRIX_H
#define _CRT_SECURE_NO_WARNINGS

#include <stddef.h>

typedef struct {
	size_t rows;
	size_t cols;
	double* data;
} Matrix;

Matrix* matrix_create(size_t r, size_t c);

void matrix_free(Matrix* m);

int matrix_input(Matrix* m);

void matrix_print(const Matrix* m);

Matrix* matrix_clone(const Matrix* m);

Matrix* matrix_add(const Matrix* a, const Matrix* b);
Matrix* matrix_sub(const Matrix* a, const Matrix* b);
Matrix* matrix_mul(const Matrix* a, const Matrix* b);
Matrix* matrix_transpose(const Matrix* a);
Matrix* matrix_scalar_mul(const Matrix* a, double scalar);

int matrix_is_same_size(const Matrix* a, const Matrix* b);

int matrix_save_result_to_file(const Matrix* m, const char* operation_name, const char* filename);
int matrix_delete_all_data(const char* filename);
void matrix_show_saved_results(const char* filename);

#endif