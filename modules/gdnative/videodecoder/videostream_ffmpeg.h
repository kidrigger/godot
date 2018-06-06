/*************************************************************************/
/*  videostream_ffmpeg.h                                                 */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2018 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2018 Godot Engine contributors (cf. AUTHORS.md)    */
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

#ifndef VIDEO_STREAM_FFMPEG_H
#define VIDEO_STREAM_FFMPEG_H

#include <modules/gdnative/gdnative.h>
#include <os/file_access.h>
#include <scene/resources/texture.h>
#include <scene/resources/video_stream.h>

class VideoStreamPlaybackFFMPEG : public VideoStreamPlayback {

	GDCLASS(VideoStreamPlaybackFFMPEG, VideoStreamPlayback);

	Ref<ImageTexture> texture;
	bool playing;
	bool paused;

protected:
	String file_name;

	FileAccess *file = nullptr;

	const godot_videodecoder_interface_gdnative *interface = nullptr;
	void *data_struct = nullptr;

public:
	VideoStreamPlaybackFFMPEG();
	~VideoStreamPlaybackFFMPEG();

	void set_interface(const godot_videodecoder_interface_gdnative *p_interface);

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

	//virtual int mix(int16_t* p_buffer,int p_frames)=0;

	virtual Ref<Texture> get_texture();
	virtual void update(float p_delta);

	virtual void set_mix_callback(AudioMixCallback p_callback, void *p_userdata);
	virtual int get_channels() const;
	virtual int get_mix_rate() const;
};

class VideoStreamFFMPEG : public VideoStream {

	GDCLASS(VideoStreamFFMPEG, VideoStream);
	RES_BASE_EXTENSION("ffmpegstr");

	String file;
	int audio_track;

protected:
	static void
	_bind_methods();

public:
	void set_file(const String &p_file);
	String get_file();

	virtual void set_audio_track(int p_track);
	virtual Ref<VideoStreamPlayback> instance_playback();

	VideoStreamFFMPEG() {}
};

#endif
