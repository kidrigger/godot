/*************************************************************************/
/*  video_stream.h                                                       */
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

#include "modules/gdnative/videodecoder/ffmpeg/include/libavformat/avformat.h"
#include "modules/gdnative/videodecoder/ffmpeg/include/libswscale/swscale.h"
#include "os/file_access.h"
#include "scene/resources/texture.h"
#include "scene/resources/video_stream.h"

class VideoStreamPlaybackFFMPEG : public VideoStreamPlayback {

	GDCLASS(VideoStreamPlaybackFFMPEG, VideoStreamPlayback);

	FileAccess *file;
	Ref<ImageTexture> texture;

	float time;
	bool playing;
	bool paused;

	AVIOContext *pIOCtx;
	AVFormatContext *pFormatCtx;
	AVCodecContext *pCodecCtx;
	AVFrame *pFrameYUV;
	AVFrame *pFrameRGB;
	SwsContext *sws_ctx;
	AVPacket packet;
	uint8_t *io_buffer;

	PoolVector<uint8_t> frame_buffer;
	int videostream_idx;

	void cleanup();

protected:
	static void _bind_methods();

	String file_name;

public:
	VideoStreamPlaybackFFMPEG(); // CLEANUP
	~VideoStreamPlaybackFFMPEG(); // CLEANUP

	bool open_file(const String &p_file); // CLEANUP

	virtual void stop();
	virtual void play();

	virtual bool is_playing() const;

	virtual void set_paused(bool p_paused);
	virtual bool is_paused() const;

	virtual void set_loop(bool p_enable); // FIX
	virtual bool has_loop() const; // FIX

	virtual float get_length() const; //

	virtual float get_playback_position() const; // TODO
	virtual void seek(float p_time); // TODO

	virtual void set_audio_track(int p_idx); // TODO

	//virtual int mix(int16_t* p_buffer,int p_frames)=0;

	virtual Ref<Texture> get_texture();
	virtual void update(float p_delta); // FIX CLEANUP

	virtual void set_mix_callback(AudioMixCallback p_callback, void *p_userdata) {} // TODO
	virtual int get_channels() const { return 0; } // TODO
	virtual int get_mix_rate() const { return 0; } // TODO
};

class VideoStreamFFMPEG : public VideoStream {

	GDCLASS(VideoStreamFFMPEG, VideoStream);
	RES_BASE_EXTENSION("ffmpegstr");

	String file;
	int audio_track;

protected:
	static void _bind_methods();

public:
	void set_file(const String& p_file);
	String get_file();

	virtual void set_audio_track(int p_track);
	virtual Ref<VideoStreamPlayback> instance_playback();

	VideoStreamFFMPEG() {}
};

#endif
