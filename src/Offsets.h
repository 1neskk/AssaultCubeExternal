#pragma once
#include <Windows.h>

#define AC_VERSION_LATEST

namespace Offsets
{
#ifdef AC_VERSION_LATEST
	// version 1.3.0.2
    constexpr DWORD player_base = 0x17E0A8; // local player ptr
    constexpr DWORD name = 0x205;

	constexpr DWORD entity_list = 0x18AC04; // entity list ptr
    constexpr DWORD team = 0x30C;
    constexpr DWORD view_matrix = 0x57DFD0;

    constexpr DWORD health = 0xEC; // int
    constexpr DWORD armor = 0xF0; // int
    constexpr DWORD rifle_ammo = 0x140; // int
    constexpr DWORD rifle_cooldown = 0x164; // int

    // glm::vec2
    constexpr DWORD yaw = 0x34; // float
    constexpr DWORD pitch = 0x38; // float

    // glm::vec3
    constexpr DWORD head_posx = 0x4; // float
    constexpr DWORD head_posy = 0x8; // float 
    constexpr DWORD head_posz = 0xC; // float

    // glm::vec3
    constexpr DWORD feet_posx = 0x28; // float
    constexpr DWORD feet_posy = 0x2C; // float
    constexpr DWORD feet_posz = 0x30; // float

#else
	// version 1.2.0.2
	constexpr DWORD player_base = 0x10F4F4; // local player ptr
	constexpr DWORD name = 0x225;

	constexpr DWORD entity_list = 0x10F4F8; // entity list ptr
	constexpr DWORD team = 0x32C;
	constexpr DWORD view_matrix = 0x501AE8;

	constexpr DWORD health = 0xF8; // int
	constexpr DWORD armor = 0xFC; // int
	constexpr DWORD rifle_ammo = 0x150; // int
	constexpr DWORD rifle_cooldown = 0x178; // int

	// glm::vec2
	constexpr DWORD yaw = 0x40; // float
	constexpr DWORD pitch = 0x44; // float

	// glm::vec3
	constexpr DWORD head_posx = 0x4; // float
	constexpr DWORD head_posy = 0x8; // float 
	constexpr DWORD head_posz = 0xC; // float

	// glm::vec3
	constexpr DWORD feet_posx = 0x34; // float
	constexpr DWORD feet_posy = 0x38; // float
	constexpr DWORD feet_posz = 0x3C; // float
#endif
}
