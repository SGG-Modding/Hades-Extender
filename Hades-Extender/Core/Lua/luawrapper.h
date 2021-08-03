#pragma once


/* Helper */
class LuaWrapper
{
private:
	/* Our actual state */
	lua_State* L;
	/* List of methods that belong in the global table */
	std::vector<LuaFunction*> functions;
	std::vector<LuaFunction*> callbacks;
public:
	/* Constructor */
	LuaWrapper()
	{
		this->L = *(lua_State**)(Engine::Addresses::Lua::state_instance);
	}

	/* Deconstructor */
	~LuaWrapper()
	{
		/* Deconstruct all function pointers */
		for (auto& function : this->functions)
			delete function; /* Free memory! */
	}
public:
	/* Accessor */
	lua_State* State()
	{
		return this->L;
	}
public:
	/* Methods */
	/* Creates a new function instance which is added to the function list */
	VOID AddFunction(const char* name, lua_CFunction cb)
	{
		LuaFunction* func = new LuaFunction(name, (PVOID)cb);
		this->functions.push_back(func);
	}

	/* Creates a new function instance with no real function to be added to the callback list*/
	VOID AddCallback(const char* name)
	{
		LuaFunction* func = new LuaFunction(name, nullptr);
		this->callbacks.push_back(func);
	}

	VOID DoCallback(const char* name, ...)
	{
		//...
	}

	/* Pushes all functions and callbacks onto the `Extender` global table */
	/* Must be called every time the engine reinitializes the lua instance */
	VOID OnInit()
	{
		/* Ensure our state pointer is updated */
		this->L = *(lua_State**)(Engine::Addresses::Lua::state_instance);

		/* Create new table */
		lua_newtable(L);
		{
			/* Push all functions */
			for (auto& function : this->functions)
			{
				/* Push function pointer */
				lua_pushcfunction(L, function->GetCallback());
				/* Assign name */
				lua_setfield(L, -2, function->GetName());
			}

			/* Push all callbacks */
			for (auto& callback : this->callbacks)
			{
				/* Set the value to nil */
				lua_pushnil(L);
				lua_setfield(L, -2, callback->GetName());
			}
		}
		/* Assign table to global name */
		/* Note: Maybe we can construct `LuaWrapper` with the name of the table? Allow us to have multiple */
		lua_setglobal(L, GLOBAL_TABLE);
	}
};