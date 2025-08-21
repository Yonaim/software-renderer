#pragma once
#include <cmath>

/**
 * @brief Vec2 / Vec3 / Vec4: POD (Plain Old Data)
 *
 * Only 'compound assignments(+,-,*,/)' are member function.
 * The others (unary, binary, scalar mul/div, dot, cross, length, normalize) are all non-member
 * function.
 */

namespace math
{
	struct Vec2
	{
		float x{}, y{};

		// compound assignments
		Vec2 &operator+=(const Vec2 &r) noexcept
		{
			x += r.x;
			y += r.y;
            return *this;
		}
		Vec2 &operator-=(const Vec2 &r) noexcept
		{
			x -= r.x;
			y -= r.y;
			return *this;
		}
		Vec2 &operator*=(float s) noexcept
		{
			x *= s;
			y *= s;
			return *this;
		}
		Vec2 &operator/=(float s) noexcept
		{
			x /= s;
			y /= s;
			return *this;
		}
	};
	struct Vec3
	{
		float x{}, y{}, z{};

		// compound assignments
		Vec3 &operator+=(const Vec3 &r) noexcept
		{
			x += r.x;
			y += r.y;
			z += r.z;
			return *this;
		}
		Vec3 &operator-=(const Vec3 &r) noexcept
		{
			x -= r.x;
			y -= r.y;
			z -= r.z;
			return *this;
		}
		Vec3 &operator*=(float s) noexcept
		{
			x *= s;
			y *= s;
			z *= s;
			return *this;
		}
		Vec3 &operator/=(float s) noexcept
		{
			x /= s;
			y /= s;
			z /= s;
			return *this;
		}
	};
	struct Vec4
	{
		float x{}, y{}, z{}, w{};

		// compound assignments
		Vec4 &operator+=(const Vec4 &r) noexcept
		{
			x += r.x;
			y += r.y;
			z += r.z;
			w += r.w;
			return *this;
		}
		Vec4 &operator-=(const Vec4 &r) noexcept
		{
			x -= r.x;
			y -= r.y;
			z -= r.z;
			w -= r.w;
			return *this;
		}
		Vec4 &operator*=(float s) noexcept
		{
			x *= s;
			y *= s;
			z *= s;
			w *= s;
			return *this;
		}
		Vec4 &operator/=(float s) noexcept
		{
			x /= s;
			y /= s;
			z /= s;
			w /= s;
			return *this;
		}
	};

	// ----------------------------------
	// Unary operators
	// ----------------------------------
	inline Vec2 operator-(Vec2 v)
	{
		return {-v.x, -v.y};
	}
	inline Vec3 operator-(Vec3 v)
	{
		return {-v.x, -v.y, -v.z};
	}
	inline Vec4 operator-(Vec4 v)
	{
		return {-v.x, -v.y, -v.z, -v.w};
	}

	// ----------------------------------
	// Binary operators
	// ----------------------------------
	inline Vec2 operator+(Vec2 a, Vec2 b)
	{
		return {a.x + b.x, a.y + b.y};
	}
	inline Vec2 operator-(Vec2 a, Vec2 b)
	{
		return {a.x - b.x, a.y - b.y};
	}
	inline Vec3 operator+(Vec3 a, Vec3 b)
	{
		return {a.x + b.x, a.y + b.y, a.z + b.z};
	}
	inline Vec3 operator-(Vec3 a, Vec3 b)
	{
		return {a.x - b.x, a.y - b.y, a.z - b.z};
	}
	inline Vec4 operator+(Vec4 a, Vec4 b)
	{
		return {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
	}
	inline Vec4 operator-(Vec4 a, Vec4 b)
	{
		return {a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
	}

	// ----------------------------------
	// Scalar multiply / divide (with symmetry support)
	// ----------------------------------
	inline Vec2 operator*(float s, Vec2 v)
	{
		return {v.x * s, v.y * s};
	}
	inline Vec2 operator*(Vec2 v, float s)
	{
		return {v.x * s, v.y * s};
	}
	inline Vec2 operator/(Vec2 v, float s)
	{
		return {v.x / s, v.y / s};
	}

	inline Vec3 operator*(float s, Vec3 v)
	{
		return {v.x * s, v.y * s, v.z * s};
	}
	inline Vec3 operator*(Vec3 v, float s)
	{
		return {v.x * s, v.y * s, v.z * s};
	}
	inline Vec3 operator/(Vec3 v, float s)
	{
		return {v.x / s, v.y / s, v.z / s};
	}

	inline Vec4 operator*(float s, Vec4 v)
	{
		return {v.x * s, v.y * s, v.z * s, v.w * s};
	}
	inline Vec4 operator*(Vec4 v, float s)
	{
		return {v.x * s, v.y * s, v.z * s, v.w * s};
	}
	inline Vec4 operator/(Vec4 v, float s)
	{
		return {v.x / s, v.y / s, v.z / s, v.w / s};
	}

	// ----------------------------------
	// dot product
	// ----------------------------------
	inline float dot(const Vec2 &a, const Vec2 &b) noexcept
	{
		return a.x * b.x + a.y * b.y;
	}
	inline float dot(const Vec3 &a, const Vec3 &b) noexcept
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}
	inline float dot(const Vec4 &a, const Vec4 &b) noexcept
	{
		return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	}

	// ----------------------------------
	// cross product (vec3)
	// ----------------------------------
	inline Vec3 cross(Vec3 a, Vec3 b)
	{
		return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
	}

	// ----------------------------------
	// normalize
	// ----------------------------------
	inline Vec3 normalize(Vec3 v)
	{
		float l = std::sqrt(dot(v, v));
		return l > 0.0f ? (v * (1.0f / l)) : v;
	}

	// ----------------------------------
	// length
	// ----------------------------------
	inline float length2(const Vec2 &v) noexcept
	{
		return dot(v, v);
	};
	inline float length2(const Vec3 &v) noexcept
	{
		return dot(v, v);
	};
	inline float length2(const Vec4 &v) noexcept
	{
		return dot(v, v);
	};

	inline float length(const Vec2 &v) noexcept
	{
		return std::sqrt(dot(v, v));
	};
	inline float length(const Vec3 &v) noexcept
	{
		return std::sqrt(dot(v, v));
	};
	inline float length(const Vec4 &v) noexcept
	{
		return std::sqrt(dot(v, v));
	};

	// ----------------------------------
	// zero check
	// ----------------------------------
	inline bool is_near_zero(const Vec2 &v, float eps = 1e-8f) noexcept
	{
		return (std::fabs(v.x) < eps && std::fabs(v.y) < eps);
	}
	inline bool is_near_zero(const Vec3 &v, float eps = 1e-8f) noexcept
	{
		return (std::fabs(v.x) < eps && std::fabs(v.y) < eps && std::fabs(v.z) < eps);
	}
	inline bool is_near_zero(const Vec4 &v, float eps = 1e-8f) noexcept
	{
		return (std::fabs(v.x) < eps && std::fabs(v.y) < eps && std::fabs(v.z) < eps
				&& std::fabs(v.w) < eps);
	}
} // namespace math
