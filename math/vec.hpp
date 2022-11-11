#pragma once
#include <cmath>
#include <cstdint>

/*
* simple vector class shit
*/

struct vec2_t
{
	float x, y;

	inline auto magnitude()
	{
		return std::sqrtf((x * x) + (y * y));
	}
};

struct vec3_t
{
	float x, y, z;

	inline auto magnitude()
	{
		return std::sqrtf((x * x) + (y * y) + (z * z));
	}

	inline auto operator-(const vec3_t& other)
	{
		return vec3_t{ x - other.x, y - other.y, z - other.z };
	}
};
