#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

static void show_loading_bar(const char* message) {
	const int width = 30;
	printf("%s\n", message);

	for (int i = 0; i <= width; i++) {
		int percent = (i * 100) / width;

		printf("\r[");
		for (int j = 0; j < width; j++) {
			if (j < i)
				putchar('#');
			else
				putchar(' ');
		}
		printf("] %3d%%", percent);

		fflush(stdout);

#ifdef _WIN32
		Sleep(100);
#else
		usleep(100 * 1000);
#endif
	}
	printf("\n");
}

static void wait_after_output(void) {
#ifdef _WIN32
	Sleep(1000);
#else
	usleep(2000 * 1000);
#endif
}


static void clear_stdin(void) {
	int ch;
	while ((ch = getchar()) != '\n' && ch != EOF);
}

static Matrix* prompt_create_and_input(void) {
	size_t r, c;
	printf("Unesite broj redova: ");
	if (scanf("%zu", &r) != 1) { clear_stdin(); return NULL; }
	printf("Unesite broj kolona: ");
	if (scanf("%zu", &c) != 1) { clear_stdin(); return NULL; }
	if (r == 0 || c == 0 || r > 100 || c > 100) {
		printf("Nevalidne dimenzije (1 - 100), unesite nove dimenzije.\n");
		return NULL;
	}
	Matrix* m = matrix_create(r, c);
	if (!m) { printf("Greska pri alokaciji matrice.\n"); return NULL; }
	if (!matrix_input(m)) { printf("Greska pri unosu.\n"); matrix_free(m); return NULL; }
	return m;
}

int main(void) {
	int choice = 0;
	Matrix* A = NULL, * B = NULL, * R = NULL;

	while (1) {
		printf("\n=== Program za rad sa matricama ===\n");
		printf("1) Unesi matricu A\n");
		printf("2) Unesi matricu B\n");
		printf("3) Ispisi matricu A\n");
		printf("4) Ispisi matricu B\n");
		printf("5) A + B\n");
		printf("6) A - B\n");
		printf("7) A * B\n");
		printf("8) Transponuj A\n");
		printf("9) Transponuj B\n");
		printf("10) Pomnozi A skalarnim\n");
		printf("11) Pomnozi B skalarnim\n");
		printf("12) Oslobodi A\n");
		printf("13) Oslobodi B\n");
		printf("0) Izlaz\n");
		printf("Izbor: ");
		printf("\n\n");
		if (scanf("%d", &choice) != 1) { 
			clear_stdin(); printf("Nevalidan unos.\n"); 
			continue; 
		}

		switch (choice) {
		case 1:
			if (A) { 
				matrix_free(A); 
				A = NULL; 
			}
			A = prompt_create_and_input();
			break;
		case 2:
			if (B) { 
				matrix_free(B); 
				B = NULL; 
			}
			B = prompt_create_and_input();
			break;
		case 3:
			if (!A) { 
				printf("Matrica A nije inicijalizovana.\n"); 
			}
			else {
				matrix_print(A);
				wait_after_output();
			}
			break;
		case 4:
			if (!B) { 
				printf("Matrica B nije inicijalizovana.\n"); 
			}
			else {
				matrix_print(B);
				wait_after_output();
			}
			break;
		case 5:
			if (!A || !B) { 
				printf("Potrebne su obe matrice A i B.\n"); 
				break;
			}
			R = matrix_add(A, B);
			if (!R) { 
				printf("Greska: matrice nisu iste dimenzije.\n"); 
			}
			else {
				show_loading_bar("Sabiranje...");
				printf("A + B =\n"); 
				matrix_print(R);
				wait_after_output();
				matrix_free(R);
			}
			R = NULL;
			break;
		case 6:
			if (!A || !B) { 
				printf("Potrebne su obe matrice A i B.\n"); 
				break; 
			}
			R = matrix_sub(A, B);
			if (!R) { 
				printf("Greska: matrice nisu iste dimenzije.\n"); 
			}
			else {
				show_loading_bar("Oduzimanje...\n");
				printf("A - B =\n"); 
				matrix_print(R);
				wait_after_output();
				matrix_free(R);
			}
			R = NULL;
			break;
		case 7:
			if (!A || !B) { 
				printf("Potrebne su obe matrice A i B.\n"); 
				break; 
			}
			R = matrix_mul(A, B);
			if (!R) { 
				printf("Greska: neodgovarajuce dimenzije za mnozenje (A.cols must == B.rows).\n"); 
			}
			else {
				show_loading_bar("Mnozenje...\n");
				printf("A * B =\n"); 
				matrix_print(R);
				wait_after_output();
				matrix_free(R);
			}
			R = NULL;
			break;
		case 8:
			if (!A) { 
				printf("Matrica A nije inicijalizovana.\n"); 
				break; 
			}
			R = matrix_transpose(A);
			printf("A^T =\n"); 
			show_loading_bar("Transponovanje A matrice...\n");
			matrix_print(R);
			wait_after_output();
			matrix_free(R); 
			R = NULL;
			break;
		case 9:
			if (!B) { 
				printf("Matrica B nije inicijalizovana.\n"); 
				break; 
			}
			R = matrix_transpose(B);
			printf("B^T =\n"); 
			show_loading_bar("Transponovanje B matrice...\n");
			matrix_print(R);
			wait_after_output();
			matrix_free(R);
			R = NULL;
			break;
		case 10:
			if (!A) { 
				printf("Matrica A nije inicijalizovana.\n"); 
				break; 
			}
			{
				double s; 
				printf("Unesite skalar: "); 
				if (scanf("%lf", &s) != 1) { 
					clear_stdin(); printf("Nevalidan unos.\n"); break; 
				}
				R = matrix_scalar_mul(A, s);
				printf("A * %g =\n", s);
				show_loading_bar("Skaliranje A matrice...\n");
				matrix_print(R);
				wait_after_output();
				matrix_free(R); 
				R = NULL;
			}
			break;
		case 11:
			if (!B) { 
				printf("Matrica B nije inicijalizovana.\n"); 
				break; 
			}
			{
				double s; 
				printf("Unesite skalar: "); 
				if (scanf("%lf", &s) != 1) { 
					clear_stdin(); 
					printf("Nevalidan unos.\n"); 
					break; 
				}
				R = matrix_scalar_mul(B, s);
				printf("B * %g =\n", s);
				show_loading_bar("Skaliranje B matrice...\n");
				matrix_print(R);
				wait_after_output();
				matrix_free(R); 
				R = NULL;
			}
			break;
		case 12:
			if (A) { 
				matrix_free(A); A = NULL; 
				printf("Matrica A je oslobodjena.\n"); 
			}
			else {
				printf("Matrica A nije inicijalizovana.\n");
			}
			break;
		case 13:
			if (B) {
				matrix_free(B); B = NULL;
				printf("Matrica B je oslobodjena.\n");
			}
			else {
				printf("Matrica B nije inicijalizovana.\n");
			}
			break;
		case 0:
			if (A) matrix_free(A);
			if (B) matrix_free(B);
			printf("Izlaz.\n");
			return 0;
		default:
			printf("Nepoznata opcija.\n");
		}
	}
	return 0;
}