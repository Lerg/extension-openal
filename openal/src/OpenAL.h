#ifndef OPENAL_H
#define OPENAL_H

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <algorithm>

#ifndef M_PI
	#define M_PI           3.14159265358979323846
#endif

#include "AL/al.h"
#include "AL/alc.h"
#include "AL/alext.h"

#ifndef DLIB_LOG_DOMAIN
#define DLIB_LOG_DOMAIN "openal"
#include <dmsdk/dlib/log.h>
#endif

#include <dmsdk/sdk.h>
#include "LuaUtils.h"
#include "static_hash.h"

// Init only on Android and HTML5
#if defined(__ANDROID__) || defined(__EMSCRIPTEN__)
	#define SHOULD_INITIALIZE_OPENAL true
#else
	#define SHOULD_INITIALIZE_OPENAL false
#endif

class OpenAL {
private:
	OpenAL();
	~OpenAL();
	OpenAL(OpenAL const&);
	void operator=(OpenAL const&);
	static OpenAL* instance;
	bool is_initializable;
	bool is_initialized;
	bool is_suspended;
	ALCdevice* device;
	ALCcontext* context;
	std::vector<ALuint> sources, suspended_sources;
public:
	static OpenAL* getInstance();
	bool init();
	void setDistanceModel(const char* model);
	float getDopplerFactor();
	void setDopplerFactor(float dopplerFactor);
	float getSpeedOfSound();
	void setSpeedOfSound(float speed);
	void setListenerPosition(float x, float y, float z);
	void setListenerVelocity(float x, float y, float z);
	void setListenerOrientation(float atx, float aty, float atz, float upx, float upy, float upz);
	ALuint newSource(dmBuffer::HBuffer* sourceBuffer);
	void getSourceDefaults(ALuint source, float* gain, float* max_distance, float* rolloff_factor, float* reference_distance, float* min_gain, float* max_gain, float* cone_outer_gain, float* cone_inner_angle, float* cone_outer_angle, float* dx, float* dy, float* dz);
	const char* getSourceState(ALuint source);
	void setSourcePosition(ALuint source, float x, float y, float z);
	void setSourceVelocity(ALuint source, float x, float y, float z);
	void setSourceDirection(ALuint source, float x, float y, float z);
	void setSourcePitch(ALuint source, float pitch);
	void setSourceGain(ALuint source, float gain);
	void setSourceMaxDistance(ALuint source, float max_distance);
	void setSourceRolloffFactor(ALuint source, float rolloff_factor);
	void setSourceReferenceDistance(ALuint source, float reference_distance);
	void setSourceMinGain(ALuint source, float min_gain);
	void setSourceMaxGain(ALuint source, float max_gain);
	void setSourceConeOuterGain(ALuint source, float cone_outer_gain);
	void setSourceConeInnerAngle(ALuint source, float cone_inner_angle);
	void setSourceConeOuterAngle(ALuint source, float cone_outer_angle);
	void setSourceLooping(ALuint source, bool looping);
	void setSourceRelative(ALuint source, bool relative);
	float getSourceTime(ALuint source);
	void setSourceTime(ALuint source, float seconds);
	void playSource(ALuint source);
	void pauseSource(ALuint source);
	void rewindSource(ALuint source);
	void stopSource(ALuint source);
	void removeSource(ALuint source);
	void suspend();
	void resume();
	void close();
};

#endif

