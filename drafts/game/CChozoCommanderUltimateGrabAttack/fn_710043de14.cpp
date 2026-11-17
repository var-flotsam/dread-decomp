// CChozoCommanderUltimateGrabAttack's constructor: CAttack's part, the two listeners, the property
// list, the cached floats and the twelve property entries (numbered 1 to 12), then the trailing
// state. Names and layouts are placeholders.
#include "base/reflection/UnkReflValue.h"
#include "game/CAbilityGhostAura.h"
#include "types.h"

extern void* g_unk_7101a362b0[];  // the class's vtable
extern void* g_unk_7101a363f8[];  // the entries' vtable
extern void* g_unk_7101a364a8[];  // the property list's vtable

struct UnkUltimateGrabCtorListener_43de14 {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    void* owner;

    void Init() {
        vtable = g_unk_7101a22d48 + 2;
        field_0x8 = 0;
        field_0x10 = 0;
        owner = nullptr;
    }
};

struct UnkUltimateGrabCtorList_43de14 {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
    u32 count;
    s32 capacity;

    void Init() {
        vtable = g_unk_7101a364a8 + 2;
        field_0x8 = 0;
        field_0x10 = 0;
        field_0x18 = 0;
        count = 0;
        capacity = 0x7fffffff;
    }
};

struct UnkUltimateGrabCtorEntry_43de14 {
    void** vtable;
    u64 field_0x8;
    u32 index;
    u32 pad_0x14;
    base::reflection::UnkReflValue value;
    u64 block[8];

    void Init(u32 i) {
        vtable = g_unk_7101a363f8 + 2;
        field_0x8 = 0;
        index = i;
        value.field_0x0 = nullptr;
        value.type = nullptr;
        value.kind = 2;
        value.field_0x14 = 1;
        for (int j = 0; j < 8; j++)
            block[j] = 0;
    }
};

struct UnkUltimateGrabTail_43de14 {
    u16 field_0x0;
    u8 pad_0x2[2];
    f32 field_0x4[25];
    f32 field_0x68;  // 1
    f32 field_0x6c[7];
};

struct UnkUltimateGrabCtor_43de14 {
    void** vtable;
    u8 pad_0x8[0x40];
    UnkUltimateGrabCtorListener_43de14 listener_a;  // 0x48
    UnkUltimateGrabCtorListener_43de14 listener_b;  // 0x68
    UnkUltimateGrabCtorList_43de14 list;            // 0x88
    u8 pad_0xb0[0x80];
    u16 field_0x130;
    u8 pad_0x132[0xe];
    f32 field_0x140;
    f32 field_0x144;
    f32 field_0x148;
    f32 field_0x14c;
    UnkUltimateGrabCtorEntry_43de14 entries[12];  // 0x150
    UnkUltimateGrabTail_43de14 tail;  // 0x690
};
static_assert(__builtin_offsetof(UnkUltimateGrabCtor_43de14, entries) == 0x150, "layout");
static_assert(__builtin_offsetof(UnkUltimateGrabCtor_43de14, tail) == 0x690, "layout");
static_assert(sizeof(UnkUltimateGrabCtor_43de14) == 0x718, "layout");

void fn_7100426700(UnkUltimateGrabCtor_43de14* self);

void fn_710043de14(UnkUltimateGrabCtor_43de14* self) {
    fn_7100426700(self);
    self->vtable = g_unk_7101a362b0 + 2;
    self->listener_a.Init();
    self->listener_b.Init();
    self->list.Init();
    self->field_0x130 = 0;
    self->field_0x140 = 0.0f;
    self->field_0x144 = -1.0f;
    self->field_0x148 = -1.0f;
    self->field_0x14c = -1.0f;
    for (int i = 0; i < 12; i++)
        self->entries[i].Init(i + 1);
    self->tail = UnkUltimateGrabTail_43de14();
    self->tail.field_0x68 = 1.0f;
}
