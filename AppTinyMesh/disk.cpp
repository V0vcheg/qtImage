//Disk
// 
// Self include
#include "disk.h"

/*!
\brief Create a Disk given radius at Vector(0).
param radius Radius
*/
Disk::Disk(double radius)
{
	c = Vector(0);
	r = radius;
}
/*!
\brief Create a Disk given radius and center Vector.
param center center
param radius radius
*/
Disk::Disk(Vector center, double radius)
{
	c = center;
	r = radius;
	
}
/*!
\brief Returns center Vector of the disk.
*/
Vector Disk::getCenter() const
{
	return c;
}
/*!
\brief Returns Radius.
*/
double Disk::getRadius() const
{
	return r;
}