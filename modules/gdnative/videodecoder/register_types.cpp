/* register_types.h */

#include "class_db.h"
#include "resource_importer_ffmpeg.h"
#include "videostream_ffmpeg.h"

void register_videodecoder_types() {

#ifdef TOOLS_ENABLED
	Ref<ResourceImporterFFMPEG> ffmpeg_import;
	ffmpeg_import.instance();
	ResourceFormatImporter::get_singleton()->add_importer(ffmpeg_import);
#endif
	ClassDB::register_class<VideoStreamFFMPEG>();
}
void unregister_videodecoder_types() {
}