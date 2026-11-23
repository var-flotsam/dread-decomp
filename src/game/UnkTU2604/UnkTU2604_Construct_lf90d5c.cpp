// Unit 2604: the constructor of an object holding an empty string id, an empty file path, a second empty
// string id and an array of 11 null slots. Names and layouts are placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkRenderPassSlots.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

// The slots of game/UnkRenderPassSlots.h with flags 0xb.
struct UnkTU2604Slots {
    base::UnkAllocator* allocator;
    u32 field_0x8;
    u32 field_0xc;
    base::global::CRntVector<void*> slots;
    u64 field_0x38;
    u64 field_0x40;
};

// The slots' constructor: 11 null slots.
static void InitSlots(UnkTU2604Slots* s) {
    s->allocator = GetDefaultAllocator();
    s->field_0x8 = 0xb;
    s->field_0xc = 0;
    new (&s->slots) base::global::CRntVector<void*>(0xb);
    s->field_0x38 = 0;
    s->field_0x40 = 0;
    UnkResizeSlots(s->slots, 11);
}

struct UnkTU2604Named {
    CStrId name;
    CFilePathStrId path;  // 0x8
    CStrId field_0x10;
    u8 pad_0x18[0x18];
    UnkTU2604Slots slots;  // 0x30
};
static_assert(__builtin_offsetof(UnkTU2604Named, slots) == 0x30, "layout");

static inline void InitEmptyStrId(CStrId* id) {
    id->data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    id->data = &g_unk_7101d47108->empty;
}

void fn_7100f90d5c(UnkTU2604Named* self) {
    InitEmptyStrId(&self->name);
    new (&self->path) CFilePathStrId();
    InitEmptyStrId(&self->field_0x10);
    InitSlots(&self->slots);
}
