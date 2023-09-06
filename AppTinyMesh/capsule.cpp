// Capsule

// Self include
#include "capsule.h"
/*!
\brief Create a Capsule given radius and height (lenght).
param r Radius
param h Height
*/
Capsule::Capsule(double r, double h) {
	radius = r;
	height = h;
}
/*!
\brief Returns Height.
*/
double Capsule::getHeight() const {
	return height;
}
/*!
\brief Returns Radius.
*/
double Capsule::getRadius() const {
	return height;
}