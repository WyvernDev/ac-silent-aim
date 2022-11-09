#include <Windows.h>
#include <thread>
#include <iostream>
#include "game/client.hpp"
#include "hook/tramp.hpp"

bool do_silent_aim = false;
std::uintptr_t original_func = 0;
std::uintptr_t silent_aim_func = 0;

void __declspec(naked) silent_aim_hook()
{
	static auto old_eax = 0;

	__asm
	{
		cmp [do_silent_aim], 0
		je exit_hook

		push eax
		call game::client::get_other_player_count
		cmp eax, 0
		pop eax
		jle exit_hook
		
		push eax
		call game::client::get_local_player
		cmp eax,[esi+8]
		pop eax
		jne exit_hook

		mov old_eax, eax
		call game::client::get_closest_enemy
		cmp eax, 0
		je pop_eax
		add eax, 4
		mov ecx, eax
		mov eax, [old_eax]
		jmp [silent_aim_func]

	pop_eax:
		mov eax, old_eax
	exit_hook:
		jmp [original_func]
	}
}

int main()
{
	original_func = tramp_hook(game::client::base + game::offsets::silent_aim_hook_location, (int)silent_aim_hook);
	silent_aim_func = original_func + 4;

	while (true)
	{
		if (GetAsyncKeyState(VK_INSERT) & 1)
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
