// CModelUpdaterComponent unit (tools/tu_map.py): the model updater's constructor: the scene component's
// constructor, two empty file paths, an empty string id, an event list and the set-up record, then a
// reset of its state. Names and layouts are placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CModelUpdaterComponent.h"
#include "game/CSceneComponent.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::UnkStringData;

struct UnkModelSetup_ovb0032;
void fn_7101170da0(UnkModelSetup_ovb0032* self);

// A list of events (g_unk_7101cb46a0's vtable) with its allocator and flags.
struct UnkModelUpdaterEvents_l170b2c {
    void** vtable;
    u32 count;  // 0x8
    void* head;  // 0x10
    void* tail;  // 0x18
    base::UnkAllocator* allocator;  // 0x20
    u32 flags;  // 0x28
    u64 field_0x30;
};

struct UnkModelUpdater_l170b2c {
    void** vtable;
    u8 pad_0x8[0x40];
    CFilePathStrId loaded_path;  // 0x48
    void* model;  // 0x50
    CFilePathStrId path;  // 0x58
    u64 field_0x60;
    bool field_0x68;
    u64 field_0x70;
    u64 field_0x78;
    u64 field_0x80;
    UnkStringData* name;  // 0x88
    f32 scale;  // 0x90
    f32 field_0x94[5];
    f32 field_0xa8;
    f32 field_0xac;
    u8 pad_0xb0[0x3c];
    f32 field_0xec[3];
    s64 field_0xf8;
    u64 field_0x100;
    u64 field_0x108;
    u64 field_0x110;
    bool field_0x118;
    UnkModelUpdaterEvents_l170b2c events;  // 0x120
    bool field_0x158;
    u8 pad_0x159[0x7];
    u8 setup[0x88];  // 0x160
};
static_assert(__builtin_offsetof(UnkModelUpdater_l170b2c, events) == 0x120, "layout");
static_assert(__builtin_offsetof(UnkModelUpdater_l170b2c, setup) == 0x160, "layout");

static inline void InitEvents(UnkModelUpdaterEvents_l170b2c* e) {
    e->vtable = g_unk_7101cb46a0 + 2;
    e->count = 0;
    e->head = nullptr;
    e->tail = nullptr;
    e->allocator = fn_7100083990();
    e->flags = 0x13;
    e->field_0x30 = 0;
}

void fn_7101170b2c(UnkModelUpdater_l170b2c* self) {
    fn_71009f1910(reinterpret_cast<UnkBreakableTileGroup_b46a2*>(self));
    self->vtable = g_unk_7101cb4508 + 2;
    new (&self->loaded_path) CFilePathStrId();
    new (&self->path) CFilePathStrId();
    self->field_0x70 = 0;
    self->field_0x78 = 0;
    self->field_0x80 = 0;
    self->name = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->name = &g_unk_7101d47108->empty;
    self->scale = 1.0f;
    for (int i = 0; i < 5; ++i)
        self->field_0x94[i] = 0.0f;
    self->field_0xa8 = 0.0f;
    self->field_0xac = 0.0f;
    for (int i = 0; i < 3; ++i)
        self->field_0xec[i] = 0.0f;
    self->field_0xf8 = -1;
    self->field_0x100 = 0;
    self->field_0x108 = 0;
    self->field_0x110 = 0;
    InitEvents(&self->events);
    fn_7101170da0(reinterpret_cast<UnkModelSetup_ovb0032*>(self->setup));

    self->model = nullptr;
    self->field_0xa8 = 0.0f;
    self->field_0xac = 0.0f;
    for (int i = 0; i < 3; ++i)
        self->field_0xec[i] = 0.0f;
    self->field_0x158 = false;
    self->field_0x60 = 0;
    self->field_0x68 = false;
    self->field_0x110 = 0;
    self->field_0x118 = false;
    self->field_0x100 = 0;
    self->field_0x108 = 0;
}
