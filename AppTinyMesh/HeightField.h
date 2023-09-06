//HeightField

#pragma once
#include <mathematics.h>
#include <ui_interface.h>
class HeightField
{
private:
	int n, m; //!< Terrain dimensions (must be equal)
	double **grid; //!<Height values
public:
	HeightField(int nn, int mm);
	HeightField(int nn, int mm, QImage im, int noise);
	int getN();
	int getM();
	double getHeight(int x, int y);
	Vector getNormal(int x, int y);
};

