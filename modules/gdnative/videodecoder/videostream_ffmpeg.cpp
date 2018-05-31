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

#include "modules/gdnative/videodecoder/ffmpeg/include/libavutil/avutil.h"
#include "modules/gdnative/videodecoder/ffmpeg/include/libavutil/imgutils.h"

// int ffmpeg_read_packet(void *ptr, uint8_t *buf, int buf_size) {
// 	// ptr is a FileAccess
// 	FileAccess *file = reinterpret_cast<FileAccess *>(ptr);

// 	// if file exists
// 	if (file) {
// 		long bytes_read = file->get_buffer(buf, buf_size);
// 		// No bytes to read => EOF
// 		if (bytes_read == 0) {
// 			return AVERROR_EOF;
// 		}
// 		return bytes_read;
// 	}
// 	return -1;
// }

// int64_t ffmpeg_seek_packet(void *ptr, int64_t pos, int whence) {
// 	// file
// 	FileAccess *file = reinterpret_cast<FileAccess *>(ptr);

// 	size_t len = file->get_len();
// 	if (file) {
// 		switch (whence) {
// 			case SEEK_SET: {
// 				// Just for explicitness
// 				size_t new_pos = static_cast<size_t>(pos);
// 				if (new_pos > len) {
// 					return -1;
// 				}
// 				file->seek(new_pos);
// 				pos = static_cast<int64_t>(file->get_position());
// 				return pos;
// 			} break;
// 			case SEEK_END: {
// 				// Just in case something goes wrong
// 				if (-pos > len) {
// 					return -1;
// 				}
// 				file->seek_end(pos);
// 				pos = static_cast<int64_t>(file->get_position());
// 				return pos;
// 			} break;
// 			case SEEK_CUR: {
// 				// Just in case it doesn't exist
// 				if (pos < 0 && -pos > file->get_position()) {
// 					return -1;
// 				}
// 				pos = pos + static_cast<int>(file->get_position());
// 				file->seek(pos);
// 				pos = static_cast<int64_t>(file->get_position());
// 				return pos;
// 			} break;
// 			case AVSEEK_SIZE: {
// 				// Asks to return the length of file
// 				return static_cast<int64_t>(len);
// 			} break;
// 		}
// 	}
// 	// In case nothing works out.
// 	return -1;
// }

bool VideoStreamPlaybackFFMPEG::open_file(const String &p_file) {

	// // TODO: Remove all the unnecessary comments.
	// // NOTE: Only meant to open file once.

	// // If playing just err out and don't do anything
	// if (playing) {
	// 	return false;
	// }

	// // set the file name
	// file_name = p_file;

	// // cleanup before starting.
	// cleanup();

	// // open file only for reading
	// file = FileAccess::open(p_file, FileAccess::READ);
	// // error out if nothing opens
	// if (!file) {
	// 	return false;
	// }

	// // FFMPEG IO

	// // TODO: Buffer length? 3KB
	// const int buffer_size = 3 * 1024;
	// io_buffer = (uint8_t *)memalloc(buffer_size);

	// pIOCtx = avio_alloc_context(io_buffer, buffer_size, 0, file, ffmpeg_read_packet, nullptr, ffmpeg_seek_packet);

	// // FFMPEG format recognition
	// pFormatCtx = avformat_alloc_context();
	// pFormatCtx->pb = pIOCtx;

	// // Recognize the input format (or FFMPEG will kill the io_buffer by loading 5 MB : Should I try 5MB io_buffer?)
	// int read_bytes = file->get_buffer(io_buffer, buffer_size);
	// if (read_bytes < buffer_size) {
	// 	cleanup();
	// 	return false;
	// }

	// file->seek(0);
	// AVProbeData probe_data;
	// probe_data.buf = io_buffer;
	// probe_data.buf_size = read_bytes;
	// probe_data.filename = "";
	// pFormatCtx->iformat = av_probe_input_format(&probe_data, 1);

	// pFormatCtx->flags = AVFMT_FLAG_CUSTOM_IO;

	// if (avformat_open_input(&pFormatCtx, "", 0, 0) != 0) {
	// 	cleanup();
	// 	return false;
	// }

	// if (avformat_find_stream_info(pFormatCtx, nullptr) < 0) {
	// 	cleanup();
	// 	return false;
	// }

	// // Find the video stream
	// for (int i = 0; i != pFormatCtx->nb_streams; i++) {
	// 	if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
	// 		videostream_idx = i;
	// 	}
	// }
	// if (videostream_idx == -1) {
	// 	cleanup();
	// 	return false;
	// }

	// AVCodecParameters *pCodecParam = pFormatCtx->streams[videostream_idx]->codecpar;

	// AVCodec *pCodec = avcodec_find_decoder(pCodecParam->codec_id);
	// if (pCodec == nullptr) {
	// 	cleanup();
	// 	return false;
	// }

	// pCodecCtx = avcodec_alloc_context3(pCodec);

	// if (pCodecCtx == nullptr) {
	// 	cleanup();
	// 	return false;
	// }

	// avcodec_parameters_to_context(pCodecCtx, pCodecParam);

	// if (avcodec_open2(pCodecCtx, pCodec, nullptr) < 0) {
	// 	cleanup();
	// 	return false;
	// }

	// pFrameYUV = av_frame_alloc();
	// pFrameRGB = av_frame_alloc();

	// if (pFrameRGB == nullptr || pFrameYUV == nullptr) {
	// 	cleanup();
	// 	return false;
	// }

	// texture->create(pCodecCtx->width, pCodecCtx->height, Image::FORMAT_RGBA8, Texture::FLAG_FILTER | Texture::FLAG_VIDEO_SURFACE);

	// int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height, 1);
	// frame_buffer.resize(numBytes);
	// PoolVector<uint8_t>::Write w = frame_buffer.write();

	// av_image_fill_arrays(pFrameRGB->data, pFrameRGB->linesize, w.ptr(), AV_PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height, 1);

	// // For scaling TODO: Get custom sizes.
	// sws_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height,
	// 		AV_PIX_FMT_RGB24, SWS_BILINEAR, nullptr, nullptr, nullptr);

	return true;
}

void VideoStreamPlaybackFFMPEG::cleanup() {

	// // All these are FFMPEG resources or file resources.
	// // Not tied to VideoPlayback.

	// if (sws_ctx != nullptr) {
	// 	sws_freeContext(sws_ctx);
	// 	sws_ctx = nullptr;
	// }
	// if (pFrameRGB != nullptr) {
	// 	av_free(pFrameRGB);
	// 	pFrameRGB = nullptr;
	// }
	// if (pFrameYUV != nullptr) {
	// 	av_free(pFrameYUV);
	// 	pFrameYUV = nullptr;
	// }
	// if (pCodecCtx != nullptr) {
	// 	avcodec_close(pCodecCtx);
	// 	pCodecCtx = nullptr;
	// }
	// if (pFormatCtx != nullptr) {
	// 	avformat_close_input(&pFormatCtx);
	// 	pFormatCtx = nullptr;
	// }
	// if (io_buffer) {
	// 	memdelete(io_buffer);
	// }
	// if (pIOCtx != nullptr) {
	// 	av_free(pIOCtx);
	// 	pIOCtx = nullptr;
	// }
	// if (file) {
	// 	memdelete(file);
	// 	file = nullptr;
	// }
}

void VideoStreamPlaybackFFMPEG::update(float p_delta) {

	// if (paused || !playing) {
	// 	return;
	// }

	// time += p_delta;

	// if (av_read_frame(pFormatCtx, &packet) >= 0) {
	// 	if (packet.stream_index == videostream_idx) {
	// 		int x;
	// 		do {
	// 			if (avcodec_send_packet(pCodecCtx, &packet) >= 0) {
	// 				x = avcodec_receive_frame(pCodecCtx, pFrameYUV);
	// 				// Not error and not repeat
	// 				if (x != 0 && x != AVERROR(EAGAIN)) {
	// 					return;
	// 				} else if (x == 0) {
	// 					sws_scale(sws_ctx, (uint8_t const *const *)pFrameYUV->data, pFrameYUV->linesize, 0, pCodecCtx->height, pFrameRGB->data, pFrameRGB->linesize);

	// 					Ref<Image> img = memnew(Image(pCodecCtx->width, pCodecCtx->height, 0, Image::FORMAT_RGBA8, frame_buffer));
	// 					texture->set_data(img);
	// 				}
	// 			} else {
	// 				return;
	// 			}
	// 		} while (x == AVERROR(EAGAIN));
	// 	}
	// 	av_packet_unref(&packet);
	// }
}

// ctor and dtor

VideoStreamPlaybackFFMPEG::VideoStreamPlaybackFFMPEG() {} //:
// playing(false),
// paused(false),
// pIOCtx(nullptr),
// pFormatCtx(nullptr),
// pCodecCtx(nullptr),
// time(0.0),
// file(nullptr),
// texture(memnew(ImageTexture)) {}

VideoStreamPlaybackFFMPEG::~VideoStreamPlaybackFFMPEG() {
	// cleanup();
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
	// TODO
}

void VideoStreamPlaybackFFMPEG::set_paused(bool p_paused) {
	paused = p_paused;
}

Ref<Texture> VideoStreamPlaybackFFMPEG::get_texture() {
	return texture;
}

bool VideoStreamPlaybackFFMPEG::has_loop() const {
	// TODO: Implement looping.
	return false;
}

void VideoStreamPlaybackFFMPEG::set_loop(bool p_enable) {
	// Do nothing
	// TODO: Do something
}

void VideoStreamPlaybackFFMPEG::set_audio_track(int p_idx) {
	// TODO: Audio
}

/* --- NOTE VideoStreamFFMPEG starts here. ----- */
// FIX: remove region
#pragma region // VideoStreamFFMPEG

Ref<VideoStreamPlayback> VideoStreamFFMPEG::instance_playback() {
	Ref<VideoStreamPlaybackFFMPEG> pb = memnew(VideoStreamPlaybackFFMPEG);
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