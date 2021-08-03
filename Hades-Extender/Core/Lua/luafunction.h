#pragma once

/* Helper function to define functions */
class LuaFunction
{
private:
	/* Fields */
	const char* name;
	void* callback;
public:
	/* Constructor */
	LuaFunction(const char* name, void* cb) : name(name)
	{
		this->callback = cb;
	}

public:
	/* Public methods */
	const char* GetName()
	{
		return this->name;
	}

	lua_CFunction GetCallback()
	{
		return (lua_CFunction)this->callback;
	}
};