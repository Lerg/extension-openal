#include "LuaListener.h"

LuaListener* LuaListener::instance = NULL;

LuaListener::LuaListener(void) :
	x(0), y(0), z(0),
	vx(0), vy(0), vz(0) {
}

LuaListener* LuaListener::getInstance(void) {
	if (!instance) {
		instance = new LuaListener();
	}
	return instance;
}

int LuaListener::index(lua_State* L) {
	LuaListener* luaListener = LuaListener::getInstance();

	const char* key = lua_tostring(L, 2);
	switch (hash_string(key)) {
		case HASH_x:
			lua_pushnumber(L, luaListener->x);
			break;
		case HASH_y:
			lua_pushnumber(L, luaListener->y);
			break;
		case HASH_z:
			lua_pushnumber(L, luaListener->z);
			break;
		case HASH_vx:
			lua_pushnumber(L, luaListener->vx);
			break;
		case HASH_vy:
			lua_pushnumber(L, luaListener->vy);
			break;
		case HASH_vz:
			lua_pushnumber(L, luaListener->vz);
			break;
		case HASH_orientation_at_x:
			lua_pushnumber(L, luaListener->atx);
			break;
		case HASH_orientation_at_y:
			lua_pushnumber(L, luaListener->aty);
			break;
		case HASH_orientation_at_z:
			lua_pushnumber(L, luaListener->atz);
			break;
		case HASH_orientation_up_x:
			lua_pushnumber(L, luaListener->upx);
			break;
		case HASH_orientation_up_y:
			lua_pushnumber(L, luaListener->upy);
			break;
		case HASH_orientation_up_z:
			lua_pushnumber(L, luaListener->upz);
			break;
	}
	return 1;
}

int LuaListener::newindex(lua_State* L) {
	LuaListener* luaListener = LuaListener::getInstance();

	const char* key = lua_tostring(L, 2);
	const int valueIndex = 3;
	switch (hash_string(key)) {
		case HASH_x:
			checkNumber(L, valueIndex);
			luaListener->x = lua_tonumber(L, valueIndex);
			OpenAL::getInstance()->setListenerPosition(luaListener->x, luaListener->y, luaListener->z);
			break;
		case HASH_y:
			checkNumber(L, valueIndex);
			luaListener->y = lua_tonumber(L, valueIndex);
			OpenAL::getInstance()->setListenerPosition(luaListener->x, luaListener->y, luaListener->z);
			break;
		case HASH_z:
			checkNumber(L, valueIndex);
			luaListener->z = lua_tonumber(L, valueIndex);
			OpenAL::getInstance()->setListenerPosition(luaListener->x, luaListener->y, luaListener->z);
			break;

		case HASH_vx:
			checkNumber(L, valueIndex);
			luaListener->vx = lua_tonumber(L, valueIndex);
			OpenAL::getInstance()->setListenerVelocity(luaListener->vx, luaListener->vy, luaListener->vz);
			break;
		case HASH_vy:
			checkNumber(L, valueIndex);
			luaListener->vy = lua_tonumber(L, valueIndex);
			OpenAL::getInstance()->setListenerVelocity(luaListener->vx, luaListener->vy, luaListener->vz);
			break;
		case HASH_vz:
			checkNumber(L, valueIndex);
			luaListener->vz = lua_tonumber(L, valueIndex);
			OpenAL::getInstance()->setListenerVelocity(luaListener->vx, luaListener->vy, luaListener->vz);
			break;

		case HASH_orientation_at_x:
			checkNumber(L, valueIndex);
			luaListener->atx = lua_tonumber(L, valueIndex);
			OpenAL::getInstance()->setListenerOrientation(luaListener->atx, luaListener->aty, luaListener->atz, luaListener->upx, luaListener->upy, luaListener->upz);
			break;
		case HASH_orientation_at_y:
			checkNumber(L, valueIndex);
			luaListener->aty = lua_tonumber(L, valueIndex);
			OpenAL::getInstance()->setListenerOrientation(luaListener->atx, luaListener->aty, luaListener->atz, luaListener->upx, luaListener->upy, luaListener->upz);
			break;
		case HASH_orientation_at_z:
			checkNumber(L, valueIndex);
			luaListener->atz = lua_tonumber(L, valueIndex);
			OpenAL::getInstance()->setListenerOrientation(luaListener->atx, luaListener->aty, luaListener->atz, luaListener->upx, luaListener->upy, luaListener->upz);
			break;

		case HASH_orientation_up_x:
			checkNumber(L, valueIndex);
			luaListener->upx = lua_tonumber(L, valueIndex);
			OpenAL::getInstance()->setListenerOrientation(luaListener->atx, luaListener->aty, luaListener->atz, luaListener->upx, luaListener->upy, luaListener->upz);
			break;
		case HASH_orientation_up_y:
			checkNumber(L, valueIndex);
			luaListener->upy = lua_tonumber(L, valueIndex);
			OpenAL::getInstance()->setListenerOrientation(luaListener->atx, luaListener->aty, luaListener->atz, luaListener->upx, luaListener->upy, luaListener->upz);
			break;
		case HASH_orientation_up_z:
			checkNumber(L, valueIndex);
			luaListener->upz = lua_tonumber(L, valueIndex);
			OpenAL::getInstance()->setListenerOrientation(luaListener->atx, luaListener->aty, luaListener->atz, luaListener->upx, luaListener->upy, luaListener->upz);
			break;
	}
	return 0;
}

void LuaListener::push(lua_State* L) {
	// listener
	lua_createtable(L, 0, 1);

	// listener's metatable
	lua_createtable(L, 0, 2);
	lua_pushcfunction(L, index);
	lua_setfield(L, -2, "__index");
	lua_pushcfunction(L, newindex);
	lua_setfield(L, -2, "__newindex");
	lua_setmetatable(L, -2);
}
