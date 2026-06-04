#pragma once

struct color
{
	using uint = unsigned int;

	uint R;
	uint G;
	uint B;
	uint A;

	color(uint r = 0, uint g = 0, uint b = 0, uint a = 1) :
		R(r),
		G(g),
		B(b),
		A(a)
	{
	}
};