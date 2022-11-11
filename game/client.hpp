#pragma once
#include "offsets.hpp"
#include "structs.hpp"
#include <vector>
#include <Windows.h>

namespace game::client
{
	static const auto base = reinterpret_cast<std::uintptr_t>(GetModuleHandleA(0)); /*base address of program...*/

	inline structs::player_entity* get_local_player() /*pretty self explanatory...*/
	{
		return *reinterpret_cast<structs::player_entity**>(base + offsets::local_player);
	}

	/*
	* used for teamcheck, assaultcube stores the game type in a variable, you can check if you're in a team game by simply comparing the values, this code is actually stolen from a decompiled assaultcube
	*/
	inline bool is_in_team_mode()
	{
		const auto game_type = *reinterpret_cast<std::uint8_t*>(base + offsets::game_type);

		return !(game_type
			&& game_type != 4
			&& game_type != 5
			&& game_type != 7
			&& game_type != 11
			&& game_type != 13
			&& game_type != 14
			&& game_type != 16
			&& game_type != 17
			&& game_type != 20
			&& game_type != 21);
	}

	/*
	* assaultcube doesnt store the local player in the entity list, so we need some way to get ahold of how many elements are actually in the entity list, we do this by subtracting 1
	* however, when in the demo map, the player count variable is 0, to prevent an integer underflow and wrapping all the way back around, we simply check that theres atleast 1 player before we subtract 1
	*/
	inline std::size_t get_other_player_count()
	{
		const auto players = *reinterpret_cast<std::size_t*>(base + offsets::num_players);
		return players < 1 || players > 32 ? 0 : players - 1;
	}

	/*
	* This function will get the closest enemy to the local player, uses vector math and shit...
	*/
	inline structs::player_entity* get_closest_enemy()
	{
		structs::player_entity* closest_entity = nullptr;
		auto local_plr = get_local_player();
		auto local_pos = local_plr->headpos;
		auto closest_dist = INT_MAX;

		for (auto i = 0; i < get_other_player_count(); i++)
		{
			const auto current_player = (*reinterpret_cast<structs::entity_list**>(base + offsets::entity_list))->entities[i];

			if (current_player->health <= 0 || current_player->player_state != structs::player_state_t::alive || (is_in_team_mode() && current_player->team == local_plr->team)) /*this makes sure the player is alive, and teamcheck...*/
				continue;

			const auto current_player_pos = current_player->headpos;
			const auto dist = (local_pos - current_player_pos).magnitude();

			if (closest_dist > dist)
			{
				closest_dist = dist;
				closest_entity = current_player;
			}
		}

		return closest_entity;
	}
}