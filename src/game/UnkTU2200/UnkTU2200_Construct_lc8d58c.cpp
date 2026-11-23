// Unit 2200 (hint CCutSceneDef): the constructor of the 0xa0-byte record: empty string and path ids, an
// empty id array and an empty id map (step 10, flags 0xe) and a number of -1. Names and layouts are
// placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntSmallDictionary.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkStrIdPair_b239.h"
#include "types.h"

#include <new>

using base::global::CFilePathStrId;
using base::global::CStrId;

using UnkStrIdMap_b239 = base::global::CRntSmallDictionary<CStrId, UnkStrIdPair_b239>;

using UnkIdArray_b239 = base::global::CRntSmallDictionary<CStrId, u32>;

struct UnkElemA0_b42a3 {
    CStrId field_0x0;
    CFilePathStrId field_0x8;
    CStrId field_0x10;
    CStrId field_0x18;
    u16 field_0x20;
    u32 field_0x24;
    UnkIdArray_b239 field_0x28;
    u64 field_0x50;
    CStrId field_0x58;
    CStrId field_0x60;
    u32 field_0x68;
    UnkStrIdMap_b239 field_0x70;
    CFilePathStrId field_0x98;
};
static_assert(sizeof(UnkElemA0_b42a3) == 0xa0, "UnkElemA0_b42a3 layout");

// An empty id, creating the pool first when there is none.
static inline void MakeEmpty(CStrId* id) {
    id->data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    id->data = &g_unk_7101d47108->empty;
}

void fn_7100c8d58c(UnkElemA0_b42a3* self) {
    MakeEmpty(&self->field_0x0);
    new (&self->field_0x8) CFilePathStrId();
    MakeEmpty(&self->field_0x10);
    MakeEmpty(&self->field_0x18);
    self->field_0x24 = 0;
    self->field_0x28.values = nullptr;
    self->field_0x28.keys = nullptr;
    self->field_0x28.count = 0;
    self->field_0x28.capacity = 0;
    self->field_0x28.step = 10;
    self->field_0x28.flags = 0xe;
    self->field_0x28.allocator = GetDefaultAllocator();
    self->field_0x50 = -1;
    MakeEmpty(&self->field_0x58);
    MakeEmpty(&self->field_0x60);
    self->field_0x70.values = nullptr;
    self->field_0x70.keys = nullptr;
    self->field_0x70.count = 0;
    self->field_0x70.capacity = 0;
    self->field_0x70.step = 10;
    self->field_0x70.flags = 0xe;
    self->field_0x70.allocator = GetDefaultAllocator();
    new (&self->field_0x98) CFilePathStrId();
}
