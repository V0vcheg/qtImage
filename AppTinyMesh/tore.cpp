//Tore

// Self include
#include "tore.h"
/*!
\brief Create a Tore given radius and thickness of the donut.
param r Radius
param t Thickness
*/
Tore::Tore(double r, double t) {
	radius = r;
	thickness = t;
}
/*!
\brief Returns Radius.
*/
double Tore::getRadius() const {
	return radius;
}
/*!
\brief Returns Thickness.
*/
double Tore::getThickness() const {
	return thickness;
}