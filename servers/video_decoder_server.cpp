/*************************************************************************/
/*  video_decoder_server.cpp                                             */
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

#include "video_decoder_server.h"
#include <servers/video/resource_format_loader_video_stream.h>

VideoDecoderServer *VideoDecoderServer::singleton = nullptr;
Ref<ResourceFormatLoader> VideoDecoderServer::resource_format_loader;

Vector<Ref<VideoStream>> VideoDecoderServer::decoders;
Vector<String> VideoDecoderServer::extensions;
HashMap<String, Vector<Ref<VideoStream>>> VideoDecoderServer::decoder_support;

void VideoDecoderServer::_bind_methods() {
	ClassDB::bind_static_method(get_class_static(), D_METHOD("get_recognized_extensions"), &VideoDecoderServer::get_recognized_extensions);
	ClassDB::bind_static_method(get_class_static(), D_METHOD("add_interface", "extension_stream"), &VideoDecoderServer::add_interface);
	ClassDB::bind_static_method(get_class_static(), D_METHOD("remove_interface", "extension_stream"), &VideoDecoderServer::remove_interface);
	ClassDB::bind_static_method(get_class_static(), D_METHOD("instantiate_stream_for_extension", "extension"), &VideoDecoderServer::instantiate_stream_for_extension);
}

VideoDecoderServer *VideoDecoderServer::get_singleton() {
	return singleton;
}

String VideoDecoderServer::get_extension_name(const String &ext) {
	if (decoder_support.has(ext)) {
		return decoder_support[ext][0]->get_class_name();
	}
	return "";
}

Vector<String> VideoDecoderServer::get_recognized_extensions() {
	return extensions;
}

void VideoDecoderServer::add_interface(const Ref<VideoStream> &extension_stream) {
	ERR_FAIL_COND(extension_stream.is_null());

	create();

	if (decoders.has(extension_stream)) {
		ERR_PRINT("Stream already Registered");
		return;
	}

	decoders.push_back(extension_stream);

	PackedStringArray supported_exts = extension_stream->get_supported_extensions();
	for (auto &new_ext : supported_exts) {
		if (!extensions.has(new_ext)) {
			extensions.push_back(new_ext);
		}
		decoder_support[new_ext].push_back(extension_stream);
	}
}

void VideoDecoderServer::remove_interface(const Ref<VideoStream> &extension_stream) {
	create();

	int index = decoders.find(extension_stream);
	decoders.remove_at(index);

	PackedStringArray supported_ext = extension_stream->get_supported_extensions();
	for (auto &ext : supported_ext) {
		decoder_support[ext].erase(extension_stream);
		if (decoder_support[ext].is_empty()) {
			extensions.erase(ext);
			decoder_support.erase(ext);
		}
	}
}

Ref<VideoStream> VideoDecoderServer::instantiate_stream_for_extension(const String &extension) {
	if (extensions.size() == 0 || !extensions.has(extension)) {
		return nullptr;
	}
	return static_cast<VideoStream*>(ClassDB::instantiate(decoder_support[extension][0]->get_class_name()));
}

VideoDecoderServer::VideoDecoderServer() {
	singleton = this;
	create();
	register_resource_loader();
}

void VideoDecoderServer::create() {
	if (is_created)
		return;

	resource_format_loader.reference_ptr(memnew(ResourceFormatLoaderVideoStreamExtension));

	is_created = true;
}

VideoDecoderServer::~VideoDecoderServer() {
	singleton = nullptr;
	destroy();
}

void VideoDecoderServer::destroy() {
	if (!is_created)
		return;

	unregister_resource_loader();
	resource_format_loader.unref();

	is_created = false;
}

void VideoDecoderServer::register_resource_loader() {
	ResourceLoader::add_resource_format_loader(resource_format_loader, true);
}

void VideoDecoderServer::unregister_resource_loader() {
	ResourceLoader::remove_resource_format_loader(resource_format_loader);
}
