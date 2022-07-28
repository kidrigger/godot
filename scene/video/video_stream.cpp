/*************************************************************************/
/*  video_stream.cpp                                                     */
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

#include "video_stream.h"

#include "core/config/project_settings.h"
#include "servers/audio_server.h"

// VideoStreamPlayback starts here.

void VideoStreamPlayback::_bind_methods() {
	//ClassDB::bind_method(D_METHOD("open_file"), &VideoStreamPlayback::open_file);
	ClassDB::bind_method(D_METHOD("file_read", "length"), &VideoStreamPlayback::file_read);
	ClassDB::bind_method(D_METHOD("file_seek", "pos"), &VideoStreamPlayback::file_seek);
	ClassDB::bind_method(D_METHOD("file_pos"), &VideoStreamPlayback::file_pos);
	ClassDB::bind_method(D_METHOD("file_len"), &VideoStreamPlayback::file_len);
	ClassDB::bind_method(D_METHOD("mix_audio", "num_frames", "buffer", "offset"), &VideoStreamPlayback::mix_audio, DEFVAL(PackedFloat32Array()), DEFVAL(0));
	GDVIRTUAL_BIND(_stop);
	GDVIRTUAL_BIND(_play);
	GDVIRTUAL_BIND(_is_playing);
	GDVIRTUAL_BIND(_set_paused, "paused");
	GDVIRTUAL_BIND(_is_paused);
	GDVIRTUAL_BIND(_set_loop, "enable");
	GDVIRTUAL_BIND(_has_loop);
	GDVIRTUAL_BIND(_get_length);
	GDVIRTUAL_BIND(_get_playback_position);
	GDVIRTUAL_BIND(_seek, "time");
	GDVIRTUAL_BIND(_set_audio_track, "idx");
	GDVIRTUAL_BIND(_get_texture);
	GDVIRTUAL_BIND(_update, "delta");
	GDVIRTUAL_BIND(_get_channels);
	GDVIRTUAL_BIND(_get_mix_rate);
	GDVIRTUAL_BIND(_file_opened);
	GDVIRTUAL_BIND(_initialize);
	GDVIRTUAL_BIND(_cleanup);
}

VideoStreamPlayback::VideoStreamPlayback() {
	GDVIRTUAL_CALL(_initialize);
}

VideoStreamPlayback::~VideoStreamPlayback() {
	GDVIRTUAL_CALL(_cleanup);
}

void VideoStreamPlayback::stop() {
	GDVIRTUAL_CALL(_stop);
}

void VideoStreamPlayback::play() {
	GDVIRTUAL_CALL(_play);
}

bool VideoStreamPlayback::is_playing() const {
	bool ret;
	if (GDVIRTUAL_CALL(_is_playing, ret)) {
		return ret;
	}
	return false;
}

void VideoStreamPlayback::set_paused(bool p_paused) {
	GDVIRTUAL_CALL(_is_playing, p_paused);
}

bool VideoStreamPlayback::is_paused() const {
	bool ret;
	if (GDVIRTUAL_CALL(_is_paused, ret)) {
		return ret;
	}
	return false;
}

void VideoStreamPlayback::set_loop(bool p_enable) {
	GDVIRTUAL_CALL(_set_loop, p_enable);
}

bool VideoStreamPlayback::has_loop() const {
	bool ret;
	if (GDVIRTUAL_CALL(_has_loop, ret)) {
		return ret;
	}
	return false;
}

double VideoStreamPlayback::get_length() const {
	double ret;
	if (GDVIRTUAL_CALL(_get_length, ret)) {
		return ret;
	}
	return 0;
}

double VideoStreamPlayback::get_playback_position() const {
	double ret;
	if (GDVIRTUAL_CALL(_get_playback_position, ret)) {
		return ret;
	}
	return 0;
}

void VideoStreamPlayback::seek(double p_time) {
	GDVIRTUAL_CALL(_seek, p_time);
}

void VideoStreamPlayback::set_audio_track(int p_idx) {
	GDVIRTUAL_CALL(_set_audio_track, p_idx);
}

Ref<Texture2D> VideoStreamPlayback::get_texture() const {
	Ref<Texture2D> ret;
	if (GDVIRTUAL_CALL(_get_texture, ret)) {
		return ret;
	}
	return nullptr;
}

void VideoStreamPlayback::update(double p_delta) {
	if (!GDVIRTUAL_CALL(_update, p_delta)) {
		ERR_FAIL_MSG("VideoStreamPlayback::update unimplemented");
	}
}

void VideoStreamPlayback::set_mix_callback(AudioMixCallback p_callback, void *p_userdata) {
	mix_callback = p_callback;
	mix_udata = p_userdata;
}

int VideoStreamPlayback::get_channels() const {
	int ret;
	if (GDVIRTUAL_CALL(_get_channels, ret)) {
		return ret;
	}
	return 0;
}

int VideoStreamPlayback::get_mix_rate() const {
	int ret;
	if (GDVIRTUAL_CALL(_get_mix_rate, ret)) {
		return ret;
	}
	return 0;
}

bool VideoStreamPlayback::open_file(const String &p_file) {
	if (file.is_valid()) {
		file.unref();
	}

	file = FileAccess::open(p_file, FileAccess::READ);

	bool retval;
	if (file.is_valid() && GDVIRTUAL_CALL(_file_opened, retval)) {
		return retval;
	}

	return false;
}

PackedByteArray VideoStreamPlayback::file_read(uint64_t length) {
	ERR_FAIL_COND_V_MSG(file == nullptr, {}, "Reading from file before opening.");
	static PackedByteArray read_buf;
	read_buf.resize(length);
	read_buf.resize(file->get_buffer(read_buf.ptrw(), read_buf.size()));
	return read_buf;
}

void VideoStreamPlayback::file_seek(int pos) {
	ERR_FAIL_COND_MSG(file == nullptr, "Seeking from file before opening.");
	file->seek(pos);
}

uint64_t VideoStreamPlayback::file_pos() {
	ERR_FAIL_COND_V_MSG(file == nullptr, 0, "Checking position in file before opening.");
	return file->get_position();
}

uint64_t VideoStreamPlayback::file_len() {
	ERR_FAIL_COND_V_MSG(file == nullptr, 0, "Checking length of file before opening.");
	return file->get_length();
}

int VideoStreamPlayback::mix_audio(int num_frames, PackedFloat32Array buffer, int offset) {
	if (num_frames <= 0) {
		return 0;
	}
	if (!mix_callback) {
		return -1;
	}
	return mix_callback(mix_udata, buffer.ptr() + offset, num_frames);
}

/* --- NOTE VideoStream starts here. ----- */

Ref<VideoStreamPlayback> VideoStream::instantiate_playback() {
	Object *ret = nullptr;
	if (GDVIRTUAL_CALL(_instantiate_playback, ret)) {
		ERR_FAIL_COND_V_MSG(ret == nullptr, nullptr, "Plugin returned null playback");

		Ref<VideoStreamPlayback> ref = Ref<VideoStreamPlayback>(ret);
		const bool file_opened = ref->open_file(file);
		if (!file_opened) {
			return nullptr;
		}
		ref->set_audio_track(audio_track);
		return ref;
	}
	return nullptr;
}

String VideoStream::get_plugin_name() {
	String ret;
	if (GDVIRTUAL_CALL(_get_plugin_name, ret)) {
		return ret;
	}
	return "godot";
}

PackedStringArray VideoStream::get_supported_extensions() const {
	PackedStringArray pbr;
	if (GDVIRTUAL_CALL(_get_supported_extensions, pbr)) {
		return pbr;
	}
	return {};
}

VideoStream::VideoStream() {
	GDVIRTUAL_CALL(_initialize);
}

VideoStream::~VideoStream() {
	GDVIRTUAL_CALL(_cleanup);
}

void VideoStream::set_file(const String &p_file) {
	file = p_file;
}

String VideoStream::get_file() {
	return file;
}

void VideoStream::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_file", "file"), &VideoStream::set_file);
	ClassDB::bind_method(D_METHOD("get_file"), &VideoStream::get_file);

	ClassDB::bind_method(D_METHOD("set_audio_track", "track"), &VideoStream::set_audio_track);

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "file", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NO_EDITOR | PROPERTY_USAGE_INTERNAL), "set_file", "get_file");

	GDVIRTUAL_BIND(_get_supported_extensions);
	GDVIRTUAL_BIND(_instantiate_playback);
	GDVIRTUAL_BIND(_get_plugin_name);
	GDVIRTUAL_BIND(_initialize);
	GDVIRTUAL_BIND(_cleanup);
}

void VideoStream::set_audio_track(int p_track) {
	audio_track = p_track;
}

/* --- NOTE ResourceFormatLoaderVideoStreamExtension starts here. ----- */

void VideoStreamLoader::_bind_methods() {
	GDVIRTUAL_BIND(_instantiate_video_stream);
}

Ref<Resource> VideoStreamLoader::load(const String &p_path, const String &p_original_path, Error *r_error, bool p_use_sub_threads, float *r_progress, CacheMode p_cache_mode) {
	Ref<FileAccess> f = FileAccess::open(p_path, FileAccess::READ);
	if (f.is_null()) {
		if (r_error) {
			*r_error = ERR_CANT_OPEN;
		}
		return Ref<Resource>();
	}

	Object *stream = nullptr;
	if (!GDVIRTUAL_CALL(_instantiate_video_stream, stream)) {
		if (r_error) {
			*r_error = ERR_CANT_OPEN;
		}
		return Ref<Resource>();
	}

	Ref<VideoStream> ext_stream = Ref<VideoStream>(stream);
	ext_stream->set_file(p_path);

	if (r_error) {
		*r_error = OK;
	}

	return ext_stream;
}

bool VideoStreamLoader::handles_type(const String &p_type) const {
	return ClassDB::is_parent_class(p_type, "VideoStream");
}
