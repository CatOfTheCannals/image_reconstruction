#ifndef MATRIX_ALGOS_CPP
#define MATRIX_ALGOS_CPP

#include "matrix_algos.h"

/* Imprime una matriz */
std::ostream& operator<<(std::ostream& os, const Matrix& M){

	for(int i = 0; i < M.rows() ; i++){
        for(int j = 0; j < M.cols(); j++){
            os << M(i,j) << " ";
        }
        os << std::endl;
    }

	return os;
}

/*Multiplicación de Matrix por escalar */
Matrix scalar_mult(const typename Matrix::value_type& scalar, const Matrix& A){

	Matrix C(A.rows(), A.cols());
	for(size_t i = BASE_INDEX; i < A.rows() + BASE_INDEX; i++)
		for(size_t j = BASE_INDEX; j < A.cols() + BASE_INDEX; j++)
			C.insert(i,j ,scalar*A(i,j));

	return C;
}

/*Suma de matrices */
Matrix operator+(const Matrix& A, const Matrix& B){	

	assert(A.rows() == B.rows() && A.cols() == B.cols());

	Matrix C(A.rows(), A.cols());
	for(size_t i = BASE_INDEX; i < A.rows() + BASE_INDEX; i++)
		for(size_t j = BASE_INDEX; j < A.cols() + BASE_INDEX; j++)
			C.insert(i,j, A(i,j) + B(i,j));

	return C;

}

Matrix operator-(const Matrix& A, const Matrix& B){	
	
	assert(A.rows() == B.rows() && A.cols() == B.cols());

	Matrix C(A.rows(), A.cols());
	for(size_t i = BASE_INDEX; i < A.rows() + BASE_INDEX; i++)
		for(size_t j = BASE_INDEX; j < A.cols() + BASE_INDEX; j++)
			C.insert(i, j, A(i,j) - B(i,j));

	return C;

}

bool operator==(const Matrix& A, const Matrix& B){

	assert(A.rows() == B.rows() && A.cols() == B.cols());


	for(size_t i = BASE_INDEX; i < A.rows() + BASE_INDEX; i++)
		for(size_t j = BASE_INDEX; j < A.cols() + BASE_INDEX; j++){
			if( fabs(A(i,j) - B(i,j)) >  A.epsilon){ //todas las matrices tienen el mismo epsilon
				return false;
			}
		}

	return true;

}

bool operator!=(const Matrix& A, const Matrix& B){

	return !(A == B);
}

/*Multiplicacion de 2 submatrices principales de A y B */
//l: filas de submatriz de A
//m: columnas de submatriz de A y filas de submatriz de B
//n: columnas de submatriz de B
Matrix partial_matrix_mult(const Matrix& A, const Matrix& B, size_t l, size_t m, size_t n){
	
	l = std::min(l, A.rows());
	m = std::min(m, A.cols());
	n = std::min(n, B.cols());

	assert(m <= B.rows());

	Matrix C = get_zero_initialized(l, n);

	for(size_t i = BASE_INDEX; i < l + BASE_INDEX; i++){
		for(size_t j = BASE_INDEX; j < m + BASE_INDEX; j++){
			for(size_t k = BASE_INDEX; k < n + BASE_INDEX; k++){
				C.insert(i, k, C(i,k) + A(i, j)*B(j,k));
			}

		}
	}

	return C;
}

Matrix operator*(const Matrix& A, const Matrix& B){
	
	assert(A.cols() == B.rows());
	Matrix C(A.rows(), B.cols());

	for(size_t i = BASE_INDEX; i < A.rows() + BASE_INDEX; i++){
		for(size_t j = BASE_INDEX; j < B.cols() + BASE_INDEX; j++){
            double acum = 0;
            for(int k = BASE_INDEX; k < A.cols() + BASE_INDEX ; k++){
			    acum+= A(i,k)*B(k,j);
            }
            C(i,j) = acum;
		}
	}

	return C;
}

//B es una matrix cuadrada diagonal
Matrix diagonal_mult(const Matrix& A, const Matrix& B){

	assert(A.rows() == B.rows() && A.cols() == B.cols());

	
	Matrix C(A);
	size_t n = C.rows() - 1 + BASE_INDEX;
	size_t m = C.cols() - 1 + BASE_INDEX;

	std::vector<Matrix::value_type> d(B.rows());
	for(size_t i = 0; i < d.size(); i++) 
		d[i] = B(i + BASE_INDEX, i + BASE_INDEX);

	for(size_t i = BASE_INDEX; i <= n; i++){
		for(size_t j = BASE_INDEX; j <= m; j++){
			C.insert(i, j, C(i,j)*d[j - BASE_INDEX]);
		}
	}

	return C;
}


Matrix get_initialized(size_t n, size_t m, Matrix::value_type element){
	Matrix C(n, m);
	for(size_t i = BASE_INDEX; i < n + BASE_INDEX; i++)
		for(size_t j = BASE_INDEX; j < m + BASE_INDEX; j++)
			C.insert(i, j ,element);
	return C;
}

Matrix get_zero_initialized(size_t n, size_t m){
	return get_initialized(n, m, Matrix::value_type(0));
}

Matrix get_zero_initialized(size_t n){
	return get_zero_initialized(n, n);
}

/*Devuelve una matriz identidad */
Matrix get_identity(size_t n){
	Matrix::value_type one = 1;
	Matrix C = get_zero_initialized(n);

	for(size_t i = BASE_INDEX; i < n + BASE_INDEX; i++)
		C.insert(i,i ,one);
	return C;
}


//Requiere que B sea un vector columna con la misma cantidad de filas que A
Matrix augmentar(const Matrix& A, const Matrix& B){

	assert(B.cols() == 1 && A.rows() == B.rows());
	Matrix C(A.rows(), A.cols() + 1);

	for(size_t i = BASE_INDEX; i < A.rows() + BASE_INDEX; i++){
		for(size_t j = BASE_INDEX; j < A.cols() + BASE_INDEX; j++){
			C.insert(i, j, A(i,j));
		}
	}

	for(size_t i = BASE_INDEX; i < B.rows() + BASE_INDEX; i++){
		C.insert(i, A.cols() + BASE_INDEX, B(i, BASE_INDEX));
	}
	return C;
}

//Requiere que A sea la matrix resultante de aplicar eliminación gausseana sobre una matrix augmentada
Matrix backward_substitution(const Matrix& A){

	size_t n = A.rows() - 1 + BASE_INDEX;
	size_t m = A.cols() - 1 + BASE_INDEX;

	Matrix X(A.rows(), 1);

	X.insert(n, BASE_INDEX, A(n, m)/A(n, m-1));

	for(size_t i_aux = BASE_INDEX; i_aux <= n - 1; i_aux++){
		size_t i = n - i_aux;

		//Calcular x_i
		//x_i = b_i - sum(a_ij con j > i) / a_ii
		Matrix::value_type sum(0), c(0);
		for(size_t j = i + 1; j <= m - 1; j++){
			Matrix::value_type y = A(i, j)*X(j, BASE_INDEX) - c;
			Matrix::value_type t = sum + y;
			c = (t - sum) - y;
			sum = t;
		}

		X.insert(i, BASE_INDEX, (A(i, m) - sum)/A(i,i));
	}
	return X;
}

Matrix resolver_sistema(const Matrix& A, const Matrix& b){

	assert(A.cols() == b.rows() && b.cols() == 1);
	Matrix tmp = augmentar(A, b);
	eliminacion_gausseana(tmp);
	return backward_substitution(tmp);
}

Matrix resolver_sistema_con_svd(const Matrix& v, const Matrix& sInv_ut, const Matrix& b){
	return v * (sInv_ut * b);
}

std::tuple<Matrix, Matrix, double> generar_svd(const Matrix& A){
    std::cout << A << std::endl;
    Matrix AtA = A.mt_times_m();
    std::cout << AtA << std::endl;
    auto u_s = calcular_autovectores(AtA, AtA.rows());
	Matrix u = std::get<0>(u_s);
	Matrix s = std::get<1>(u_s);
    
	Matrix sq = sqrt_to_all_elems(s);
	double numero_de_condicion = sq(BASE_INDEX, BASE_INDEX) / sq(BASE_INDEX + sq.rows()-1, BASE_INDEX);
    std::cout << s << std::endl << std::endl;
    std::cout << sq << std::endl << std::endl;
    std::cout << std::endl;
    Matrix ut = trasponer(u);

    Matrix vt = apply_inverse_sigma(sq, ut*A, A.cols());

    Matrix sInv_ut = apply_inverse_sigma(sq, ut, A.cols());

    return std::make_tuple(trasponer(vt), sInv_ut, numero_de_condicion);
}

Matrix sqrt_to_all_elems(const Matrix& A) {
	Matrix res(A);
	for (int  i = 0;  i < res.rows(); ++ i) {
		for (int j = 0; j < res.cols(); ++j) {
			res.insert(i,j, sqrt(res(i,j)) );
		}
	}
	return res;
}

Matrix apply_inverse_sigma(const Matrix& s, const Matrix& A, int rows) {

    Matrix res = subMatrix(A, 0, rows-1, 0, A.cols()-1);
    // for i, singular_value in enumerate(s):
    for (int  i = 0;  i < s.rows(); ++ i) {
        for (int j = 0; j < res.cols(); ++j) {
            res.insert(i,j, res(i,j) / s(i, 0));
        }
    }
    return res;
}

Matrix subMatrix(const Matrix& A, int i1, int i2, int j1, int j2){
    /*
     * returns the matrix between (i1, i2) rows and (j1, j2) cols
     * indexes i2 and j2 will be part of the answer
     */

    assert(i1 <= i2 && j1 <= j2);
    assert(-1 < i1 && -1 < j2);
    assert(i2 < A.rows() && j2 < A.cols());

    int res_rows = i2 - i1 + 1;
    int res_cols = j2 - j1 + 1;

    Matrix res(res_rows, res_cols);
    for(int i = BASE_INDEX; i < res.rows() + BASE_INDEX; i++){
        for(int j = BASE_INDEX; j < res.cols() + BASE_INDEX; j++){
            res.insert(i,j, A(i + i1, j1 + j));
        }
    }
    return res;
}

bool is_relevant(double d){
    double epsilon = 1e-5;
    if(d < 0){ return d < epsilon*(-1); }
    else{ return d > epsilon; }
}

//Calcula los primeros k autovectores de B
std::tuple<Matrix, Matrix> calcular_autovectores(Matrix B, size_t k){

	assert(B.rows() == B.cols());
	size_t n = B.rows();
    if(k == 0){k = B.rows();}

	//El vector inicial se calcula random.
	//Ajustamos iteraciones según nuestro criterio.
	Matrix v(n, 1);
	Matrix autovalores(n, 1);
	Matrix v_anterior(n, 1);
	Matrix C(n, k);
	int iteraciones = 15;

	//calcular n autovectores
	for(size_t autovector_actual = BASE_INDEX; autovector_actual < k + BASE_INDEX; autovector_actual++){


		if(0 == k%100){
			std::cout << "debug: Calculando autovector nº " << (1 + autovector_actual) << " / " << k << std::endl;
		}

		//Inicializa un vector random. No importa cual sea
		for(size_t i = BASE_INDEX; i < n + BASE_INDEX; i++){
			v.insert(i, BASE_INDEX, (1 - 2*(i%2)) );
		}

		for(int i = 0; i < iteraciones ; i++){
			
			v_anterior = v;
			//Itera sobre el producto de B*v. Cuantas mas iteraciones se hagan. Mas cerca va a estar v
			//Del subespacio generado por alguno de los autovectores de B.
			v = B*v;
			//Para evitar que crezcan demasiado los valores de v

            double norma = norma_2(v);
			v = scalar_mult((1/norma), v);
		}

		Matrix v_t = trasponer(v); 

		for(size_t i = BASE_INDEX; i < n + BASE_INDEX; i++){
			//Insertas el autovector i en la columna correspondiente de C
			C.insert(i, autovector_actual, v(i, BASE_INDEX));
		}

		Matrix numer = B*v;
		bool escape = false;
		double autovalor_asociado = 0;
		for(int val = 0; val < v.rows() && escape==false; val++){
			double denom = v(val,0);
			if( is_relevant(denom) ){
				autovalor_asociado = numer(val,0)/denom;
			    escape = true;
			}
		}
		autovalores.insert(autovector_actual, BASE_INDEX, autovalor_asociado);

		if(autovector_actual != n + BASE_INDEX - 1){
			// Elimina el autoespacio asociado al autovector calculado de la base de autovectores de B (vìa deflacion)
            //std::cout << B << std::endl << std::endl;
            std::cout << v(BASE_INDEX,BASE_INDEX) << ", " << v(BASE_INDEX+1, BASE_INDEX) << std::endl ;
            std::cout << "autovalores : " << autovalor_asociado <<", " << numer(BASE_INDEX+1, BASE_INDEX)/v(BASE_INDEX+1, BASE_INDEX) << std::endl ;
            
            std::cout << numer(BASE_INDEX,BASE_INDEX)<< ", " << numer(BASE_INDEX+1, BASE_INDEX) << std::endl ;
            //std::cout << numer*v_t << std::endl << std::endl;
			B = (B - numer*v_t);
            //std::cout << B << std::endl << std::endl;
		}

	}

	return std::make_tuple(C,autovalores);
}

Matrix trasponer(const Matrix& A){

	Matrix C(A.cols(), A.rows());

	for(size_t i = BASE_INDEX; i < A.rows() + BASE_INDEX; i++)
		for(size_t j = BASE_INDEX; j < A.cols() + BASE_INDEX; j++)
			C.insert(j, i, A(i,j));

	return C;
}

inline
Matrix::value_type norma_2(const Matrix& v){
	assert(v.cols() == 1);

	size_t n = v.rows();	
	double res = 0;

	for(size_t i = BASE_INDEX; i < n + BASE_INDEX; i++){
		double y = v(i, BASE_INDEX);
		res += y*y;
	}
	return sqrt(res);
}

inline
Matrix::value_type norma_inf(const Matrix& v){
	assert(v.cols() == 1);

	typedef Matrix::value_type decimal;

	size_t n = v.rows();	
	decimal norma_inf(0);
	decimal c(0);
	for(size_t i = BASE_INDEX; i < n + BASE_INDEX; i++){
		decimal y = std::abs(v(i, BASE_INDEX)) - c;
		decimal t = norma_inf + y;
		c = (t - norma_inf) - y;
		norma_inf = t;
	}

	return norma_inf;

}


#endif