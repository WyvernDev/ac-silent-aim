#pragma once
#include <Windows.h>
#include <cstdint>
#include <cstddef>
#include <stdexcept>


inline auto calculate_relative(const std::uintptr_t location, const std::uintptr_t function_addr)
{
	return (function_addr - location) - 5;
}

std::uintptr_t tramp_hook(const std::uintptr_t target, const std::uintptr_t hook, const std::size_t n_nops = 0)
{
	const auto write_jump = [](std::uintptr_t addr, std::uintptr_t jmp_to, std::size_t n_nops = 0)
	{
		std::memset(reinterpret_cast<void*>(addr), 0x90, 5 + n_nops);
		*reinterpret_cast<std::uint8_t*>(addr) =  0xE9;
		*reinterpret_cast<std::uintptr_t*>(addr + 1) = calculate_relative(addr, jmp_to);
	};

	auto old = 0ul;
	VirtualProtect(reinterpret_cast<void*>(target), 5 + n_nops, PAGE_EXECUTE_READWRITE, &old);

	const auto gateway = reinterpret_cast<std::uintptr_t>(VirtualAlloc(nullptr, 5 + n_nops, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));

	if (!gateway)
		throw std::runtime_error("Failed to allocate gateway memory.");

	std::memmove(reinterpret_cast<void*>(gateway), reinterpret_cast<void*>(target), 5 + n_nops);
	write_jump(gateway + 5 + n_nops, target + 5);

	write_jump(target, hook, n_nops);

	VirtualProtect(reinterpret_cast<void*>(target), 5 + n_nops, old, &old);

	return gateway;
}