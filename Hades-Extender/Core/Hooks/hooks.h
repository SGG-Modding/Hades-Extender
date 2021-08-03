/*
	Our primary hook file. We do not really have many hooks right now so I am keeping all of them in this file. - 08-02-21
*/
#pragma once
#include <minhook/include/MinHook.h>

namespace Core::Hooks
{
	/* Window Handle */
	HWND wnd;

	/* Our custom callback function pointers */
	std::function<void()> on_lua_init									= nullptr;
	std::function<void()> on_lua_pre_init								= nullptr;
	std::function<void(const char*)> on_lua_load_file					= nullptr;
	std::function<void(float /*dt*/, bool /*post*/)> on_world_update	= nullptr;
	std::function<void(HWND, UINT, WPARAM, LPARAM)> on_wndproc			= nullptr;


	/* Original function pointers used for returning to original flow */
	PVOID original_lua_init		= nullptr;
	PVOID original_lua_load_file= nullptr;
	PVOID original_world_init	= nullptr;
	WNDPROC original_wndproc	= nullptr;

	/* Raw callbacks */
	/* `sgg::ScriptManager::InitLua` */
	VOID __fastcall hook_init_lua( VOID )
	{
		DEBUG("[ Extender ] Lua is being initialized by the engine...\n");
		/* Call our `pre-initialization` event */
		if (on_lua_pre_init)
			on_lua_pre_init();
		/* Execute original lua initialization */
		reinterpret_cast<void(__fastcall*)()>(original_lua_init)();
		DEBUG("[ Extender ] Initializing custom lua parameters...\n");
		/* Check if our callback exists and execute if it does */
		if (on_lua_init)
			on_lua_init();

		DEBUG("[ Extender ] Finished initializing custom lua parameters.\n");
	}

	/* `sgg::ScriptManager::LoadFile` */
	VOID __fastcall hook_lua_load_file(const char* file_name)
	{
		if (on_lua_load_file)
			on_lua_load_file(file_name);
		return reinterpret_cast<VOID(__fastcall*)(const char*)>(original_lua_load_file)(file_name);
	}

	/* `WndProc` */
	LRESULT WINAPI wnd_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (on_wndproc)
			on_wndproc(hWnd, msg, wParam, lParam);
		return CallWindowProc(original_wndproc, hWnd, msg, wParam, lParam);
	}


	/* `sgg::World::Update(sgg::World *this, float elapsedSeconds)` */
	VOID __fastcall hook_world_update(class World* world, FLOAT dt)
	{
		if (on_world_update)
			on_world_update(dt, false);
		/* Execute original function */
		reinterpret_cast<void(__fastcall*)(class World*, FLOAT)>(original_world_init)(world, dt);
		if (on_world_update)
			on_world_update(dt, true);
	}

	VOID HookWndProc( VOID )
	{
		/* Add listener for WndProc */
		original_wndproc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(wnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(wnd_proc)));
	}

	VOID UnhookWndProc( VOID )
	{
		SetWindowLongPtr(wnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(original_wndproc));
	}

	/* Initialize all of our hooks using `MinHook` */
	BOOL Initialize( VOID )
	{
		/* Call MinHook initialization routine */
		if (MH_Initialize())
			return FALSE;

		/* Create our hooks */
		MH_CreateHook((PVOID)Engine::Addresses::Lua::Functions::init_lua, &hook_init_lua, &original_lua_init);
		MH_CreateHook((PVOID)Engine::Addresses::Lua::Functions::load_file, &hook_lua_load_file, &original_lua_load_file);
		MH_CreateHook((PVOID)Engine::Addresses::World::Functions::update, &hook_world_update, &original_world_init);

		/* Enable all hooks we have created */
		MH_EnableHook(MH_ALL_HOOKS);

		/* Get Window Handle */
		wnd = FindWindowA("The Forge", NULL);
		return TRUE;
	}
}