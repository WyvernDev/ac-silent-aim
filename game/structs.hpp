#pragma once
#include <cstdint>
#include "../math/vec.hpp"

namespace game::structs
{
	enum class player_state_t : int32_t
	{
		alive = 0,
		dead = 1
	};

	class player_entity
	{
	public:
		char pad_0000[4];
		vec3_t headpos;
		vec3_t velocity;
		char pad_001C[24];
		vec3_t feetpos;
		vec2_t view_angle;
		char pad_0048[68];
		bool a_is_pressed;
		bool d_is_pressed;
		bool w_is_pressed;
		bool s_is_pressed;
		char pad_0090[104];
		int32_t health;
		int32_t armour;
		char pad_0100[292];
		bool attacking;
		char name[15];
		char pad_0234[248];
		int32_t team;
		char pad_0330[8];
		player_state_t player_state;
		char pad_033C[60];
		class weapon* current_weapon;
	};


	class weapon
	{
	public:
		char pad_0000[8];
		class player_entity* owner;
	};

	class entity_list
	{
	public:
		char pad_0000[4];
		class player_entity* entities[31];
	};
}