// game::logic::collision::CBroadphaseObject's unit: the constructor, on the game object base
// (fn_7101043904): both vtables, an empty name id, two empty boxes, identity current and previous
// transforms and an empty handler list. Names and layouts are placeholders.
#include <float.h>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CGameObject.h"
#include "game/logic/collision/CBroadphaseObject.h"
#include "types.h"

using base::global::UnkStringData;

struct UnkBroadphaseHandlerNode;

struct UnkBox_ld357d8 {
    f32 min_x;
    f32 min_y;
    f32 max_x;
    f32 max_y;
};

struct UnkBroadphaseObject_ld357d8 {
    char* vtable;
    u8 pad_0x8[0x8];
    u64 field_0x10;
    s64 field_0x18;
    UnkStringData* name;  // 0x20
    bool field_0x28;
    u8 pad_0x29[7];
    u64 field_0x30;
    s32 field_0x38;
    bool field_0x3c;
    u8 pad_0x3d[3];
    UnkBox_ld357d8 box_0x40;
    UnkBox_ld357d8 box_0x50;
    bool field_0x60;
    u8 pad_0x61[3];
    u32 field_0x64;
    bool field_0x68;
    bool field_0x69;
    bool field_0x6a;
    bool field_0x6b;
    f32 xform[16];       // 0x6c
    f32 prev_xform[16];  // 0xac
    u8 pad_0xec[4];
    char* vtable_0xf0;
    s32 count;                       // 0xf8
    UnkBroadphaseHandlerNode* head;  // 0x100
    UnkBroadphaseHandlerNode* tail;  // 0x108
    base::IAllocator* allocator;   // 0x110
    u32 flags;                       // 0x118
    u8 pad_0x11c[4];
    bool busy;  // 0x120
};
static_assert(__builtin_offsetof(UnkBroadphaseObject_ld357d8, busy) == 0x120, "layout");

static inline void ClearBox(UnkBox_ld357d8& box) {
    box.min_x = FLT_MAX;
    box.min_y = FLT_MAX;
    box.max_x = -FLT_MAX;
    box.max_y = -FLT_MAX;
}

static inline void SetIdentity(f32* m) {
    for (int i = 0; i < 16; ++i)
        m[i] = 0.0f;
    m[15] = 1.0f;
    m[10] = 1.0f;
    m[5] = 1.0f;
    m[0] = 1.0f;
}

void fn_7100d357d8(UnkBroadphaseObject_ld357d8* self) {
    fn_7101043904(self);
    self->vtable = g_unk_7101aa84e0 + 0x10;
    self->field_0x10 = 0;
    self->field_0x18 = -1;
    self->name = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->name = &g_unk_7101d47108->empty;
    self->field_0x28 = false;
    self->field_0x30 = 0;
    self->field_0x38 = -1;
    self->field_0x3c = false;
    ClearBox(self->box_0x40);
    ClearBox(self->box_0x50);
    self->field_0x60 = true;
    self->field_0x64 = 0;
    self->field_0x68 = true;
    self->field_0x69 = true;
    self->field_0x6a = false;
    self->field_0x6b = false;
    SetIdentity(self->xform);
    SetIdentity(self->prev_xform);
    self->vtable_0xf0 = g_unk_7101aa8590 + 0x10;
    self->count = 0;
    self->head = nullptr;
    self->tail = nullptr;
    self->allocator = GetDefaultAllocator();
    self->flags = 0x13;
    self->busy = false;
}
