// The CFileCacheDevice unit's chained hash set (base/file/UnkFileCacheSet.h): setting up its storage,
// with nodes of 0x18 bytes.
#include "base/file/UnkFileCacheSet.h"

void fn_7100086510(UnkFileCacheSet* self, u32 buckets, u32 nodes) {
    UnkFileCacheSetSetUp<0x18>(self, buckets, nodes);
}
