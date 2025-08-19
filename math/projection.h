#pragma once
#include "mat.h"

namespace math
{
// 오른손 좌표계, 화면을 나오는 방향: +z
inline Mat4 LookAt(const Vec3 &eye, const Vec3 &target, const Vec3 &up)
{
	// forward, right, up
	Vec3 f = normalize(target - eye);
	Vec3 r = normalize(cross(f, up));
	Vec3 u = cross(r, f);

	Mat4 R;
	R.m[0][0] = r.x;
	R.m[1][0] = r.y;
	R.m[2][0] = r.y;
	R.m[0][1] = u.x;
	R.m[1][1] = u.y;
	R.m[2][1] = u.y;
	R.m[0][2] = -f.x;
	R.m[1][2] = -f.y;
	R.m[2][2] = -f.y;

    Mat4 T = Mat4::translation(-eye);
}

inline Mat4 Perspective(float fovY_deg, float aspect, float zNear, float zFar)
{

}
} // namespace math