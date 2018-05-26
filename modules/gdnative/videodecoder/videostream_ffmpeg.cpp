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

#include "videostream_ffmpeg.h"

int ffmpeg_read_packet(void *ptr, uint8_t *buf, int buf_size) {
	// ptr is a FileAccess
	FileAccess *file = reinterpret_cast<FileAccess *>(ptr);

	// if file exists
	if (file) {
		long bytes_read = file->get_buffer(buf, buf_size);
		// No bytes to read => EOF
		if (bytes_read == 0) {
			return AVERROR_EOF;
		}
		return bytes_read;
	}
	return -1;
}

int64_t ffmpeg_seek_packet(void *ptr, int64_t pos, int whence) {
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
			case AVSEEK_SIZE: {
				// Asks to return the length of file
				return static_cast<int64_t>(len);
			} break;
		}
	}
	// In case nothing works out.
	return -1;
}

bool VideoStreamPlaybackFFMPEG::open_file(const String &p_file) {

	// TODO Remove all the unnecessary comments.
	// NOTE Only meant to open file once.

	// If playing just err out and don't do anything
	if (playing) {
		return false;
	}

	// set the file name
	file_name = p_file;

	// delete file if exists
	if (file) {
		memdelete(file);
	}
	// open file only for reading
	file = FileAccess::open(p_file, FileAccess::READ);
	// error out if nothing opens
	if (!file) {
		return false;
	}

	// FFMPEG IO

	// TODO: Buffer length? 3KB
	const int buffer_size = 3 * 1024;
	uint8_t *buffer = (uint8_t *)memalloc(buffer_size);
	pIOCtx = avio_alloc_context(buffer, buffer_size, 0, file, ffmpeg_read_packet, nullptr, ffmpeg_seek_packet);

	// FFMPEG format recognition
	pFormatCtx = avformat_alloc_context();
	pFormatCtx->pb = pIOCtx;

	// Recognize the input format (or FFMPEG will kill the buffer by loading 5 MB : Should I try 5MB buffer?)
	int read_bytes = file->get_buffer(buffer, buffer_size);
	if (read_bytes < buffer_size) {
		return false;
	}

	file->seek(0);
	AVProbeData probe_data;
	probe_data.buf = buffer;
	probe_data.buf_size = read_bytes;
	probe_data.filename = "";
	pFormatCtx->iformat = av_probe_input_format(&probe_data, 1);

	pFormatCtx->flags = AVFMT_FLAG_CUSTOM_IO;

	if (avformat_open_input(&pFormatCtx, "", 0, 0) != 0) {
		return false;
	}

	return true;
}

VideoStreamPlaybackFFMPEG::~VideoStreamPlaybackFFMPEG() {
	avformat_close_input(&pFormatCtx);
	av_free(pIOCtx);
	if (file) {
		memdelete(file);
	}
}
