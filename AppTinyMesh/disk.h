//Disk

#pragma once

#include <vector>
#include <iostream>

#include "mathematics.h"
class Disk
{
public:
	//! Empty.
	Disk() {}
	explicit Disk(double radius);
	explicit Disk(Vector center, double radius);
	~Disk() {}

	Vector getCenter() const;
	double getRadius() const;

protected:
	Vector c; //!< Center of the disk
	double r; //!< Radius
};


