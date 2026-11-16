// CChozoCommanderKiGrabAttack's constructor (unit 652): the base attack's constructor, then the two
// listeners, the property list, the timing fields and the seven property entries (numbered 1 to 7),
// and the zeroed state after them. Names and layouts are placeholders.
#include "game/CAbilityGhostAura.h"
#include "types.h"

extern void* g_unk_7101a35b28[];  // the class's vtable
extern void* g_unk_7101a35c40[];  // the entries' vtable
extern void* g_unk_7101a35cf0[];  // the property list's vtable

void fn_7100426700(void* self);  // the base attack's constructor

struct UnkKiGrabListener_l43399c {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    u64 owner;

    void Init() {
        field_0x10 = 0;
        owner = 0;
        vtable = g_unk_7101a22d48 + 2;
        field_0x8 = 0;
    }
};

struct UnkKiGrabValue_l43399c {
    void* field_0x0;
    const char* type;
    u32 kind;
    u32 field_0x14;
};

struct UnkKiGrabEntry_l43399c {
    void** vtable;
    u64 field_0x8;
    u32 index;
    u32 pad_0x14;
    UnkKiGrabValue_l43399c value;
    u64 block[8];

    void Init(u32 i) {
        field_0x8 = 0;
        index = i;
        value.field_0x0 = nullptr;
        value.type = nullptr;
        value.kind = 2;
        value.field_0x14 = 1;
        vtable = g_unk_7101a35c40 + 2;
        for (int k = 0; k < 8; k++)
            block[k] = 0;
    }
};

struct UnkKiGrabTail_l43399c {
    u64 field_0x0;
    u64 field_0x8;
    u64 field_0x10;
    u32 field_0x18;
};

struct UnkKiGrab_l43399c {
    void** vtable;
    u8 pad_0x8[0x40];
    UnkKiGrabListener_l43399c listener_a;  // 0x48
    UnkKiGrabListener_l43399c listener_b;  // 0x68
    void** list_vtable;                     // 0x88
    u64 list_field_0x8;                     // 0x90
    u64 list_field_0x10;                    // 0x98
    u64 list_field_0x18;                    // 0xa0
    u32 list_count;                         // 0xa8
    u8 pad_0xac[0x80];
    u32 limit;     // 0x12c
    u16 field_0x130;
    u8 pad_0x132[0xe];
    f32 field_0x140;
    f32 field_0x144;
    f32 field_0x148;
    f32 field_0x14c;
    UnkKiGrabEntry_l43399c entries[7];  // 0x150
    u16 field_0x460;
    u8 pad_0x462[2];
    u32 field_0x464[25];
    f32 scale;  // 0x4c8
    UnkKiGrabTail_l43399c tail;  // 0x4cc
};
static_assert(__builtin_offsetof(UnkKiGrab_l43399c, limit) == 0x12c, "layout");
static_assert(__builtin_offsetof(UnkKiGrab_l43399c, entries) == 0x150, "layout");
static_assert(__builtin_offsetof(UnkKiGrab_l43399c, field_0x460) == 0x460, "layout");
static_assert(__builtin_offsetof(UnkKiGrab_l43399c, scale) == 0x4c8, "layout");

void fn_710043399c(UnkKiGrab_l43399c* self) {
    fn_7100426700(self);
    self->vtable = g_unk_7101a35b28 + 2;
    self->listener_a.Init();
    self->listener_b.Init();
    self->list_count = 0;
    self->list_field_0x10 = 0;
    self->list_field_0x18 = 0;
    self->list_vtable = g_unk_7101a35cf0 + 2;
    self->list_field_0x8 = 0;
    self->limit = 0x7fffffff;
    self->field_0x130 = 0;
    self->field_0x140 = 0.0f;
    self->field_0x144 = -1.0f;
    self->field_0x148 = -1.0f;
    self->field_0x14c = -1.0f;
    for (int i = 0; i < 7; i++)
        self->entries[i].Init(i + 1);
    self->field_0x460 = 0;
    for (int i = 0; i < 25; i++)
        self->field_0x464[i] = 0;
    self->scale = 1.0f;
    self->tail.field_0x0 = 0;
    self->tail.field_0x8 = 0;
    self->tail.field_0x10 = 0;
    self->tail.field_0x18 = 0;
}
