//Sphere

// Self include
#include "sphere.h"
/*!
\brief Create a Sphere given radius at Vector(0).
param radius Radius
*/
Sphere::Sphere(double radius)
{
	r = radius;
	c = Vector(0);
}
/*!
\brief Create a Sphere given radius and center Vector.
param v center
param radius radius
*/
Sphere::Sphere(Vector v,double radius)
{
	r = radius;
	c = v;
}
/*!
\brief Returns center Vector of the Sphere.
*/
Vector Sphere::getCenter() const
{
	return c;
}
/*!
\brief Returns Radius.
*/
double Sphere::getRadius() const
{
	return r;
}
