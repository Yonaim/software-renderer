#pragma once
#include "vec.h"

namespace math
{
struct Mat4
{
	float       m[4][4]; // row-major
	static Mat4 identity();
	static Mat4 translation(Vec3 t);
	static Mat4 scale(Vec3 s);

	// (row-major matrix) * (row-vector)
	Vec4 mul_point(Vec3 p) const;  // (x,y,z,1)
	Vec4 mul_vector(Vec3 v) const; // (x,y,z,0)
	Mat4 operator*(const Mat4 &r) const;
};
} // namespace math
