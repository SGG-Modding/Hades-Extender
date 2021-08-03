#pragma once

namespace Core::Lua
{
	/* Global access */
	LuaWrapper* extender = nullptr;

	/* Initialize our hook callbacks */
	BOOL InitializeCallbacks()
	{
		Core::Hooks::on_world_update = [=](float dt, bool post)
		{
			const char* cb_name = "OnUpdate";
			if (post)
				cb_name = "OnPostUpdate";
			lua_State* L = extender->State();
			/* TODO: Somehow wrap this */
			lua_getglobal(L, GLOBAL_TABLE);
			lua_getfield(L, -1, cb_name);
			if (!lua_isnil(L, -1))
			{
				lua_pushnumber(L, dt);
				if (lua_pcall(L, 1, 0, 0))
					DEBUG("[ Extender ] Critical Error! %s\n", lua_tostring(L, -1));

				lua_pop(L, 1);
			}
		};

		Core::Hooks::on_wndproc = [=](HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
		{
			/*if (!extender || !extender->Initialized())
				return;

			lua_State* L = extender->State();
			if (hwnd == GetFocus())
			{
				lua_getglobal(L, GLOBAL_TABLE);
				lua_getfield(L, -1, "OnWndProc");
				if (!lua_isnil(L, -1))
				{
					lua_pushnumber(L, msg);
					lua_pushnumber(L, wparam);
					lua_pushnumber(L, lparam);
					if (lua_pcall(L, 3, 0, 0) != 0)
					{
						DEBUG("[ Extender ] Error running OnWndProc: %s\n", lua_tostring(L, -1));
					}
					lua_pop(L, 3);
				}
			}*/
		};

		return TRUE;
	}

	/* Initialize all of our events */
	BOOL Initialize()
	{
		/* Initialize our wrapper */
		extender = new LuaWrapper();

		/* Set lua initialization event */
		Core::Hooks::on_lua_init = [=]()
		{
			return;
			DEBUG("[ Extender ] Initializing lua...\n");
			/* Set thread guard */
			_InterlockedCompareExchange64((LONG64*)Engine::Addresses::Game::thread_guard, GetCurrentThreadId(), 0);

			/* Call our `OnInit` function */
			extender->OnInit();
			/* Re-open the lua debug library to restore the `debug.sethook` function and copy it. */
			reinterpret_cast<int(__fastcall*)(lua_State*)>(Engine::Addresses::Lua::Functions::open_debug)(extender->State());
			extender->DoString("Extender.SetHook = debug.sethook\ndebug.sethook = function(...) end");

			DEBUG("[ Extender ] Finished initializing lua!\n");

			/* Test buffer */
			const char* buffer = R"(

				print('Hello from Lua Extender!')
				local x,y  = Extender.GetCursorPosition()
				print(tostring(x))
				print(tostring(y))
				)";

			extender->DoString(buffer);

			/* We have finished all of our initialization, load original files! */
			//extender->LoadFile("Main.lua");
			//extender->LoadFile("RoomManager.lua");
			/* Reset thread guard */
			_InterlockedExchange64((LONG64*)Engine::Addresses::Game::thread_guard, 0);
		};

		/* Set lua pre initialization event */
		Core::Hooks::on_lua_pre_init = [=]()
		{
			DEBUG("[ Extender ] Marking extension as reset...\n");
			extender->Reset();
		};

		/* Set lua load file event */
		Core::Hooks::on_lua_load_file = [=](const char* file_name)
		{
			/* Do not allow loading of any files until extender has finished initializing! */
			if (strcmp(file_name, "Main.lua") == 0)
			{
				DEBUG("[ Extender ] Initializing lua...\n");
				/* Set thread guard */
				_InterlockedCompareExchange64((LONG64*)Engine::Addresses::Game::thread_guard, GetCurrentThreadId(), 0);

				/* Call our `OnInit` function */
				extender->OnInit();
				/* Re-open the lua debug library to restore the `debug.sethook` function and copy it. */
				reinterpret_cast<int(__fastcall*)(lua_State*)>(Engine::Addresses::Lua::Functions::open_debug)(extender->State());
				extender->DoString("Extender.SetHook = debug.sethook\ndebug.sethook = function(...) end");

				DEBUG("[ Extender ] Finished initializing lua!\n");

				/* Test buffer */
				const char* buffer = R"(

				print('Hello from Lua Extender!')
				local x,y  = Extender.GetCursorPosition()
				print(tostring(x))
				print(tostring(y))
				)";

				extender->DoString(buffer);

				/* We have finished all of our initialization, load original files! */
				//extender->LoadFile("Main.lua");
				//extender->LoadFile("RoomManager.lua");
				/* Reset thread guard */
				_InterlockedExchange64((LONG64*)Engine::Addresses::Game::thread_guard, 0);

				/* Enable Wndproc hook */
				Core::Hooks::HookWndProc();
			}
		};

		/* Begin adding our functions */

		/* TODO: Possibly add helper functions for lua stack functions */

		/* `Extender.GetAsyncKeyState` */
		extender->AddFunction("GetAsyncKeyState", [](lua_State* L) -> int
			{
				int vk = lua_tonumber(L, 1);
				lua_pushnumber(L, GetAsyncKeyState(vk));
				return 1;
			});

		/* `Extender.IsKeyDown` */
		extender->AddFunction("IsKeyDown", [](lua_State* L) -> int
			{
				int vk = lua_tonumber(L, 1);
				lua_pushnumber(L, GetAsyncKeyState(vk) & 0x8000);
				return 1;
			});

		/* `Extender.GetCursorPosition` */
		extender->AddFunction("GetCursorPosition", [](lua_State* L) -> int
			{
				Engine::Cursor* cursor = Engine::Cursor::Instance();
				if (!cursor)
				{
					/* Cursor pointer is not valid or created yet! */
					lua_pushnumber(L, -1);
					lua_pushnumber(L, -1);
					return 2;
				}
				/* TODO: Add Vector2 */
				lua_pushnumber(L, cursor->pos_x);
				lua_pushnumber(L, cursor->pos_y);
				return 2;
			});

		/* End adding our functions */
		/* Begin adding our callbacks */

		extender->AddCallback("OnWndProc");
		extender->AddCallback("OnUpdate");
		extender->AddCallback("OnPostUpdate");

		/* End adding our callbacks */

		/* Initialize callback events */
		if (!InitializeCallbacks())
			return FALSE;

		return TRUE; /* We have no real conditions right now...*/
	}
}