#pragma once
#include "vec.h"

namespace math
{
// winding order: CW
// cross product: P * edge
// If all orients are positive, then inside
inline float edge(Vec2 v0, Vec2 v1, Vec2 p)
{
	// (p - v0) * (v1 - v0)
	return (p.x - v0.x) * (v1.y - v0.y) - (p.y - v0.y) * (v1.x - v0.x);
}
inline bool inside_cw(Vec2 v0, Vec2 v1, Vec2 v2, Vec2 p)
{
	float w0 = edge(v0, v1, p);
	float w1 = edge(v1, v2, p);
	float w2 = edge(v2, v0, p);

	return (w0 > 0.0f && w1 > 0.0f && w2 > 0.0f);
}
} // namespace math
