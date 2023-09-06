// HeightField
// 
// Self include
#include "HeightField.h"
#include <stdlib.h>
/*!
\brief Create a HeightField given a dimensions (nn must be equal to mm).
Height grid will be filled with random number from 0 to 1
param nn, mm Dimensions (nn=mm)
*/
HeightField::HeightField(int nn, int mm) {

	grid = new double* [nn];
	for (int i = 0; i <= nn; ++i) {
		grid[i] = new double[mm];
		for (int j = 0; j <= mm; ++j) {
			grid[i][j] = (rand() % (10000)) / 10000.0;
		}
	}

	n = nn;
	m = mm;
}
/*!
\brief Create a HeightField given a dimensions (nn must be equal to mm), an image and a noise value.
Height grid will be based on the color of the image converted to grey
param nn, mm Dimensions (nn=mm)
param im QImage used to calculate heights
param noise Increase or decrease the range of height calculated (noise 1 -> [0-255] (max rgb value)) 
*/
HeightField::HeightField(int nn, int mm, QImage im, int noise) {
	im = im.scaled(nn+1, mm+1);
	grid = new double* [nn];
	for (int i = 0; i <= nn; ++i) {
		grid[i] = new double[mm];
		for (int j = 0; j <= mm; ++j) {
			double grey =(double)qGray(im.pixel(i,j));
			double scaledGrey = (noise - 0)*((grey - 0) / (255)) - 0;
			grid[i][j] = scaledGrey;
		}
	}

	n = nn;
	m = mm;
}

/*!
\brief Returns N
*/
int HeightField::getN() {
	return n;
}
/*!
\brief Returns M
*/
int HeightField::getM() {
	return m;
}
/*!
\brief Returns height value at grid[x][y]
param x X coord
param x Y coord
*/
double HeightField::getHeight(int x, int y){
	if (x < 0 || y < 0) {
		return -1;
	}
	if (x > m || y > n) {
		return -1;
	}
	return grid[x][y];
}

/*!
\brief Returns normal vector from the current point calucalted by scalar product of adjacents points
param x X coord
param x Y coord
*/
Vector HeightField::getNormal(int x, int y) {
	int ip;
	int in = x + 1;
	int jp;
	int jn = y + 1;
	if (x == 0)
	{
		ip = 0;
	}
	else if (x == n) {
		in = x;
	}
	else {
		ip = x - 1;
	}
	if (y == 0) {
		jp = 0;
	}
	else if (y == m) {
		jn = y;
	}
	else {
		jp = y - 1;
	}

	Vector a(ip, y, grid[jp][y]);
	Vector b(in, y, grid[in][y]);
	Vector c(x, jp, grid[x][jp]);
	Vector d(x, jn, grid[x][jn]);

	Vector ab = Normalized(b - a);
	Vector cd = Normalized(d - c);

	Vector n = ab/cd;

	return n;
}