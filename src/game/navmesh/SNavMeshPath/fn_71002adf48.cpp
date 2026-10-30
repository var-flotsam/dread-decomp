// In navmesh::SNavMeshPath's unit (unit 366 of tools/tu_map.py): marks the path changed (+0x8) and
// adds the 16-bit id to its array at +0x11984 (the length at +0x11980) unless it is -1 or already
// there. Names and layouts are placeholders.
#include "types.h"

// A fixed array of 16-bit ids with its length in front.
struct UnkNavMeshIdArray {
    u32 count;
    s16 ids[1];
};

struct UnkNavMeshPathIds {
    u8 pad_0x0[0x8];
    bool changed;
    u8 pad_0x9[0x11980 - 0x9];
    UnkNavMeshIdArray list;  // 0x11980
};
static_assert(__builtin_offsetof(UnkNavMeshPathIds, list) == 0x11980, "UnkNavMeshPathIds layout");

// The id's slot, or the end (as std::find).
static inline s16* Find(s16* first, s16* last, s16 id) {
    for (; first != last; ++first)
        if (*first == id)
            return first;
    return last;
}

void fn_71002adf48(UnkNavMeshPathIds* self, s16 id) {
    self->changed = true;
    if (id == -1)
        return;
    UnkNavMeshIdArray& list = self->list;
    u32 count = list.count;
    // The ids are reached past the count (the original's address arithmetic).
    s16* first = reinterpret_cast<s16*>(&list.count + 1);
    s16* last = first + count;
    if (Find(first, last, id) != last)
        return;
    *last = id;
    list.count = count + 1;
}
