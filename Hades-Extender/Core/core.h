#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMCX
#define NOIME
#define NOSERVICE
#define NOKANJI
// win
#include <Windows.h>
#include <winternl.h>
#include <Xinput.h>
#include <d3d11.h>
#include <comdef.h>
#include <minwindef.h>
#include <winbase.h>
#include <intrin.h>
#include <mutex>
#include <cassert>
#include <psapi.h>

#pragma comment(lib, "d3d11.lib")

// stl
#include <vector>
#include <deque>
#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <cctype>
#include <clocale>
#include <algorithm>
#include <sstream>
#include <map>
#include <functional>

/* Utilities */
#include "../Utilities/xorstr.h"
#include "../Utilities/memory.h"
#include "../Utilities/macros.h"

/* Engine */
#include "../Engine/engine.h"

/* Proxy system */
#include "./proxy.h"

/* Hooks */
#include "./Hooks/hooks.h"

/* Lua */
#include "./lua/lua.h"

namespace Core
{
	/* Initialize all of our events and subsystems */
	BOOL Initialize( VOID )
	{
		DEBUG("[ + ] Initializing Core...\n");
		/* Initialize our proxy system to prevent crashes from DirectX */
		if (!proxy.Initialize())
		{
			DEBUG("[ ! ] Failed to initialize proxy!\n");
		}
		/* Call hook system initialization */
		DEBUG("[ + ] Hooking functions...\n");
		if (!Core::Hooks::Initialize())
			return FALSE;
		DEBUG("[ + ] Initializing lua subsystem...\n");
		if (!Core::Lua::Initialize())
		{
			DEBUG("[ ! ] Failed to initialize lua!\n");
			return FALSE;
		}

		DEBUG("[ + ] Setting events...\n");

		/* Everything looks good! */
		return TRUE;
	}
}