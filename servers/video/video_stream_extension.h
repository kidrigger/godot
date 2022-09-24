/*************************************************************************/
/*  video_stream_extension.h                                             */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#ifndef VIDEO_STREAM_EXTENSION_H
#define VIDEO_STREAM_EXTENSION_H

#include "core/io/file_access.h"
#include "scene/resources/texture.h"

class VideoStreamPlayback : public Resource {
	GDCLASS(VideoStreamPlayback, Resource);

public:
	typedef int (*AudioMixCallback)(void *p_udata, const float *p_data, int p_frames);

protected:
	String file_name;
	Ref<FileAccess> file = nullptr;

	AudioMixCallback mix_callback = nullptr;
	void *mix_udata = nullptr;

	static void _bind_methods();
	GDVIRTUAL0(_stop);
	GDVIRTUAL0(_play);
	GDVIRTUAL0RC(bool, _is_playing);
	GDVIRTUAL1(_set_paused, bool);
	GDVIRTUAL0RC(bool, _is_paused);
	GDVIRTUAL1(_set_loop, bool);
	GDVIRTUAL0RC(bool, _has_loop);
	GDVIRTUAL0RC(float, _get_length);
	GDVIRTUAL0RC(float, _get_playback_position);
	GDVIRTUAL1(_seek, float);
	GDVIRTUAL1(_set_audio_track, int);
	GDVIRTUAL0RC(Ref<Texture2D>, _get_texture);
	GDVIRTUAL1(_update, float);
	GDVIRTUAL0RC(int, _get_channels);
	GDVIRTUAL0RC(int, _get_mix_rate);

	GDVIRTUAL0R(bool, _file_opened);

	GDVIRTUAL0(_initialize);
	GDVIRTUAL0(_cleanup);

	PackedByteArray file_read(uint64_t length);
	void file_seek(int pos);
	uint64_t file_pos();
	uint64_t file_len();
	int mix_audio(int num_frames, PackedFloat32Array buffer = {}, int offset = 0);

public:
	VideoStreamPlayback();
	virtual ~VideoStreamPlayback();

	bool open_file(const String &p_file);

	virtual void stop();
	virtual void play();

	virtual bool is_playing() const;

	virtual void set_paused(bool p_paused);
	virtual bool is_paused() const;

	virtual void set_loop(bool p_enable);
	virtual bool has_loop() const;

	virtual float get_length() const;

	virtual float get_playback_position() const;
	virtual void seek(float p_time);

	virtual void set_audio_track(int p_idx);

	virtual Ref<Texture2D> get_texture() const;
	virtual void update(float p_delta);

	virtual void set_mix_callback(AudioMixCallback p_callback, void *p_userdata);
	virtual int get_channels() const;
	virtual int get_mix_rate() const;
};

class VideoStream : public Resource {
	GDCLASS(VideoStream, Resource);
	OBJ_SAVE_TYPE(VideoStream);

protected:
	static void
	_bind_methods();

	GDVIRTUAL0R(Ref<VideoStreamPlayback>, _instantiate_playback);
	GDVIRTUAL0RC(PackedStringArray, _get_supported_extensions);
	GDVIRTUAL0RC(String, _get_plugin_name);
	GDVIRTUAL0(_initialize);
	GDVIRTUAL0(_cleanup);

	String file;
	int audio_track = 0;

public:
	void set_file(const String &p_file);
	String get_file();

	virtual void set_audio_track(int p_track);
	virtual Ref<VideoStreamPlayback> instantiate_playback();
	virtual String get_plugin_name();

	virtual PackedStringArray get_supported_extensions() const;

	VideoStream();
	~VideoStream();
};

#endif // VIDEO_STREAM_EXTENSION_H
