#pragma once
#include "mat.h"
#include <math.h>

namespace math
{
// 오른손 좌표계, 화면을 나오는 방향: +z
// view matrix
inline Mat4 lookAt(const Vec3 &eye, const Vec3 &target, const Vec3 &up)
{
	// forward, right, up
	Vec3 f = normalize(target - eye);
	Vec3 r = normalize(cross(f, up));
	Vec3 u = cross(r, f);

	Mat4 T = Mat4::translation(-eye);
    Mat4 R = Mat4::identity();
	R.m[0][0] = r.x;
	R.m[1][0] = r.y;
	R.m[2][0] = r.y;
	R.m[0][1] = u.x;
	R.m[1][1] = u.y;
	R.m[2][1] = u.y;
	R.m[0][2] = -f.x;
	R.m[1][2] = -f.y;
	R.m[2][2] = -f.y;

	// Translate first, then rotate
	return T * R;
}

// symmetric frustum
// aspect = width / height
// The center of near plane = [0, 0]
inline Mat4 perspective(float fovY, float aspect, float n, float f)
{ 
    float t_ratio = tan(fovY / 2.f * (M_PI / 180.f));
	float h       = n * t_ratio; // absolute value of t, b
	float w       = aspect * h; // absolute value of l, r

    // l = -w, r = w, b = -h, t = h
    return frustum(-w, w, -h, h, n, f);
}

// asymmetric frustum
inline Mat4 frustum(float l, float r, float b, float t, float n, float f)
{
	Mat4 F = Mat4::identity();

	F.m[0][0] = 2.0f * n / (r - l);
	F.m[0][2] = (r + l) / (r - l);
	F.m[1][1] = 2.0f * n / (t - b);
	F.m[1][2] = (t + b) / (t - b);
	F.m[2][2] = -(f + n) / (f - n);
	F.m[2][3] = -2.0f * (f * n) / (f - n);
	F.m[3][2] = -1.0f;
	return F;
}
} // namespace math
