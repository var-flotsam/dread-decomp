// The CMinimapManager unit's copy of the file cache set's set-up (base/file/UnkFileCacheSet.h), with
// nodes of 0x1600 bytes: the pooled set src/game/CMinimapManager.cpp keeps at +0xd64210.
#include "base/file/UnkFileCacheSet.h"

void fn_7100eb2cc0(void* self, u32 buckets, u32 nodes) {
    UnkFileCacheSetSetUp<0x1600>(static_cast<UnkFileCacheSet*>(self), buckets, nodes);
}
