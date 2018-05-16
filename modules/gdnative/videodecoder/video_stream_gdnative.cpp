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

VideoStreamPlaybackGDNative::VideoStreamPlaybackGDNative() {

    print_line("Contructing VSGDN");
}

VideoStreamPlaybackGDNative::~VideoStreamPlaybackGDNative() {

    print_line("Destroying VSGDN");

}

void VideoStreamPlaybackGDNative::cleanup() {
    print_line("cleanup");
}

void VideoStreamPlaybackGDNative::set_interface(const godot_video_stream_playback_interface_gdnative* p_interface) {
    print_line("set interface");
}

bool VideoStreamPlaybackGDNative::is_initialized() {
	print_line("check interface");
    return false;
}

void VideoStreamPlaybackGDNative::stop() {
    print_line("Stop");
    playing = false;
}

void VideoStreamPlaybackGDNative::play() {
    print_line("Play");
    playing = true;
}

bool VideoStreamPlaybackGDNative::is_playing() {
    print_line("is_playing");
    return playing;
}

void VideoStreamPlaybackGDNative::set_paused(bool p_paused) {
    print_line("is_playing");
    paused = p_paused;
}
bool VideoStreamPlaybackGDNative::is_paused() {
    print_line("is_paused");
    return paused;
}

void VideoStreamPlaybackGDNative::set_loop(bool p_enable) {
    print_line("set_loop");
    looping = p_enable;
}
bool VideoStreamPlaybackGDNative::has_loop() {
    print_line("has_loop");
    return looping;
}

float VideoStreamPlaybackGDNative::get_length() {
    print_line("get_length");
    return 0;
}

float VideoStreamPlaybackGDNative::get_playback_position() {
    print_line("get_playback_position");
    return 0;
}
void VideoStreamPlaybackGDNative::seek(float p_time) {
    print_line("seek");
}

void VideoStreamPlaybackGDNative::set_audio_track(int p_idx) {
    print_line("Set audio track");
}

Ref<Texture> VideoStreamPlaybackGDNative::get_texture() {
    return Ref<Texture>();
}

void VideoStreamPlaybackGDNative::update(float p_delta) {
    print_line("Updated");
}

void VideoStreamPlaybackGDNative::set_mix_callback(AudioMixCallback p_callback, void *p_userdata) {
    print_line("set_mix_callback");
}

int VideoStreamPlaybackGDNative::get_channels() {
    print_line("get_channels");
    return 0;
}

int VideoStreamPlaybackGDNative::get_mix_rate() {
    print_line("Get mix rate");
    return 5;
}
