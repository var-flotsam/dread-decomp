// Unit 365 (navmesh::CNavMeshGenerator): appends a 48-byte element to a fixed array of 128 and
// reports whether it fitted, and whether it is now full. Names and layouts are placeholders.
#include "types.h"

typedef u32 __attribute__((may_alias)) a_u32;

struct UnkElem_0x30 {
    a_u32 words[12];
};

struct UnkFixedList_l2a7298 {
    u32 count;
    UnkElem_0x30 items[128];
};

struct UnkOwner_l2a7298 {
    u8 pad_0x0[8];
    UnkFixedList_l2a7298 list;  // 0x8
};

struct UnkAppendResult_l2a7298 {
    u32 code;
};

UnkAppendResult_l2a7298 fn_71002a7298(UnkOwner_l2a7298* self, const UnkElem_0x30* e) {
    UnkAppendResult_l2a7298 r;
    UnkFixedList_l2a7298* l = &self->list;
    s32 room = (s32)(128u - l->count);
    if (room < 1) {
        r.code = 2;
        return r;
    }
    UnkElem_0x30* d = &l->items[l->count];
    for (int i = 0; i < 12; ++i)
        d->words[i] = e->words[i];
    l->count += 1;
    r.code = room == 1 ? 3 : 1;
    return r;
}
