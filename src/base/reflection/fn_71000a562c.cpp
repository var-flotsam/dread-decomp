// The base::reflection unit's copy of the file cache set's set-up (base/file/UnkFileCacheSet.h), with
// nodes of 0x20 bytes.
#include "base/file/UnkFileCacheSet.h"

void fn_71000a562c(UnkFileCacheSet* self, u32 buckets, u32 nodes) {
    UnkFileCacheSetSetUp<0x20>(self, buckets, nodes);
}
