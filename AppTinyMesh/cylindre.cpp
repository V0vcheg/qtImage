#include "cylindre.h"
Cylindre::Cylindre(double r, double h) {
	radius = r;
	height = h;
}

double Cylindre::getRadius() const
{
	return radius;
}

double Cylindre::getHeight() const
{
	return height;
}


