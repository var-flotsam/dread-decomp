// CChozoWarriorXUltimateGrabAttack's constructor: CAttack's part (fn_7100493608), then the class's
// vtable, its two listeners (+0x40, +0x60), the property list (+0x80), the twelve property entries at
// +0x148 (ids 1 to 12, each with an empty reflected value) and the state after them. Names and
// layouts are placeholders.
#include "base/reflection/UnkReflValue.h"
#include "game/CAbilityGhostAura.h"
#include "types.h"

void* fn_7100493608(void* attack);  // CAttack's constructor

extern void* g_unk_7101a3b438[];  // the class's vtable
extern void* g_unk_7101a3b560[];  // the entries' vtable
extern void* g_unk_7101a3b610[];  // the property list's vtable

struct UnkUltimateGrabListener_4869b4 {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;

    void Init() {
        vtable = g_unk_7101a22d48 + 2;
        field_0x8 = 0;
        field_0x10 = 0;
        field_0x18 = 0;
    }
};

struct UnkUltimateGrabEntry_4869b4 {
    void** vtable;
    u64 field_0x8;
    u32 id;
    u32 field_0x14;
    base::reflection::UnkReflValue value;
    u64 block[8];

    void Init(u32 entry_id) {
        vtable = g_unk_7101a3b560 + 2;
        field_0x8 = 0;
        id = entry_id;
        value.field_0x0 = nullptr;
        value.type = nullptr;
        value.kind = 2;
        value.field_0x14 = 1;
        for (int i = 0; i < 8; i++)
            block[i] = 0;
    }
};

struct UnkUltimateGrabList_4869b4 {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
    u32 count;
    u8 pad_0x24[0x80];
    u32 limit;  // 0xa4

    void Init() {
        vtable = g_unk_7101a3b610 + 2;
        field_0x8 = 0;
        field_0x10 = 0;
        field_0x18 = 0;
        count = 0;
        limit = 0x7fffffff;
    }
};
static_assert(sizeof(UnkUltimateGrabList_4869b4) == 0xa8, "layout");


struct UnkUltimateGrab_4869b4 {
    void** vtable;
    u8 pad_0x8[0x38];
    UnkUltimateGrabListener_4869b4 listener_a;  // 0x40
    UnkUltimateGrabListener_4869b4 listener_b;  // 0x60
    UnkUltimateGrabList_4869b4 list;            // 0x80
    u16 field_0x128;
    u8 pad_0x12a[0xe];
    f32 field_0x138[4];
    UnkUltimateGrabEntry_4869b4 entries[12];  // 0x148
    u16 field_0x688;
    u8 pad_0x68a[2];
    f32 field_0x68c[25];
    f32 field_0x6f0;
    f32 field_0x6f4[7];
};
static_assert(__builtin_offsetof(UnkUltimateGrab_4869b4, entries) == 0x148, "layout");
static_assert(__builtin_offsetof(UnkUltimateGrab_4869b4, field_0x688) == 0x688, "layout");
static_assert(sizeof(UnkUltimateGrab_4869b4) == 0x710, "layout");

void fn_71004869b4(UnkUltimateGrab_4869b4* self) {
    fn_7100493608(self);
    self->vtable = g_unk_7101a3b438 + 2;
    self->listener_a.Init();
    self->listener_b.Init();
    self->list.Init();
    self->field_0x128 = 0;
    self->field_0x138[0] = 0.0f;
    self->field_0x138[1] = -1.0f;
    self->field_0x138[2] = -1.0f;
    self->field_0x138[3] = -1.0f;
    for (int i = 0; i < 12; i++)
        self->entries[i].Init(i + 1);
    self->field_0x688 = 0;
    for (int i = 0; i < 25; i++)
        self->field_0x68c[i] = 0.0f;
    self->field_0x6f0 = 1.0f;
    for (int i = 0; i < 7; i++)
        self->field_0x6f4[i] = 0.0f;
}