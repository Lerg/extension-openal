#ifndef LUASOURCE_H
#define LUASOURCE_H

#include "OpenAL.h"

class LuaSource {
private:
	ALuint source;
	float x;
	float y;
	float z;
	float vx;
	float vy;
	float vz;
	float dx;
	float dy;
	float dz;
	bool is_looping;
	float pitch;
	float gain;
	float max_distance;
	float rolloff_factor;
	float reference_distance;
	float min_gain;
	float max_gain;
	float cone_outer_gain;
	float cone_inner_angle;
	float cone_outer_angle;
	bool is_relative;
	static int index(lua_State* L);
	static int newindex(lua_State* L);
	static int play(lua_State* L);
	static int pause(lua_State* L);
	static int rewind(lua_State* L);
	static int stop(lua_State* L);

public:
	LuaSource(ALuint source);
	void push(lua_State* L);
	~LuaSource();
};

#endif
