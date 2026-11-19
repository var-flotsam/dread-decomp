// A constructor in CChozoCommanderKiGrabAttack's unit (652) for a sibling attack class: the base
// attack's constructor, the two listeners, the property list, the timing fields, the seven property
// entries (numbered 1 to 7) and two zeroed arrays of 2D vectors. Names and layouts are placeholders.
#include "game/CAbilityGhostAura.h"
#include "game/CChozoCommanderKiGrabAttack.h"
#include "types.h"

extern void* g_unk_7101a35c40[];  // the entries' vtable

void fn_7100426700(void* self);  // the base attack's constructor

struct UnkKiGrabListener_l430124 {
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

struct UnkKiGrabValue_l430124 {
    void* field_0x0;
    const char* type;
    u32 kind;
    u32 field_0x14;
};

struct UnkKiGrabEntry_l430124 {
    void** vtable;
    u64 field_0x8;
    u32 index;
    u32 pad_0x14;
    UnkKiGrabValue_l430124 value;
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

struct UnkKiGrabVec2_l430124 {
    f32 x;
    f32 y;
};

// The property list; its limit sits at its end.
struct UnkKiGrabList_l430124 {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
    u32 count;
    u8 pad_0x24[0x80];
    u32 limit;  // 0xa4

    void Init() {
        vtable = g_unk_7101a35cf0 + 2;
        field_0x8 = 0;
        field_0x10 = 0;
        field_0x18 = 0;
        count = 0;
        limit = 0x7fffffff;
    }
};

struct UnkKiGrab_l430124 {
    void** vtable;
    u8 pad_0x8[0x40];
    UnkKiGrabListener_l430124 listener_a;  // 0x48
    UnkKiGrabListener_l430124 listener_b;  // 0x68
    UnkKiGrabList_l430124 list;             // 0x88
    u16 field_0x130;
    u8 pad_0x132[0xe];
    f32 field_0x140;
    f32 field_0x144;
    f32 field_0x148;
    f32 field_0x14c;
    UnkKiGrabEntry_l430124 entries[7];  // 0x150
    u16 field_0x460;
    u8 pad_0x462[2];
    UnkKiGrabVec2_l430124 points[12];  // 0x464
    u32 point_count;                   // 0x4c4
    f32 scale;                         // 0x4c8
    UnkKiGrabVec2_l430124 extra[3];    // 0x4cc
    u32 extra_count;                   // 0x4e4
};
static_assert(__builtin_offsetof(UnkKiGrab_l430124, field_0x130) == 0x130, "layout");
static_assert(__builtin_offsetof(UnkKiGrab_l430124, entries) == 0x150, "layout");
static_assert(__builtin_offsetof(UnkKiGrab_l430124, field_0x460) == 0x460, "layout");
static_assert(__builtin_offsetof(UnkKiGrab_l430124, scale) == 0x4c8, "layout");

// NON_MATCHING(functional): Operations differ from the original; behaves the same on every input tools/equiv_test.py --any-kind tried, with full coverage. Owes a match.
void fn_7100430124(UnkKiGrab_l430124* self) {
    fn_7100426700(self);
    self->vtable = g_unk_7101a35b28 + 2;
    self->listener_a.Init();
    self->listener_b.Init();
    self->list.Init();
    self->field_0x130 = 0;
    self->field_0x140 = 0.0f;
    self->field_0x144 = -1.0f;
    self->field_0x148 = -1.0f;
    self->field_0x14c = -1.0f;
    for (int i = 0; i < 7; i++)
        self->entries[i].Init(i + 1);
    self->field_0x460 = 0;
    const UnkKiGrabVec2_l430124 zero = {0.0f, 0.0f};
    for (int i = 0; i < 12; i++)
        self->points[i] = zero;
    self->point_count = 0;
    self->scale = 1.0f;
    for (int i = 0; i < 3; i++)
        self->extra[i] = zero;
    self->extra_count = 0;
}
