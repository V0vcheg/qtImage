//Capsule

#pragma once
class Capsule
{
private:
	double radius; 
	double height;
public:
	Capsule(double radius, double height);


	double getRadius() const;
	double getHeight() const;
};

