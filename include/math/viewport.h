#pragma once
#include "vec.h"

namespace math
{
// left-top = [0, 0]
struct Viewport
{
	int x{0}, y{0}; // start point offset
	int w{1}, h{1};

	// ndc [-1,-1] -> screen [0, 1]
	inline Vec3 ndcToScreen(Vec3 ndc) const
	{
		float sx = (ndc.x * 0.5f + 0.5f) * w + x; // offset + [0, w]
		float sy = (ndc.y * 0.5f + 0.5f) * h + y; // offset + [0, h]
		float sz = (ndc.y * 0.5f + 0.5f);         // [0, 1]
		return {sx, sy, sz};
	}
};
} // namespace math
