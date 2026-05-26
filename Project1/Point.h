#pragma once

struct color
{
	using uint = unsigned int;

	uint R;
	uint G;
	uint B;
	uint A;

	color(uint r = 0, uint g = 0, uint b = 0, uint a = 1):
		R(r),
		G(g),
		B(b),
		A(a)
	{ }
};
struct Point
{
	double x;
	double y;
	double z;

	color col;

	Point(double _x, double _y, double _z, color _col) :
		x(_x),
		y(_y),
		z(_z),
		col(_col)
	{ }
};