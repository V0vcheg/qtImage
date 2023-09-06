//Tore

#pragma once
class Tore
{
private:
	double radius; //!< Radius of the inner circle
	double thickness; //!< Thickness of the tore
public:
	Tore(double r, double t);

	double getRadius() const;
	double getThickness() const;
};

