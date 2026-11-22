// CSpawnGroupComponent unit (tools/tu_map.py unit 2703): fn_7101118d5c, the component's constructor:
// the component base, its lists (the holders' list reserving one slot), two callback sets, the
// flags, timers and counters, the named keys and the two string ids. Names and layouts are placeholders.
#include <new>

#include "base/UnkNamedRegistry.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CComponent.h"
#include "game/CSpawnGroupComponent.h"
#include "game/UnkTU341.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CStrId;

extern void* g_unk_7101cb2680[];  // the component's vtable
void fn_710111f9b4(void* self);

// A holder (0x40 bytes): moved by fn_710111fd80, destroyed through its vtable's first slot.
struct UnkSpawnHolder_l118d5c {
    struct Vtbl {
        void (*destroy)(UnkSpawnHolder_l118d5c* self);
    };
    Vtbl* vtable;
    u8 data[0x38];

    UnkSpawnHolder_l118d5c(UnkSpawnHolder_l118d5c&& other) { fn_710111fd80(this, &other); }
    ~UnkSpawnHolder_l118d5c() { vtable->destroy(this); }
};

// The holders' list: grows by 3, one slot reserved from the start.
struct UnkSpawnHolderList_l118d5c {
    CRntVector<UnkSpawnHolder_l118d5c> list;

    UnkSpawnHolderList_l118d5c() : list(5) {
        list.step = 3;
        list.Reserve(1);
    }
};

// A callback set (0x40 bytes): the base part's vtable, then its own.
struct UnkSpawnCallbackSet_l118d5c {
    void** vtable;
    CRntVector<void*> callbacks;
    u32 field_0x30;
    u64 field_0x38;

    UnkSpawnCallbackSet_l118d5c()
        : vtable(g_unk_7101a20998 + 2), callbacks(0xf), field_0x30(0), field_0x38(0) {
        vtable = g_unk_7101a20950 + 2;
    }
};

// A named key, empty at first.
struct UnkSpawnKey_l118d5c {
    base::UnkNamedEntry entry;

    UnkSpawnKey_l118d5c() { CRntString_ConstructFromCString(&entry, g_unk_710162e2ed); }
};

struct UnkSpawnState_l118d5c {
    u8 data[0x38];

    UnkSpawnState_l118d5c() { fn_710111f9b4(this); }
};

struct UnkSpawnGroupComponent_l118d5c {
    void** vtable;
    u8 pad_0x8[0x38];  // the component base
    CRntVector<void*> field_0x40;
    UnkSpawnHolderList_l118d5c holders;  // 0x68
    CRntVector<void*> field_0x90;
    UnkSpawnCallbackSet_l118d5c set_0xb8;
    UnkSpawnCallbackSet_l118d5c set_0xf8;
    bool field_0x138;
    bool field_0x139;
    bool field_0x13a;
    bool field_0x13b;
    bool field_0x13c;
    bool field_0x13d;
    bool field_0x13e;
    bool field_0x13f;
    bool field_0x140;
    u8 pad_0x141[7];
    UnkSpawnKey_l118d5c key_0x148;
    f32 field_0x178;
    f32 field_0x17c;
    f32 field_0x180;
    f32 field_0x184;
    f32 field_0x188;
    f32 field_0x18c;
    s32 field_0x190;
    s32 field_0x194;
    s32 field_0x198;
    u8 pad_0x19c[4];
    bool field_0x1a0;
    s32 field_0x1a4;
    s32 field_0x1a8;
    s32 field_0x1ac;
    s32 field_0x1b0;
    s32 field_0x1b4;
    bool field_0x1b8;
    bool field_0x1b9;
    CStrId id_0x1c0;
    UnkSpawnKey_l118d5c keys[11];  // 0x1c8
    u32 field_0x3d8;
    u8 pad_0x3dc[4];
    UnkSpawnState_l118d5c state;  // 0x3e0
    CStrId id_0x418;

};
static_assert(__builtin_offsetof(UnkSpawnGroupComponent_l118d5c, key_0x148) == 0x148, "layout");
static_assert(__builtin_offsetof(UnkSpawnGroupComponent_l118d5c, id_0x1c0) == 0x1c0, "layout");
static_assert(__builtin_offsetof(UnkSpawnGroupComponent_l118d5c, field_0x3d8) == 0x3d8, "layout");
static_assert(__builtin_offsetof(UnkSpawnGroupComponent_l118d5c, id_0x418) == 0x418, "layout");

void fn_7101118d5c(char* p) {
    auto* self = reinterpret_cast<UnkSpawnGroupComponent_l118d5c*>(p);
    fn_71010c4148(reinterpret_cast<UnkStateObject*>(self));
    self->vtable = g_unk_7101cb2680 + 2;
    new (&self->field_0x40) CRntVector<void*>(0xf);
    new (&self->holders) UnkSpawnHolderList_l118d5c();
    new (&self->field_0x90) CRntVector<void*>(0xf);
    new (&self->set_0xb8) UnkSpawnCallbackSet_l118d5c();
    new (&self->set_0xf8) UnkSpawnCallbackSet_l118d5c();
    self->field_0x138 = false;
    self->field_0x139 = false;
    self->field_0x13a = false;
    self->field_0x13b = false;
    self->field_0x13c = false;
    self->field_0x13d = false;
    self->field_0x13e = false;
    self->field_0x13f = false;
    self->field_0x140 = false;
    new (&self->key_0x148) UnkSpawnKey_l118d5c();
    self->field_0x178 = 0.0f;
    self->field_0x17c = 0.0f;
    self->field_0x180 = 0.0f;
    self->field_0x184 = -1.0f;
    self->field_0x188 = 0.0f;
    self->field_0x18c = 0.0f;
    self->field_0x190 = 0;
    self->field_0x194 = 0;
    self->field_0x198 = 0;
    self->field_0x1a0 = false;
    self->field_0x1a4 = 0;
    self->field_0x1a8 = 0;
    self->field_0x1ac = 0;
    self->field_0x1b0 = 0;
    self->field_0x1b4 = 0;
    self->field_0x1b8 = false;
    self->field_0x1b9 = true;
    new (&self->id_0x1c0) CStrId(g_unk_7101d470e0);
    for (UnkSpawnKey_l118d5c& key : self->keys)
        new (&key) UnkSpawnKey_l118d5c();
    self->field_0x3d8 = 0;
    new (&self->state) UnkSpawnState_l118d5c();
    new (&self->id_0x418) CStrId(g_unk_7101d470e0);
}