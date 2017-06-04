#include "OpenAL.h"
#ifdef __EMSCRIPTEN__
	#include "compatibility.h"
#endif
/*
 * Struct that holds the RIFF data of the Wave file.
 * The RIFF data is the meta data information that holds,
 * the ID, size and format of the wave file
 */

struct RIFF_Header {
	char chunkID[4];
	unsigned int chunkSize; //size not including chunkSize or chunkID
	char format[4];
};

/*
 * Struct to hold fmt subchunk data for WAVE files.
 */
struct WAVE_Format {
	char subChunkID[4];
	unsigned int subChunkSize;
	unsigned short audioFormat;
	unsigned short numChannels;
	unsigned int sampleRate;
	unsigned int byteRate;
	unsigned short blockAlign;
	unsigned short bitsPerSample;
};

/*
* Struct to hold the data of the wave file
*/
struct WAVE_Data {
	char subChunkID[4]; //should contain the word data
	unsigned int subChunk2Size; //Stores the size of the data block
};

/*
 * Load wave file function. No need for ALUT with this
 */
static ALuint loadWavFile(dmBuffer::HBuffer* sourceBuffer) {
	uint8_t* sourcedata = 0;
	uint32_t datasize = 0;
	dmBuffer::GetBytes(*sourceBuffer, (void**)&sourcedata, &datasize);

	//Local Declarations
	WAVE_Format wave_format;
	RIFF_Header riff_header;
	WAVE_Data wave_data;
	unsigned char* data;

	int cursor = 0;
	// Read in the first chunk into the struct
	int field_size = sizeof(riff_header.chunkID);
	memcpy(&riff_header.chunkID, &sourcedata[cursor], field_size);
	cursor += field_size;

	field_size = sizeof(riff_header.chunkSize);
	memcpy(&riff_header.chunkSize, &sourcedata[cursor], field_size);
	cursor += field_size;

	field_size = sizeof(riff_header.format);
	memcpy(&riff_header.format, &sourcedata[cursor], field_size);
	cursor += field_size;

	//check for RIFF and WAVE tag in memeory
	if ((strncmp(riff_header.chunkID, "RIFF", 4) != 0) && (strncmp(riff_header.chunkID, "WAVE", 4) != 0)) {
		dmLogError("Invalid WAV RIFF or WAVE header\n");
		return 0;
	}

	//Read in the 2nd chunk for the wave info
	field_size = sizeof(wave_format.subChunkID);
	memcpy(&wave_format.subChunkID, &sourcedata[cursor], field_size);
	cursor += field_size;

	field_size = sizeof(wave_format.subChunkSize);
	memcpy(&wave_format.subChunkSize, &sourcedata[cursor], field_size);
	cursor += field_size;

	field_size = sizeof(wave_format.audioFormat);
	memcpy(&wave_format.audioFormat, &sourcedata[cursor], field_size);
	cursor += field_size;

	field_size = sizeof(wave_format.numChannels);
	memcpy(&wave_format.numChannels, &sourcedata[cursor], field_size);
	cursor += field_size;

	field_size = sizeof(wave_format.sampleRate);
	memcpy(&wave_format.sampleRate, &sourcedata[cursor], field_size);
	cursor += field_size;

	field_size = sizeof(wave_format.byteRate);
	memcpy(&wave_format.byteRate, &sourcedata[cursor], field_size);
	cursor += field_size;

	field_size = sizeof(wave_format.blockAlign);
	memcpy(&wave_format.blockAlign, &sourcedata[cursor], field_size);
	cursor += field_size;

	field_size = sizeof(wave_format.bitsPerSample);
	memcpy(&wave_format.bitsPerSample, &sourcedata[cursor], field_size);
	cursor += field_size;

	//check for fmt tag in memory
	if (strncmp(wave_format.subChunkID, "fmt ", 4) != 0) {
		dmLogError("Invalid WAV fmt header.\n");
		return 0;
	}

	//check for extra parameters;
	if (wave_format.subChunkSize > 16) {
		cursor += sizeof(short);
	}

	// Skip all additional chunks until data chunk is found
	bool data_found = false;
	do {
		//Read in the the last byte of data before the sound file
		field_size = sizeof(wave_data.subChunkID);
		memcpy(&wave_data.subChunkID, &sourcedata[cursor], field_size);
		cursor += field_size;

		field_size = sizeof(wave_data.subChunk2Size);
		memcpy(&wave_data.subChunk2Size, &sourcedata[cursor], field_size);
		cursor += field_size;

		//check for data tag in memory
		if (strncmp(wave_data.subChunkID, "data", 4) == 0) {
			data_found = true;
		} else if (cursor >= datasize - sizeof(wave_data)) {
			dmLogError("Invalid WAV data header.\n");
			return 0;
		}
	} while (!data_found);

	//Allocate memory for data
	data = new unsigned char[wave_data.subChunk2Size];

	// Read in the sound data into the soundData variable
	memcpy(data, &sourcedata[cursor], wave_data.subChunk2Size);

	//Now we set the variables that we passed in with the
	//data from the structs
	ALsizei size = wave_data.subChunk2Size;
	ALsizei frequency = wave_format.sampleRate;
	ALenum format = AL_FORMAT_MONO8;
	//The format is worked out by looking at the number of
	//channels and the bits per sample.
	if (wave_format.numChannels == 1) {
		if (wave_format.bitsPerSample == 8) {
			format = AL_FORMAT_MONO8;
		} else if (wave_format.bitsPerSample == 16) {
			format = AL_FORMAT_MONO16;
		}
	} else if (wave_format.numChannels == 2) {
		if (wave_format.bitsPerSample == 8) {
			format = AL_FORMAT_STEREO8;
		} else if (wave_format.bitsPerSample == 16) {
			format = AL_FORMAT_STEREO16;
		}
	}
	//create our openAL buffer and check for success
	ALuint buffer = 0;
	alGenBuffers(1, &buffer);
	ALenum err = alGetError();
	if (err != AL_NO_ERROR) {
		dmLogError("OpenAL Error: %s\n", alGetString(err));
	}

	//now we put our data into the openAL buffer and
	//check for success
	alBufferData(buffer, format, (void*)data, size, frequency);
	delete data;

	/* Check if an error occured, and clean up if so. */
	err = alGetError();
	if (err != AL_NO_ERROR) {
		dmLogError("OpenAL Error: %s\n", alGetString(err));
		if (alIsBuffer(buffer)) {
			alDeleteBuffers(1, &buffer);
		}
		return 0;
	}

	return buffer;
}

enum WaveType {
	WT_Sine,
	WT_Square,
	WT_Sawtooth,
	WT_Triangle,
	WT_Impulse,
};

static void ApplySin(ALfloat *data, ALdouble g, ALuint srate, ALuint freq) {
	ALdouble smps_per_cycle = (ALdouble)srate / freq;
	ALuint i;
	for (i = 0;i < srate;i++) {
		data[i] += (ALfloat)(sin(i/smps_per_cycle * 2.0*M_PI) * g);
	}
}

/* Generates waveforms using additive synthesis. Each waveform is constructed
 * by summing one or more sine waves, up to (and excluding) nyquist.
 */
 // TODO: expose creating synths
static ALuint CreateWave(enum WaveType type, ALuint freq, ALuint srate) {
	ALint data_size;
	ALfloat* data;
	ALuint buffer;
	ALenum err;
	ALuint i;

	data_size = srate * sizeof(ALfloat);
	data = (ALfloat*)calloc(1, data_size);
	if (type == WT_Sine) {
		ApplySin(data, 1.0, srate, freq);
	} else if (type == WT_Square) {
		for (i = 1;freq*i < srate/2;i+=2) {
			ApplySin(data, 4.0/M_PI * 1.0/i, srate, freq*i);
		}
	} else if (type == WT_Sawtooth) {
		for (i = 1;freq*i < srate/2;i++) {
			ApplySin(data, 2.0/M_PI * ((i&1)*2 - 1.0) / i, srate, freq*i);
		}
	} else if (type == WT_Triangle) {
		for (i = 1;freq*i < srate/2;i+=2) {
			ApplySin(data, 8.0/(M_PI*M_PI) * (1.0 - (i&2)) / (i*i), srate, freq*i);
		}
	} else if (type == WT_Impulse) {
		/* NOTE: Impulse isn't really a waveform, but it can still be useful to
		 * test (other than resampling, the ALSOFT_DEFAULT_REVERB environment
		 * variable can prove useful here to test the reverb response).
		 */
		for (i = 0;i < srate;i++) {
			data[i] = (i%(srate/freq)) ? 0.0f : 1.0f;
		}
	}

	/* Buffer the audio data into a new buffer object. */
	buffer = 0;
	alGenBuffers(1, &buffer);
	alBufferData(buffer, AL_FORMAT_MONO_FLOAT32, data, data_size, srate);
	free(data);

	/* Check if an error occured, and clean up if so. */
	err = alGetError();
	if (err != AL_NO_ERROR) {
		dmLogError("OpenAL Error: %s\n", alGetString(err));
		if (alIsBuffer(buffer)) {
			alDeleteBuffers(1, &buffer);
		}
		return 0;
	}

	return buffer;
}

OpenAL* OpenAL::instance = NULL;

OpenAL::OpenAL() :
	is_initializable(SHOULD_INITIALIZE_OPENAL),
	is_initialized(false),
	is_suspended(false) {
	sources.reserve(128);
	suspended_sources.reserve(128);
}

OpenAL* OpenAL::getInstance() {
	if (!instance) {
		instance = new OpenAL();
	}
	return instance;
}

bool OpenAL::init() {
	if (!is_initialized && is_initializable) {
		device = alcOpenDevice(NULL);
		if (!device) {
			dmLogError("Failed to open audio device.\n");
			return 1;
		}
		context = alcCreateContext(device, NULL);
		alcMakeContextCurrent(context);
		if (!context) {
			dmLogError("Failed to make audio context.\n");
			return false;
		}
		is_initialized = true;
	}
	return true;
}

void OpenAL::setDistanceModel(const char* model) {
	switch (hash_string(model)) {
		case HASH_inverse:
			alDistanceModel(AL_INVERSE_DISTANCE);
			break;
		case HASH_inverse_clamped:
			alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);
			break;
		case HASH_linear:
			alDistanceModel(AL_LINEAR_DISTANCE);
			break;
		case HASH_linear_clamped:
			alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED);
			break;
		case HASH_exponent:
			alDistanceModel(AL_EXPONENT_DISTANCE);
			break;
		case HASH_exponent_clamped:
			alDistanceModel(AL_EXPONENT_DISTANCE_CLAMPED);
			break;
		case HASH_none:
			alDistanceModel(AL_NONE);
			break;
	}
}

float OpenAL::getDopplerFactor() {
	return alGetFloat(AL_DOPPLER_FACTOR);
}

void OpenAL::setDopplerFactor(float dopplerFactor) {
	alDopplerFactor(dopplerFactor);
}

float OpenAL::getSpeedOfSound() {
	return alGetFloat(AL_SPEED_OF_SOUND);
}

void OpenAL::setSpeedOfSound(float speed) {
	alSpeedOfSound(speed);
}

void OpenAL::setListenerPosition(float x, float y, float z) {
	alListener3f(AL_POSITION, x, y, z);
}

void OpenAL::setListenerVelocity(float x, float y, float z) {
	alListener3f(AL_VELOCITY, x, y, z);
}

void OpenAL::setListenerOrientation(float atx, float aty, float atz, float upx, float upy, float upz) {
	ALfloat orientation[] = {atx, aty, atz, upx, upy, upz};
	alListenerfv(AL_ORIENTATION, orientation);
}

ALuint OpenAL::newSource(dmBuffer::HBuffer* sourceBuffer) {
	// Load the sound into a buffer.
	ALuint buffer = loadWavFile(sourceBuffer);
	if (buffer == 0) {
		return 0;
	}

	// Create the source to play the sound with.
	ALuint source = 0;
	alGenSources(1, &source);
	alSourcei(source, AL_SOURCE_RELATIVE, AL_FALSE); // Positioned audio by default.
	alSourcei(source, AL_BUFFER, buffer);
	sources.push_back(source);
	if (alGetError() == AL_NO_ERROR) {
		alSource3f(source, AL_POSITION, 0., 0., 0.);
		alSource3f(source, AL_VELOCITY, 0., 0., 0.);
		alSourcef(source, AL_REFERENCE_DISTANCE, 50.);
		alSourcef(source, AL_MAX_DISTANCE, 1000.);
	}
	return source;
}

void OpenAL::getSourceDefaults(ALuint source, float* gain, float* max_distance, float* rolloff_factor, float* reference_distance, float* min_gain, float* max_gain, float* cone_outer_gain, float* cone_inner_angle, float* cone_outer_angle, float* dx, float* dy, float* dz) {
	alGetSourcef(source, AL_GAIN, gain);
	alGetSourcef(source, AL_MAX_DISTANCE, max_distance);
	alGetSourcef(source, AL_ROLLOFF_FACTOR, rolloff_factor);
	alGetSourcef(source, AL_REFERENCE_DISTANCE, reference_distance);
	#ifndef __EMSCRIPTEN__
		alGetSourcef(source, AL_MIN_GAIN, min_gain);
		alGetSourcef(source, AL_MAX_GAIN, max_gain);
	#else
		*min_gain = 0.f;
		*max_gain = 1.f;
	#endif
	alGetSourcef(source, AL_CONE_OUTER_GAIN, cone_outer_gain);
	alGetSourcef(source, AL_CONE_INNER_ANGLE, cone_inner_angle);
	alGetSourcef(source, AL_CONE_OUTER_ANGLE, cone_outer_angle);
	alGetSource3f(source, AL_DIRECTION, dx, dy, dz);
}

const char* OpenAL::getSourceState(ALuint source) {
	ALenum state = 0;
	alGetSourcei(source, AL_SOURCE_STATE, &state);
	switch (state) {
		case AL_INITIAL:
			return "initial";
		case AL_PLAYING:
			return "playing";
		case AL_PAUSED:
			return "paused";
		case AL_STOPPED:
			return "stopped";
	}
	return "";
}

void OpenAL::setSourcePosition(ALuint source, float x, float y, float z) {
	alSource3f(source, AL_POSITION, x, y, z);
}

void OpenAL::setSourceVelocity(ALuint source, float x, float y, float z) {
	alSource3f(source, AL_VELOCITY, x, y, z);
}

void OpenAL::setSourceDirection(ALuint source, float x, float y, float z) {
	alSource3f(source, AL_DIRECTION, x, y, z);
}

void OpenAL::setSourcePitch(ALuint source, float pitch) {
	alSourcef(source, AL_PITCH, pitch);
}

void OpenAL::setSourceGain(ALuint source, float gain) {
	alSourcef(source, AL_GAIN, gain);
}

void OpenAL::setSourceMaxDistance(ALuint source, float max_distance) {
	alSourcef(source, AL_MAX_DISTANCE, max_distance);
}

void OpenAL::setSourceRolloffFactor(ALuint source, float rolloff_factor) {
	alSourcef(source, AL_ROLLOFF_FACTOR, rolloff_factor);
}

void OpenAL::setSourceReferenceDistance(ALuint source, float reference_distance) {
	alSourcef(source, AL_REFERENCE_DISTANCE, reference_distance);
}

void OpenAL::setSourceMinGain(ALuint source, float min_gain) {
	alSourcef(source, AL_MIN_GAIN, min_gain);
}

void OpenAL::setSourceMaxGain(ALuint source, float max_gain) {
	alSourcef(source, AL_MAX_GAIN, max_gain);
}

void OpenAL::setSourceConeOuterGain(ALuint source, float cone_outer_gain) {
	alSourcef(source, AL_CONE_OUTER_GAIN, cone_outer_gain);
}

void OpenAL::setSourceConeInnerAngle(ALuint source, float cone_inner_angle) {
	alSourcef(source, AL_CONE_INNER_ANGLE, cone_inner_angle);
}

void OpenAL::setSourceConeOuterAngle(ALuint source, float cone_outer_angle) {
	alSourcef(source, AL_CONE_OUTER_ANGLE, cone_outer_angle);
}

void OpenAL::setSourceLooping(ALuint source, bool looping) {
	alSourcei(source, AL_LOOPING, looping ? AL_TRUE : AL_FALSE);
}

void OpenAL::setSourceRelative(ALuint source, bool relative) {
	alSourcei(source, AL_SOURCE_RELATIVE, relative ? AL_TRUE : AL_FALSE);
}

float OpenAL::getSourceTime(ALuint source) {
	float offset;
	alGetSourcef(source, AL_SEC_OFFSET, &offset);
	return offset;
}

void OpenAL::setSourceTime(ALuint source, float seconds) {
	alSourcef(source, AL_SEC_OFFSET, seconds);
}

void OpenAL::playSource(ALuint source) {
	alSourcePlay(source);
}

void OpenAL::pauseSource(ALuint source) {
	alSourcePause(source);
}

void OpenAL::rewindSource(ALuint source) {
	alSourceRewind(source);
}

void OpenAL::stopSource(ALuint source) {
	alSourceStop(source);
}

void OpenAL::removeSource(ALuint source) {
	ALint buffer;
	alGetSourcei(source, AL_BUFFER, &buffer);
	alDeleteSources(1, &source);
	ALuint buffer_u = (ALuint)buffer;
	alDeleteBuffers(1, &buffer_u);

	std::vector<ALuint>::iterator it = std::find(sources.begin(), sources.end(), source);
	if (it != sources.end()) {
		std::swap(*it, sources.back());
		sources.pop_back();
	}
}

void OpenAL::suspend() {
	if (!is_suspended && is_initializable) {
		for (std::vector<ALuint>::iterator i = sources.begin(); i != sources.end(); ++i) {
			ALenum state = 0;
			alGetSourcei(*i, AL_SOURCE_STATE, &state);
			if (state == AL_PLAYING) {
				alSourcePause(*i);
				suspended_sources.push_back(*i);
			}
		}
		is_suspended = true;
	}
}

void OpenAL::resume() {
	if (is_suspended) {
		for (std::vector<ALuint>::iterator i = suspended_sources.begin(); i != suspended_sources.end(); ++i) {
			alSourcePlay(*i);
		}
		suspended_sources.clear();
		is_suspended = false;
	}
}

void OpenAL::close() {
	for (std::vector<ALuint>::iterator i = sources.begin(); i != sources.end(); ++i) {
		ALint buffer;
		alGetSourcei(*i, AL_BUFFER, &buffer);
		alDeleteSources(1, &*i);
		ALuint buffer_u = (ALuint)buffer;
		alDeleteBuffers(1, &buffer_u);
	}
	sources.clear();
	if (is_initialized) {
		alcMakeContextCurrent(NULL);
		alcDestroyContext(context);
		alcCloseDevice(device);
	}
}
