/*************************************************************************/
/*  videostream_ffmpeg.cpp                                               */
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

#include "videostream_ffmpeg.h"

static const godot_videodecoder_interface_gdnative *stat_interface = nullptr;

// TODO: Put at end.
extern "C" {
godot_int GDAPI godot_videodecoder_read_packet(void *ptr, uint8_t *buf, int buf_size) {
	// ptr is a FileAccess
	FileAccess *file = reinterpret_cast<FileAccess *>(ptr);

	// if file exists
	if (file) {
		long bytes_read = file->get_buffer(buf, buf_size);
		// No bytes to read => EOF
		if (bytes_read == 0) {
			return 0;
		}
		return bytes_read;
	}
	return -1;
}

int64_t GDAPI godot_videodecoder_seek_packet(void *ptr, int64_t pos, int whence) {
	// file
	FileAccess *file = reinterpret_cast<FileAccess *>(ptr);

	size_t len = file->get_len();
	if (file) {
		switch (whence) {
			case SEEK_SET: {
				// Just for explicitness
				size_t new_pos = static_cast<size_t>(pos);
				if (new_pos > len) {
					return -1;
				}
				file->seek(new_pos);
				pos = static_cast<int64_t>(file->get_position());
				return pos;
			} break;
			case SEEK_END: {
				// Just in case something goes wrong
				if (-pos > len) {
					return -1;
				}
				file->seek_end(pos);
				pos = static_cast<int64_t>(file->get_position());
				return pos;
			} break;
			case SEEK_CUR: {
				// Just in case it doesn't exist
				if (pos < 0 && -pos > file->get_position()) {
					return -1;
				}
				pos = pos + static_cast<int>(file->get_position());
				file->seek(pos);
				pos = static_cast<int64_t>(file->get_position());
				return pos;
			} break;
			default: {
				// Only 4 possible options, hence default = AVSEEK_SIZE
				// Asks to return the length of file
				return static_cast<int64_t>(len);
			} break;
		}
	}
	// In case nothing works out.
	return -1;
}

void GDAPI godot_videodecoder_register_decoder(const godot_videodecoder_interface_gdnative *p_interface) {
	print_line("Interface registered");
	print_line(p_interface->get_plugin_name());
	stat_interface = p_interface;
}
}

bool VideoStreamPlaybackFFMPEG::open_file(const String &p_file) {
	ERR_FAIL_COND_V(interface == nullptr, false);
	file = FileAccess::open(p_file, FileAccess::READ);
	return interface->open_file(data_struct, file);
}

void VideoStreamPlaybackFFMPEG::update(float p_delta) {
	if (!playing || paused) {
		return;
	}
	ERR_FAIL_COND(interface == nullptr);
	interface->update(data_struct, p_delta);
}

// ctor and dtor

VideoStreamPlaybackFFMPEG::VideoStreamPlaybackFFMPEG() :
		texture(Ref<ImageTexture>(memnew(ImageTexture))) {
	;
}

VideoStreamPlaybackFFMPEG::~VideoStreamPlaybackFFMPEG() {
	// cleanup();
}

void VideoStreamPlaybackFFMPEG::set_interface(const godot_videodecoder_interface_gdnative *p_interface) {
	interface = p_interface;
}

// controls

bool VideoStreamPlaybackFFMPEG::is_playing() const {
	return playing;
}

bool VideoStreamPlaybackFFMPEG::is_paused() const {
	return paused;
}

void VideoStreamPlaybackFFMPEG::play() {

	stop();

	playing = true;
}

void VideoStreamPlaybackFFMPEG::stop() {
	if (playing) {
		seek(0);
	}
	playing = false;
}

void VideoStreamPlaybackFFMPEG::seek(float p_time) {
	ERR_FAIL_COND(interface == nullptr);
	interface->seek(data_struct, p_time);
}

void VideoStreamPlaybackFFMPEG::set_paused(bool p_paused) {
	paused = p_paused;
}

Ref<Texture> VideoStreamPlaybackFFMPEG::get_texture() {
	return texture;
}

float VideoStreamPlaybackFFMPEG::get_length() const {
	ERR_FAIL_COND_V(interface == nullptr, 0);
	return interface->get_length(data_struct);
}

float VideoStreamPlaybackFFMPEG::get_playback_position() const {

	ERR_FAIL_COND_V(interface == nullptr, 0);
	return interface->get_playback_position(data_struct);
}

bool VideoStreamPlaybackFFMPEG::has_loop() const {
	// TODO: Implement looping?
	return false;
}

void VideoStreamPlaybackFFMPEG::set_loop(bool p_enable) {
	// Do nothing
}

void VideoStreamPlaybackFFMPEG::set_audio_track(int p_idx) {
	ERR_FAIL_COND(interface == nullptr);
	interface->set_audio_track(data_struct, p_idx);
}

void VideoStreamPlaybackFFMPEG::set_mix_callback(AudioMixCallback p_callback, void *p_userdata) {
	// TODO
}

int VideoStreamPlaybackFFMPEG::get_channels() const {
	ERR_FAIL_COND_V(interface == nullptr, 0);

	return interface->get_channels(data_struct);
}

int VideoStreamPlaybackFFMPEG::get_mix_rate() const {
	ERR_FAIL_COND_V(interface == nullptr, 0);

	return interface->get_mix_rate(data_struct);
}

/* --- NOTE VideoStreamFFMPEG starts here. ----- */
// FIX: remove region
#pragma region // VideoStreamFFMPEG

Ref<VideoStreamPlayback> VideoStreamFFMPEG::instance_playback() {
	Ref<VideoStreamPlaybackFFMPEG> pb = memnew(VideoStreamPlaybackFFMPEG);
	pb->set_interface(stat_interface);
	pb->set_audio_track(audio_track);
	if (pb->open_file(file))
		return pb;
	return NULL;
}

void VideoStreamFFMPEG::set_file(const String &p_file) {

	file = p_file;
}

String VideoStreamFFMPEG::get_file() {

	return file;
}

void VideoStreamFFMPEG::_bind_methods() {

	ClassDB::bind_method(D_METHOD("set_file", "file"), &VideoStreamFFMPEG::set_file);
	ClassDB::bind_method(D_METHOD("get_file"), &VideoStreamFFMPEG::get_file);

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "file", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NOEDITOR | PROPERTY_USAGE_INTERNAL), "set_file", "get_file");
}

void VideoStreamFFMPEG::set_audio_track(int p_track) {

	audio_track = p_track;
}

// FIX remove
#pragma endregion
