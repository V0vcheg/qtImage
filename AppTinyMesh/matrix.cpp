//Matrix
// 
// Self include
#include "matrix.h"
/*!
\brief Create a 3x3 identity matrix.
param radius Radius
*/
Matrix::Matrix() {
	tab = new double* [2];
	for (int i = 0; i <= 2; ++i) {
		tab[i] = new double[2];
		for (int j = 0; j <= 2; ++j) {
			tab[i][j] = 0;
		}
	}
	tab[0][0] = 1;
	tab[1][1] = 1;
	tab[2][2] = 1;
}

/*!
\brief Create a 3x3 from given matrix.
param t 3x3 matrix
*/
Matrix::Matrix(double **t) {
	tab = new double* [2];
	for (int i = 0; i <= 2; ++i) {
		for (int j = 0; j <= 2; ++j) {
			tab[i][j] = t[i][j];
		}
	}
}

/*!
\brief Inverse given matrix and returns true if done, false not possible
*/
bool Matrix::Inverse(Matrix &m) {
	float d = 0;
	for (int i = 0; i < 3; i++){
		d = d + (tab[0][i] * (tab[1][(i + 1) % 3] *
			tab[2][(i + 2) % 3] - tab[1][(i + 2) % 3] * tab[2][(i + 1) % 3]));
	}
	if (d == 0){
		return false;
	}
	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++){
			m.tab[i][j] = ((tab[(i + 1) % 3][(j + 1) % 3] *
					  tab[(i + 2) % 3][(j + 2) % 3]) - (tab[(i + 1) % 3][(j + 2) % 3] *
					  tab[(i + 2) % 3][(j + 1) % 3])) / d;
		}
		
	}
	return true;
}
/*!
\brief Transpose current matrix 
*/
void Matrix::Transpose() {
	double** newTab;
	newTab = new double* [2];
	for (int i = 0; i <= 2; ++i){
		for (int j = 0; j <= 2; ++j) {
			newTab[j][i] = tab[i][j];
		}
	}

	tab = newTab;
}
/*!
\brief Returns a result of multipling the matrix by a (3x1) Vector 
*/
Vector Matrix::operator*(Vector &vector)
{
    Vector newV = Vector(0);
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			newV[i] += vector[j] * tab[i][j];
		}
	}
	return newV;
}

