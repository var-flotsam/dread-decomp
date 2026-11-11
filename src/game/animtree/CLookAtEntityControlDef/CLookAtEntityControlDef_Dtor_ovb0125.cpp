// animtree::CLookAtEntityControlDef's unit (tools/tu_map.py unit 417): the deleting destructor of the
// three-level look-at definition (fn_71002e5c40; each level resets its vtable and releases its string id,
// 0x40, 0x10 and 0x8, the last without resetting it since the object is freed) and the copy assignment of
// its record (fn_71002e6134: the three string ids with CStrId's copy assignment, the words between them
// copied plainly). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/animtree/CAnimatedNodeDef.h"
#include "game/animtree/CLookAtEntityControlDef.h"
#include <new>
#include "types.h"

// Deleting destructor of the three-level look-at definition (vtables g_unk_7101a21ef0,
// g_unk_7101a21af8, g_unk_7101a21718): each level resets its vtable and releases its string id
// (0x40, 0x10, 0x8); the last id is not reset to the empty string since the object is freed.


using base::global::CStrId;
using base::global::UnkStringPool;

extern void* g_unk_7101a21af8[];

struct UnkLookAtDef_ovb0125 {
    void** vtable;
    CStrId field_0x8;
    CStrId field_0x10;
    u8 unk_0x18[0x28];
    CStrId field_0x40;
};

void fn_71002e5c40(UnkLookAtDef_ovb0125* self) {
    self->vtable = g_unk_7101a21ef0 + 2;
    self->field_0x40.Reset();
    self->vtable = g_unk_7101a21af8 + 2;
    self->field_0x10.Reset();
    self->vtable = g_unk_7101a21718 + 2;
    if (UnkStringPool* pool = g_unk_7101d47108) {
        base::global::UnkStringData* data = self->field_0x8.data;
        if (data && data != &pool->empty) {
            --data->ref_count;
            if (self->field_0x8.data->ref_count == 0)
                fn_71000803a4(pool, self->field_0x8.data);
        }
    }
    operator delete(self);
}

using base::global::CStrId;

struct UnkLookAtPair_ovb0125 {
    u64 field_0x0;
    u64 field_0x8;
};

struct UnkLookAtRecord_ovb0125 {
    void** vtable;
    CStrId field_0x8;
    CStrId field_0x10;
    UnkLookAtPair_ovb0125 field_0x18;
    UnkLookAtPair_ovb0125 field_0x28;
    u32 field_0x38;
    u32 pad_0x3c;
    CStrId field_0x40;
};

UnkLookAtRecord_ovb0125* fn_71002e6134(UnkLookAtRecord_ovb0125* self, const UnkLookAtRecord_ovb0125* other) {
    self->field_0x8 = other->field_0x8;
    self->field_0x10 = other->field_0x10;
    self->field_0x18 = other->field_0x18;
    __builtin_memcpy(&self->field_0x28, &other->field_0x28, 0x14);
    self->field_0x40 = other->field_0x40;
    return self;
}
