#include "LuaSource.h"

LuaSource::LuaSource(ALuint source) :
	source(source),
	x(0), y(0), z(0),
	vx(0), vy(0), vz(0),
	pitch(1),
	is_looping(false),
	is_relative(false) {
	OpenAL::getInstance()->getSourceDefaults(source, &gain, &max_distance, &rolloff_factor, &reference_distance, &min_gain, &max_gain, &cone_outer_gain, &cone_inner_angle, &cone_outer_angle, &dx, &dy, &dz);
}

int LuaSource::index(lua_State* L) {
	lua_getfield(L, 1, "__userdata");
	LuaSource* luaSource = (LuaSource*)lua_touserdata(L, -1);
	lua_pop(L, 1);

	const char* key = lua_tostring(L, 2);
	switch (hash_string(key)) {
		case HASH_x:
			lua_pushnumber(L, luaSource->x);
			break;
		case HASH_y:
			lua_pushnumber(L, luaSource->y);
			break;
		case HASH_z:
			lua_pushnumber(L, luaSource->z);
			break;

		case HASH_vx:
			lua_pushnumber(L, luaSource->vx);
			break;
		case HASH_vy:
			lua_pushnumber(L, luaSource->vy);
			break;
		case HASH_vz:
			lua_pushnumber(L, luaSource->vz);
			break;

		case HASH_dx:
			lua_pushnumber(L, luaSource->dx);
			break;
		case HASH_dy:
			lua_pushnumber(L, luaSource->dy);
			break;
		case HASH_dz:
			lua_pushnumber(L, luaSource->dz);
			break;

		case HASH_pitch:
			lua_pushnumber(L, luaSource->pitch);
			break;
		case HASH_gain:
			lua_pushnumber(L, luaSource->gain);
			break;
		case HASH_max_distance:
			lua_pushnumber(L, luaSource->max_distance);
			break;
		case HASH_rolloff_factor:
			lua_pushnumber(L, luaSource->rolloff_factor);
			break;
		case HASH_reference_distance:
			lua_pushnumber(L, luaSource->reference_distance);
			break;
		case HASH_min_gain:
			lua_pushnumber(L, luaSource->min_gain);
			break;
		case HASH_max_gain:
			lua_pushnumber(L, luaSource->max_gain);
			break;

		case HASH_cone_outer_gain:
			lua_pushnumber(L, luaSource->cone_outer_gain);
			break;
		case HASH_cone_inner_angle:
			lua_pushnumber(L, luaSource->cone_inner_angle);
			break;
		case HASH_cone_outer_angle:
			lua_pushnumber(L, luaSource->cone_outer_angle);
			break;

		case HASH_is_looping:
			lua_pushboolean(L, luaSource->is_looping);
			break;
		case HASH_is_relative:
			lua_pushboolean(L, luaSource->is_relative);
			break;

		case HASH_state:
			lua_pushstring(L, OpenAL::getInstance()->getSourceState(luaSource->source));
			break;
		case HASH_time:
			lua_pushnumber(L, OpenAL::getInstance()->getSourceTime(luaSource->source));
			break;
	}
	return 1;
}

int LuaSource::newindex(lua_State* L) {
	lua_getfield(L, 1, "__userdata");
	LuaSource* luaSource = (LuaSource*)lua_touserdata(L, -1);
	lua_pop(L, 1);

	const char* key = lua_tostring(L, 2);
	const int valueIndex = 3;
	switch (hash_string(key)) {
		case HASH_x:
			checkNumber(L, valueIndex);
			luaSource->x = lua_tonumber(L, valueIndex);
			OpenAL::getInstance()->setSourcePosition(luaSource->source, luaSource->x, luaSource->y, luaSource->z);
			break;
		case HASH_y:
			checkNumber(L, valueIndex);
			luaSource->y = lua_tonumber(L, valueIndex);
			OpenAL::getInstance()->setSourcePosition(luaSource->source, luaSource->x, luaSource->y, luaSource->z);
			break;
		case HASH_z:
			checkNumber(L, valueIndex);
			luaSource->z = lua_tonumber(L, valueIndex);
			OpenAL::getInstance()->setSourcePosition(luaSource->source, luaSource->x, luaSource->y, luaSource->z);
			break;

		case HASH_vx:
			checkNumber(L, valueIndex);
			luaSource->vx = lua_tonumber(L, valueIndex);
			OpenAL::getInstance()->setSourceVelocity(luaSource->source, luaSource->vx, luaSource->vy, luaSource->vz);
			break;
		case HASH_vy:
			checkNumber(L, valueIndex);
			luaSource->vy = lua_tonumber(L, valueIndex);
			OpenAL::getInstance()->setSourceVelocity(luaSource->source, luaSource->vx, luaSource->vy, luaSource->vz);
			break;
		case HASH_vz:
			checkNumber(L, valueIndex);
			luaSource->vz = lua_tonumber(L, valueIndex);
			OpenAL::getInstance()->setSourceVelocity(luaSource->source, luaSource->vx, luaSource->vy, luaSource->vz);
			break;

		case HASH_dx:
			checkNumber(L, valueIndex);
			luaSource->dx = lua_tonumber(L, valueIndex);
			OpenAL::getInstance()->setSourceDirection(luaSource->source, luaSource->dx, luaSource->dy, luaSource->dz);
			break;
		case HASH_dy:
			checkNumber(L, valueIndex);
			luaSource->dy = lua_tonumber(L, valueIndex);
			OpenAL::getInstance()->setSourceDirection(luaSource->source, luaSource->dx, luaSource->dy, luaSource->dz);
			break;
		case HASH_dz:
			checkNumber(L, valueIndex);
			luaSource->dz = lua_tonumber(L, valueIndex);
			OpenAL::getInstance()->setSourceDirection(luaSource->source, luaSource->dx, luaSource->dy, luaSource->dz);
			break;

		case HASH_pitch:
			checkNumber(L, valueIndex);
			luaSource->pitch = lua_tonumber(L, valueIndex);
			OpenAL::getInstance()->setSourcePitch(luaSource->source, luaSource->pitch);
			break;
		case HASH_gain:
			checkNumber(L, valueIndex);
			luaSource->gain = lua_tonumber(L, valueIndex);
			OpenAL::getInstance()->setSourceGain(luaSource->source, luaSource->gain);
			break;
		case HASH_max_distance:
			checkNumber(L, valueIndex);
			luaSource->max_distance = lua_tonumber(L, valueIndex);
			OpenAL::getInstance()->setSourceMaxDistance(luaSource->source, luaSource->max_distance);
			break;
		case HASH_rolloff_factor:
			checkNumber(L, valueIndex);
			luaSource->rolloff_factor = lua_tonumber(L, valueIndex);
			OpenAL::getInstance()->setSourceRolloffFactor(luaSource->source, luaSource->rolloff_factor);
			break;
		case HASH_reference_distance:
			checkNumber(L, valueIndex);
			luaSource->reference_distance = lua_tonumber(L, valueIndex);
			OpenAL::getInstance()->setSourceReferenceDistance(luaSource->source, luaSource->reference_distance);
			break;
		case HASH_min_gain:
			checkNumber(L, valueIndex);
			luaSource->min_gain = lua_tonumber(L, valueIndex);
			OpenAL::getInstance()->setSourceMinGain(luaSource->source, luaSource->min_gain);
			break;
		case HASH_max_gain:
			checkNumber(L, valueIndex);
			luaSource->max_gain = lua_tonumber(L, valueIndex);
			OpenAL::getInstance()->setSourceMaxGain(luaSource->source, luaSource->max_gain);
			break;

		case HASH_cone_outer_gain:
			checkNumber(L, valueIndex);
			luaSource->cone_outer_gain = lua_tonumber(L, valueIndex);
			OpenAL::getInstance()->setSourceConeOuterGain(luaSource->source, luaSource->cone_outer_gain);
			break;
		case HASH_cone_inner_angle:
			checkNumber(L, valueIndex);
			luaSource->cone_inner_angle = lua_tonumber(L, valueIndex);
			OpenAL::getInstance()->setSourceConeInnerAngle(luaSource->source, luaSource->cone_inner_angle);
			break;
		case HASH_cone_outer_angle:
			checkNumber(L, valueIndex);
			luaSource->cone_outer_angle = lua_tonumber(L, valueIndex);
			OpenAL::getInstance()->setSourceConeOuterAngle(luaSource->source, luaSource->cone_outer_angle);
			break;

		case HASH_is_looping:
			checkBoolean(L, valueIndex);
			luaSource->is_looping = lua_toboolean(L, valueIndex);
			OpenAL::getInstance()->setSourceLooping(luaSource->source, luaSource->is_looping);
			break;
		case HASH_is_relative:
			checkBoolean(L, valueIndex);
			luaSource->is_relative = lua_toboolean(L, valueIndex);
			OpenAL::getInstance()->setSourceRelative(luaSource->source, luaSource->is_relative);
			break;

		case HASH_time:
			checkNumber(L, valueIndex);
			OpenAL::getInstance()->setSourceTime(luaSource->source, lua_tonumber(L, valueIndex));
			break;
	}
	return 0;
}

int LuaSource::play(lua_State* L) {
	checkArgCount(L, 1);
	checkTable(L, 1);

	lua_getfield(L, 1, "__userdata");
	if (lua_type(L, -1) != LUA_TLIGHTUSERDATA) {
		return 0;
	}
	LuaSource* luaSource = (LuaSource*)lua_touserdata(L, -1);
	lua_pop(L, 1);

	OpenAL::getInstance()->playSource(luaSource->source);

	return 0;
}

int LuaSource::pause(lua_State* L) {
	checkArgCount(L, 1);
	checkTable(L, 1);

	lua_getfield(L, 1, "__userdata");
	if (lua_type(L, -1) != LUA_TLIGHTUSERDATA) {
		return 0;
	}
	LuaSource* luaSource = (LuaSource*)lua_touserdata(L, -1);
	lua_pop(L, 1);

	OpenAL::getInstance()->pauseSource(luaSource->source);

	return 0;
}

int LuaSource::rewind(lua_State* L) {
	checkArgCount(L, 1);
	checkTable(L, 1);

	lua_getfield(L, 1, "__userdata");
	if (lua_type(L, -1) != LUA_TLIGHTUSERDATA) {
		return 0;
	}
	LuaSource* luaSource = (LuaSource*)lua_touserdata(L, -1);
	lua_pop(L, 1);

	OpenAL::getInstance()->rewindSource(luaSource->source);

	return 0;
}

int LuaSource::stop(lua_State* L) {
	checkArgCount(L, 1);
	checkTable(L, 1);

	lua_getfield(L, 1, "__userdata");
	if (lua_type(L, -1) != LUA_TLIGHTUSERDATA) {
		return 0;
	}
	LuaSource* luaSource = (LuaSource*)lua_touserdata(L, -1);
	lua_pop(L, 1);

	OpenAL::getInstance()->stopSource(luaSource->source);

	return 0;
}

void LuaSource::push(lua_State* L) {
	// source
	lua_createtable(L, 0, 1);
	// source.__userdata
	lua_pushlightuserdata(L, this);
	lua_setfield(L, -2, "__userdata");

	// source:play()
	lua_pushcfunction(L, play);
	lua_setfield(L, -2, "play");

	// source:pause()
	lua_pushcfunction(L, pause);
	lua_setfield(L, -2, "pause");

	// source:rewind()
	lua_pushcfunction(L, rewind);
	lua_setfield(L, -2, "rewind");

	// source:stop()
	lua_pushcfunction(L, stop);
	lua_setfield(L, -2, "stop");

	// source's metatable
	lua_createtable(L, 0, 2);
	lua_pushcfunction(L, index);
	lua_setfield(L, -2, "__index");
	lua_pushcfunction(L, newindex);
	lua_setfield(L, -2, "__newindex");
	lua_setmetatable(L, -2);
}

LuaSource::~LuaSource() {
	OpenAL::getInstance()->removeSource(source);
}
