# OpenAL Extension
Provides 3D audio, pitch control and other stuff.

## Quick example
```lua
local res = resource.load("/sounds/sound.wav")
local pos = go.get_position()
self.soundSource = openal.new_source(res)
self.soundSource.x, self.soundSource.y = pos.x, pos.y
self.soundSource.is_looping = true
self.soundSource:play()
```
Video demonstration: https://www.youtube.com/watch?v=5vgHMGr8cpw
## Setup

Copy "openal" folder to your project. Add WAV files as resources (specify resources directory in `game.project`).

## Properties
### `openal.doppler_factor`

Float, positive. Adjusts the Doppler effect. Higher value makes the effect more noticeable.

### `openal.speed_of_sound`

Float, positive. Get or set the speed of sound.

### `openal.listener`

Table. The `Listener` object. Described below.

## Functions
### `openal.get_info()`

Returns a table with information on OpenAL implementation.

Fields:
- `vendor`
- `version`
- `renderer`
- `extensions`

### `openal.set_distance_model(distance_model)`

`distance_model` - string.

Sets a distance model by which all sound attenuation is calculated.

Available models:
- `"inverse"`
- `"inverse_clamped"`
- `"linear"`
- `"linear_clamped"`
- `"exponent"`
- `"exponent_clamped"`
- `"none"`

### `openal.new_source(resource)`

`resource` - resource that represents a WAV file.

Returns a `Source` object.

Creates a sound source object and loads a WAV file into it. The `Source` object is described below.
Only mono WAV files will be positioned. Stereo files will be played as if they are at the listener's position.

### `openal.remove_source(source)`

`source` - a `Source` object.

Destroys the sound source and frees the memory.

# Objects
# `Listener`

Represents the listener - a point to which all the sounds come to.

## Properties

### `listener.x`

Float. X position.

### `listener.y`

Float. Y position.

### `listener.z`

Float. Z position.

### `listener.vx`

Float. X component of the listener's velocity.

### `listener.vy`

Float. Y component of the listener's velocity.

### `listener.vz`

Float. Z component of the listener's velocity.

### `listener.orientation_at_x`

Float. X component of the listener's "at" orientation.

### `listener.orientation_at_y`

Float. Y component of the listener's "at" orientation.

### `listener.orientation_at_x`

Float. Z component of the listener's "at" orientation.

### `listener.orientation_up_x`

Float. X component of the listener's "up" orientation.

### `listener.orientation_up_y`

Float. Y component of the listener's "up" orientation.

### `listener.orientation_up_z`

Float. Z component of the listener's "up" orientation.

# `Source`

Represents a sound source.

## Properties

### `source.x`

Float. X position.

### `source.y`

Float. Y position.

### `source.z`

Float. Z position.

### `source.vx`

Float. X component of the source's velocity.

### `source.vy`

Float. Y component of the source's velocity.

### `source.vz`

Float. Z component of the source's velocity.

### `source.dx`

Float. X component of the source's direction.

### `source.dy`

Float. Y component of the source's direction.

### `source.dz`

Float. Z component of the source's direction.

### `source.pitch`

Float. Pitch adjustment, default is `1.0`.

### `source.gain`

Float. Gain adjustment, a value from `0` to `1.0`, default is `1.0`.

### `source.max_distance`

Float. Used with the Inverse Clamped Distance Model to set the distance where there will no longer be any attenuation of the source.

### `source.rolloff_factor`

Float. The rolloff rate for the source, default is `1.0`.

### `source.reference_distance`

Float. The distance under which the volume for the source would normally drop by half (before being influenced by `source.rolloff_factor` or `source.max_distance`), default is `50.0`.

### `source.min_gain`

Float. The minimum gain for this source.

### `source.max_gain`

Float. The maximum gain for this source.

### `source.cone_outer_gain`

Float. The gain when outside the oriented cone.

### `source.cone_inner_angle`

Float. The gain when inside the oriented cone.

### `source.cone_outer_angle`

Float. Outer angle of the sound cone, in degrees, default is `360`.

### `source.is_looping`

Boolean. If `true`, the sound will be repeated indefinitely, default is `false`.

### `source.is_relative`

Boolean. If `true`, the sound will be positioned relative to the listener, default is `false`.

### `source.time`

Float. Get or set the sound playback progress in seconds.

### `source.state`

String. Get this source's state. Possible values: `"initial"`, `"playing"`, `"paused"`, `"stopped"`.

## Functions

### `source:play()`

Starts the playback of the sound from this source.

### `source:pause()`

Pauses the playback.

### `source:rewind()`

Resets the playback to the start of the sound.

### `source:stop()`

Stops the playback.
