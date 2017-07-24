// Copyright 2017 Sergey Lerg

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#if defined(DM_PLATFORM_WINDOWS)
	#include <io.h>
#else
	#include <unistd.h>
#endif

#define LIB_NAME "openal"
#define MODULE_NAME LIB_NAME

#include "OpenAL.h"
#include "LuaListener.h"
#include "LuaSource.h"

static int extension_get_info(lua_State* L) {
	const char* al_vendor = alGetString(AL_VENDOR);
	const char* al_version = alGetString(AL_VERSION);
	const char* al_renderer = alGetString(AL_RENDERER);
	const char* al_extensions = alGetString(AL_EXTENSIONS);

	lua_createtable(L, 0, 4);

	lua_pushstring(L, al_vendor);
    lua_setfield(L, -2, "vendor");

	lua_pushstring(L, al_version);
    lua_setfield(L, -2, "version");

	lua_pushstring(L, al_renderer);
    lua_setfield(L, -2, "renderer");

	lua_pushstring(L, al_extensions);
    lua_setfield(L, -2, "extensions");

	return 1;
}

static int extension_set_distance_model(lua_State* L) {
	checkArgCount(L, 1);
	checkString(L, 1);
	OpenAL::getInstance()->setDistanceModel(lua_tostring(L, 1));
	return 0;
}

static int extension_new_source(lua_State* L) {
	checkArgCount(L, 1);
	checkBuffer(L, 1);
	dmScript::LuaHBuffer* sourceLuaBuffer = dmScript::CheckBuffer(L, 1);
	ALuint source = OpenAL::getInstance()->newSource(&sourceLuaBuffer->m_Buffer);
	if (source == 0) {
		lua_pushnil(L);
	} else {
		LuaSource* luaSource = new LuaSource(source);
		luaSource->push(L);
	}
	return 1;
}

static int extension_remove_source(lua_State* L) {
	checkArgCount(L, 1);
	// Ignore nil sources, might not be created at all.
	if (lua_isnil(L, 1)) {
		dmLogInfo("Attempt to remove nil sound source.\n");
		return 0;
	}
	checkTable(L, 1);

	lua_getfield(L, 1, "__userdata");
	if (lua_type(L, -1) != LUA_TLIGHTUSERDATA) {
		return 0;
	}
	LuaSource* luaSource = (LuaSource*)lua_touserdata(L, -1);
	lua_pop(L, 1);

	// remove source.__userdata
	lua_pushnil(L);
	lua_setfield(L, -2, "__userdata");

	delete luaSource;
	return 0;
}

int extension_index(lua_State* L) {
	const char* key = lua_tostring(L, 2);
	switch (hash_string(key)) {
		case HASH_doppler_factor:
			lua_pushnumber(L, OpenAL::getInstance()->getDopplerFactor());
			break;
		case HASH_speed_of_sound:
			lua_pushnumber(L, OpenAL::getInstance()->getSpeedOfSound());
			break;
	}
	return 1;
}

int extension_newindex(lua_State* L) {
	const char* key = lua_tostring(L, 2);
	const int valueIndex = 3;
	switch (hash_string(key)) {
		case HASH_doppler_factor: {
			checkNumber(L, valueIndex);
			float value = lua_tonumber(L, valueIndex);
			if (value > 0) {
				OpenAL::getInstance()->setDopplerFactor(value);
			}
			break;
		}
		case HASH_speed_of_sound: {
			checkNumber(L, valueIndex);
			float value = lua_tonumber(L, valueIndex);
			if (value > 0) {
				OpenAL::getInstance()->setSpeedOfSound(value);
			}
			break;
		}
	}
	return 0;
}

static const luaL_reg Module_methods[] = {
	{"get_info", extension_get_info},
	{"set_distance_model", extension_set_distance_model},
	{"new_source", extension_new_source},
	{"remove_source", extension_remove_source},
	{0, 0}
};

static void LuaInit(lua_State* L) {
	int top = lua_gettop(L);
	luaL_register(L, MODULE_NAME, Module_methods);

	// extension.listener
	LuaListener::getInstance()->push(L);
	lua_setfield(L, -2, "listener");

	// extension's metatable
	lua_createtable(L, 0, 2);
	lua_pushcfunction(L, extension_index);
	lua_setfield(L, -2, "__index");
	lua_pushcfunction(L, extension_newindex);
	lua_setfield(L, -2, "__newindex");
	lua_setmetatable(L, -2);

	lua_pop(L, 1);
	assert(top == lua_gettop(L));
}

dmExtension::Result AppInitializeOpenAL(dmExtension::AppParams* params) {
	return dmExtension::RESULT_OK;
}

dmExtension::Result InitializeOpenAL(dmExtension::Params* params) {
	LuaInit(params->m_L);
	bool result = OpenAL::getInstance()->init();
	return result ? dmExtension::RESULT_OK : dmExtension::RESULT_INIT_ERROR;
}

dmExtension::Result AppFinalizeOpenAL(dmExtension::AppParams* params) {
	return dmExtension::RESULT_OK;
}

void OnEventOpenAL(dmExtension::Params* params, const dmExtension::Event* event) {
	switch (event->m_Event) {
		case dmExtension::EVENT_ID_ACTIVATEAPP:
			OpenAL::getInstance()->resume();
			break;
		case dmExtension::EVENT_ID_DEACTIVATEAPP:
			OpenAL::getInstance()->suspend();
			break;
	}
}

dmExtension::Result FinalizeOpenAL(dmExtension::Params* params) {
	OpenAL::getInstance()->close();
	return dmExtension::RESULT_OK;
}

DM_DECLARE_EXTENSION(openal, LIB_NAME, AppInitializeOpenAL, AppFinalizeOpenAL, InitializeOpenAL, 0, OnEventOpenAL, FinalizeOpenAL)
