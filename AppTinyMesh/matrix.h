//Matrix

#pragma once
#include <vector>
#include <iostream>

#include "mathematics.h"
class Matrix
{
public:
	double **tab; //!< 2d array of the matrix (3x3)
	Matrix();
	Matrix(double **tab);


	bool Inverse(Matrix& m);

	void Transpose();

	Vector operator*(Vector &vector);
};

