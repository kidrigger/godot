/*************************************************************************/
/*  video_stream_webm.cpp                                                */
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

#include "video_stream_gdnative.h"

void VideoStreamGDNative::stop() {
    instance->stop();
}
void VideoStreamGDNative::play() {
    instance->play();
}

bool VideoStreamGDNative::is_playing() {
    return instance->is_playing();
}

void VideoStreamGDNative::set_paused(bool p_paused) {
    instance->set_paused(p_paused);
}
bool VideoStreamGDNative::is_paused() {
    return instance->is_paused();
}

void VideoStreamGDNative::set_loop(bool p_enable) {
    instance->set_loop(p_enable);
}
bool VideoStreamGDNative::has_loop() {
    return instance->has_loop();
}

float VideoStreamGDNative::get_length() {
    return instance->get_length;
}

float VideoStreamGDNative::get_playback_position() {
    return instance->get_playback_position;
}
void VideoStreamGDNative::seek(float p_time) {
    instance->seek(p_time);
}

void VideoStreamGDNative::set_audio_track(int p_idx) {
    instance->set_audio_track(p_idx);
}

Ref<Texture> VideoStreamGDNative::get_texture() {
    instance->get_texture();
}
void VideoStreamGDNative::update(float p_delta);

void VideoStreamGDNative::set_mix_callback(AudioMixCallback p_callback, void *p_userdata) = 0;
int VideoStreamGDNative::get_channels();
int VideoStreamGDNative::get_mix_rate();
