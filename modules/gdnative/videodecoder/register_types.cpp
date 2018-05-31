/* register_types.h */

#include "class_db.h"
#include "videostream_ffmpeg.h"

void register_videodecoder_types() {
	ClassDB::register_class<VideoStreamFFMPEG>();
}
void unregister_videodecoder_types() {
}