#ifndef LUALISTENER_H
#define LUALISTENER_H

#include "OpenAL.h"

class LuaListener {
private:
	LuaListener();
	~LuaListener();
	LuaListener(LuaListener const&);
	void operator=(LuaListener const&);
	static LuaListener* instance;
	float x;
	float y;
	float z;
	float vx;
	float vy;
	float vz;
	float atx;
	float aty;
	float atz;
	float upx;
	float upy;
	float upz;
	static int index(lua_State* L);
	static int newindex(lua_State* L);
public:
	static LuaListener* getInstance(void);
	void push(lua_State* L);
};

#endif
