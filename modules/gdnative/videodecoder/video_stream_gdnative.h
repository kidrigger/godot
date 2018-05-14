/*************************************************************************/
/*  video_stream_webm.h                                                  */
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

#ifndef VIDEO_STREAM_GDNATIVE_H
#define VIDEO_STREAM_GDNATIVE_H

#include "scene/resources/video_stream.h"

/*
 * TODO: Relocate the struct
 */
 #ifdef __cplusplus
extern "C" {
#endif
typedef struct {
    void *(*constructor)(godot_object *);
	void (*destructor)(void *);
	void (*stop)(void *);
	void (*play)(void *);
	godot_bool (*is_playing)(const void *);
	godot_bool (*is_paused)(const void *);
	void (*set_paused)(void *, godot_bool);
	void (*set_loop)(void *);
	godot_bool (*has_loop)(const void *);
	godot_real (*get_length)(void *);
	godot_real (*get_playback_position)(void *);
	void (*seek)(void *, godot_real);
	void (*set_audio_track)(void *, godot_int);
    // TODO: Ref<Texture>
    Ref<Texture> (*get_texture)(void *);
	void (*update)(void *, godot_real);
    // TODO: AudioMixCallback
    void (*set_mix_callback)(AudioMixCallback, void*);
    int (*get_channels)(void *);
    int (*get_mix_rate)(void *);
} video_stream_playback_interface;
#ifdef __cplusplus
}
#endif

/* */

class VideoStreamPlaybackGDNative : public VideoStreamPlayback {

	GDCLASS(VideoStreamPlaybackGDNative, VideoStreamPlayback)

protected:
    static void _bind_methods();
    const video_stream_playback_interface* instance;

public:

    virtual void stop();
    virtual void play();

    virtual bool is_playing();

    virtual void set_paused(bool p_paused);
    virtual bool is_paused();

    virtual void set_loop(bool p_enable);
    virtual bool has_loop();

    virtual float get_length();

    virtual float get_playback_position();
    virtual void seek(float p_time);

    virtual void set_audio_track(int p_idx);

    //virtual int mix(int16_t* p_buffer,int p_frames)=0;

    virtual Ref<Texture> get_texture();
    virtual void update(float p_delta);

    virtual void set_mix_callback(AudioMixCallback p_callback, void *p_userdata) = 0;
    virtual int get_channels();
    virtual int get_mix_rate();

    VideoStreamPlaybackGDNative();
};

/**/

class VideoStreamGDNative : public VideoStream {

	GDCLASS(VideoStreamGDNative, VideoStream);
    OBJ_SAVE_TYPE(VideoStreamGDNative); //children are all saved as AudioStream, so they can be exchanged

    public:
    virtual void set_audio_track(int p_track) = 0;
    virtual Ref<VideoStreamPlaybackGDNative> instance_playback() = 0;

    VideoStreamGDNative() {}
};

#endif // VIDEO_STREAM_GDNATIVE_H
