#ifndef MATRIX_ALGOS_H
#define MATRIX_ALGOS_H

#include "matrix.h"

#include <assert.h>
#include <iostream>
#include <memory>
#include <cstring>
#include <vector>
#include <math.h>
#include <cmath>

#ifndef BASE_INDEX
	#define BASE_INDEX 0
#endif

/* Imprime una matriz */
std::ostream& operator<<(std::ostream& os, const Matrix& M);


/*Multiplicación de Matrix por escalar */
Matrix scalar_mult(const typename Matrix::value_type& scalar, const Matrix& A);

/*Suma de matrices */
Matrix operator+(const Matrix& A, const Matrix& B);

/*Resta de matrices */
Matrix operator-(const Matrix& A, const Matrix& B);

/*Multiplica 2 submatrices principales de A y B. 
Parametros: m es la cantidad de filas de la submatriz de A, 
			n es la cantidad de columnas y de filas de las submatrices de A y B respectivamente
			k es la cantidad de columnas de la sumatriz de B
*/
Matrix partial_matrix_mult(const Matrix& A, const Matrix& B, size_t l, size_t m, size_t n);

/*Multiplicacion de matrices */
Matrix operator*(const Matrix& A, const Matrix& B);

/*Igualdad de matrices ==*/
bool operator==(const Matrix& A, const Matrix& B);

/*Desigualdad de matrices !=*/
bool operator!=(const Matrix& A, const Matrix& B);

//B es una matrix cuadrada diagonal, A una matriz. Multiplicación A*B optimizada
Matrix diagonal_mult(const Matrix& A, const Matrix& B);

/*Devuelve una matriz inicialicada con k en todos sus elementos */
Matrix get_initialized(size_t n, size_t m, Matrix::value_type k);

/*Devuelve la matriz nula de n*m */
Matrix get_zero_initialized(size_t n, size_t m);

/*Devuelve la matriz nula de n*n */
Matrix get_zero_initialized(size_t n);

/*Devuelve la matriz identidad de n*n*/
Matrix get_identity(size_t n);

//Requiere que B sea un vector columna con la misma cantidad de filas que A
//Devuelve la matriz augmentada [A | B]
Matrix augmentar(const Matrix& A, const Matrix& B);

//Requiere que A sea la matrix resultante de aplicar eliminación gausseana sobre una matriz augmentada
//Devuelve el vector x, donde x_i es el valor de la variable según la resolución del sistema triangulado A
Matrix backward_substitution(const Matrix& A);

//pre: cols(A) == filas(b) ^ cols(b) = 1
//Resuelve el sistema Ax = b y devuelve x. Se asume que el sistema es compatible determinado y que EG puede ejecutarse sobre el.
Matrix resolver_sistema(const Matrix& A, const Matrix& b);

//Calcula los primeros k autovectores de B y devuelve una matriz en la que
//La j-esima columna es el autovector asociado al j-esimo autovalor (autovalores ordenados segun valor absoluto)
Matrix calcular_autovectores(Matrix B, size_t k = 0);

//Calcula la traspuesta de la matriz A. A(i,j) = A_t(j,i)
Matrix trasponer(const Matrix& A);

Matrix::value_type norma_2(const Matrix& v);

Matrix::value_type norma_inf(const Matrix& v);

#endif