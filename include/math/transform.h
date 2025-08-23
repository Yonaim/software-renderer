#pragma once
#include "mat.h"
#include <cmath>

namespace math
{
inline Mat4 rotateX(float r)
{
	float c = std::cos(r);
	float s = std::sin(r);
	Mat4  M = Mat4::identity();

	M.m[1][1] = c;
	M.m[1][2] = s;
	M.m[2][1] = -s;
	M.m[2][2] = c;
	return M;
}
inline Mat4 rotateY(float r)
{
	float c = std::cos(r);
	float s = std::sin(r);
	Mat4  M = Mat4::identity();

	M.m[0][0] = c;
	M.m[0][2] = -s;
	M.m[2][0] = s;
	M.m[2][2] = c;
	return M;
}
inline Mat4 rotateZ(float r)
{
	float c = std::cos(r);
	float s = std::sin(r);
	Mat4  M = Mat4::identity();

	M.m[0][0] = c;
	M.m[0][1] = s;
	M.m[1][0] = -s;
	M.m[1][1] = c;
	return M;
}
} // namespace math
