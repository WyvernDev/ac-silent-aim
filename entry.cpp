#include <Windows.h>
#include <thread>
#include <iostream>
#include "game/client.hpp"
#include "hook/tramp.hpp"

bool do_silent_aim = false;
std::uintptr_t original_func = 0;
std::uintptr_t silent_aim_func = 0;

void __declspec(naked) silent_aim_hook() /*naked func so we dont generate any stack setup bullshit...*/
{
	static auto old_eax = 0;

	__asm
	{
		/*check to make sure the user wants to do silent aim...*/
		cmp [do_silent_aim], 0
		je exit_hook

		/*check to make sure that theres atleast one other player...*/
		push eax
		call game::client::get_other_player_count
		cmp eax, 1
		pop eax
		jle exit_hook
		
		/*check to make sure that the local player was the calling player, so we dont accidentally allow silent aim for other players...*/
		push eax
		call game::client::get_local_player
		cmp eax,[esi+8]
		pop eax
		jne exit_hook

		/*actual silent aim logic*/
		mov old_eax, eax
		call game::client::get_closest_enemy
		cmp eax, 0
		je pop_eax
		add eax, 4 /*eax holds the return from get_closest_enemy, the offset from the base of the player object to their head position is + 4, so add 4 to get a pointer to the enemy player's head*/
		mov ecx, eax /*ecx is the register that holds the pointer to the bullet spawn position, eax holds a pointer to the enemy players head position, hmmm... I'm sure you can figure out what happens next ;)*/
		mov eax, [old_eax]
		jmp [silent_aim_func] /*we want to skip over the instruction that changes the value of ecx, silent_aim_func is just original_func + 4, so we can skip the instruction that loads the real bullet spawn pos into ecx*/

	pop_eax:
		mov eax, old_eax
	exit_hook:
		jmp [original_func]
	}
}

int main()
{
	original_func = tramp_hook(game::client::base + game::offsets::silent_aim_hook_location, reinterpret_cast<int>(silent_aim_hook)); /*perform the actual hook lol*/
	silent_aim_func = original_func + 4;

	while (true)
	{
		if (GetAsyncKeyState(VK_INSERT) & 1) /*press insert to toggle*/
			do_silent_aim = !do_silent_aim;

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	return EXIT_SUCCESS;
}


BOOL WINAPI DllMain(HMODULE h_inst_dll, unsigned long reason_for_call, void* reserved)
{
	if (reason_for_call == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(h_inst_dll);
		std::thread(main).detach();
	}
	return true;
}
