#pragma once

namespace game::offsets
{
	/*
	* player related offsets
	*/
	constexpr auto local_player = 0x10f4f4;
	constexpr auto entity_list = 0x10F4F8;
	constexpr auto num_players = 0x10F500;
	
	/*
	* game related
	*/
	constexpr auto game_type = 0x10F49C;
	
	/*
	* function offsets
	*/
	constexpr auto silent_aim_hook_location = 0x637B6; /*this is where we hook so we can spoof the position at which the bullet spawns in...*/

}