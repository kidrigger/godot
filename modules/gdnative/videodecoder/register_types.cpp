/* register_types.h */

#include "class_db.h"
#include "resource_importer_gdnative.h"
#include "video_stream_gdnative.h"

void register_videodecoder_types() {

#ifdef TOOLS_ENABLED
	Ref<ResourceImporterGDNative> avgdn_import;
	avgdn_import.instance();
	ResourceFormatImporter::get_singleton()->add_importer(avgdn_import);
#endif
	ClassDB::register_class<VideoStreamGDNative>();
}
void unregister_videodecoder_types() {
}