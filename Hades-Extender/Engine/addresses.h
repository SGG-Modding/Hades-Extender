#pragma once

/* Our main file used to gather information on addresses and functions/instances */
/* We obtain all offsets to these through instruction signature scanning to prevent issues when the game update */

namespace Engine
{
	namespace Addresses
	{
		namespace Lua
		{
			namespace Functions
			{
				/* Called whenever the lua state is 'reset' */
				DWORD64 CONST init_lua = Memory::GetCallAddress("E8 ? ? ? ? 48 8B 1D ? ? ? ? 48 8D 3D");
			}

			/* Lua state pointer */
			DWORD64 CONST state_instance = Memory::GetInstanceAddress("48 8B 0D ? ? ? ? 48 85 C9 75 03");
		}

		namespace SDL
		{
			namespace Functions
			{
				DWORD64 CONST unload_xinput = (DWORD64)GetModuleHandleA("SDL2.exe") + 0x00DD19; // TODO: Signature scan!
			}
		}

		namespace World
		{
			namespace Functions
			{
				/* Called every world frame */
				DWORD64 CONST update = Memory::GetCallAddress("E8 ? ? ? ? 0F 28 CE 48 8B 0D ? ? ? ? E8 ? ? ? ? 48 0F BE 0D");
			}
		}

		namespace CursorManager
		{

			DWORD64 CONST instance = Memory::GetInstanceAddress("48 8D 0D ? ? ? ? 48 8B 04 C1 F3 44 0F 10 50");
		}
	}
}