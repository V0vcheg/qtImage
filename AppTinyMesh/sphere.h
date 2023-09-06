//Sphere
#pragma once

#include <vector>
#include <iostream>

#include "mathematics.h"

class Sphere
{
public:
	Sphere() {}
	explicit Sphere(double radius);
	~Sphere() {}

	Sphere(Vector v, double radius);

	Vector getCenter() const;
	double getRadius() const;

protected:
	Vector c; //!< Center of the sphere
	double r; //!< Radius
};

