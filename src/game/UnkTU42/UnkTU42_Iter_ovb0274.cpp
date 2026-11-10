// Unit 42 of tools/tu_map.py: a filtered walk over a sorted array of object pointers. The cursor
// (+0x18) steps through the slice, objects whose flags pass the iterator's mask/filter get their
// vf_0x18 called with a {token, context} pair, and the walk stops once the call bumped the token
// (+0x8). Names and layouts are placeholders.
#include "types.h"

struct UnkWalkObject_ovb0274;

struct UnkWalkCallArgs_ovb0274 {
    u64 token;
    u64 context;
};

struct UnkWalkObjectVtable_ovb0274 {
    void* slots[3];
    void (*visit)(UnkWalkObject_ovb0274* self, UnkWalkCallArgs_ovb0274* args);  // 0x18
};

struct UnkWalkObject_ovb0274 {
    UnkWalkObjectVtable_ovb0274* vtable;
    u32 flags;  // 0x8
};

struct UnkWalkList_ovb0274 {
    UnkWalkObject_ovb0274** data;  // 0x0
    u8 pad_0x8[8];
    u32 count;  // 0x10
};

struct UnkWalk_ovb0274 {
    UnkWalkList_ovb0274* list;  // 0x0
    u64 token;                  // 0x8
    u64 context;                // 0x10
    UnkWalkObject_ovb0274** cursor;  // 0x18
    u32 filter;                 // 0x20
    u8 mask;                    // 0x24
    u8 pad_0x25[3];
    u8 active;                  // 0x28
};

UnkWalkObject_ovb0274** fn_71000699a0(UnkWalkList_ovb0274* list, f32 key);
UnkWalkObject_ovb0274** fn_710006993c(UnkWalkList_ovb0274* list, f32 key);
bool fn_71000687d8(UnkWalk_ovb0274* self, UnkWalkObject_ovb0274** begin, UnkWalkObject_ovb0274** end, bool skipFlagged);

bool fn_7100068950(UnkWalk_ovb0274* self, f32 key, bool fromStart, bool toEnd, bool skipFlagged) {
    if (!self->list || !self->list->count)
        return false;
    if (!fromStart && !toEnd) {
        self->cursor = fn_710006993c(self->list, key);
        return false;
    }
    UnkWalkObject_ovb0274** lower = fromStart ? self->cursor : fn_710006993c(self->list, key);
    UnkWalkObject_ovb0274** upper = toEnd ? fn_71000699a0(self->list, key) : fn_710006993c(self->list, key);
    if (self->cursor > lower)
        self->cursor = lower;
    if (lower > upper)
        upper = &self->list->data[self->list->count];
    return fn_71000687d8(self, lower, upper, skipFlagged);
}
