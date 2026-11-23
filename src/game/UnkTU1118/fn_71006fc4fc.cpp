// Unit 1118: constructor of a large AI object: runs the base constructor, sets both vtables and clears
// every member. Names and layouts are placeholders.
#include "game/behaviortree/CFlyingFollowPathAction.h"
#include "types.h"
#include "base/global/CRntVector.h"
#include "game/UnkTU1124.h"
#include "game/navmesh/SNavMeshPath.h"

extern char g_unk_7101a6b910[];

struct UnkTU1118Slot {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u8 field_0xc;

    void Clear() {
        field_0x0 = 0;
        field_0x4 = 0;
        field_0x8 = 0;
        field_0xc = 0;
    }
};

struct UnkTU1118Flagged {
    u8 flag;
    u64 field_0x8;
    u64 field_0x10;

    void Clear() {
        flag = 0;
        field_0x8 = 0;
        field_0x10 = 0;
    }
};

struct UnkTU1118Entry {
    u8 flag;
    u8 pad_0x1[7];
};

struct UnkTU1118Member {
    void* vtable;                     // 0x0
    UnkTU1118Slot field_0x8;          // 0x8
    u8 pad_0x18[4];
    u8 field_0x1c;                    // 0x1c
    u64 field_0x20;                   // 0x20
    u64 field_0x28;                   // 0x28
    u8 field_0x30;                    // 0x30
    u32 field_0x34;                   // 0x34
    u8 pad_0x38[8];
    UnkTU1118Entry field_0x40[7];     // 0x40
    u8 field_0x78;                    // 0x78
    UnkTU1118Slot field_0x7c;         // 0x7c
    u8 pad_0x8c[4];
    u8 field_0x90;                    // 0x90
    u8 pad_0x91;
    u8 field_0x92;                    // 0x92
    u8 pad_0x93;
    u8 field_0x94;                    // 0x94
    u8 pad_0x95[7];
    u8 field_0x9c;                    // 0x9c
    u8 pad_0x9d;
    u8 field_0x9e;                    // 0x9e
    UnkTU1118Flagged field_0xa0[3];   // 0xa0
    u32 field_0xe8;                   // 0xe8
    u8 field_0xec;                    // 0xec
    u32 field_0xf0;                   // 0xf0
    base::UnkAllocator* allocator;    // 0xf8
    u8 field_0x100;                   // 0x100
    u8 pad_0x101;
    u8 field_0x102;                   // 0x102
    u8 pad_0x103;
    u8 field_0x104;                   // 0x104
    u8 pad_0x105;
    u8 field_0x106;                   // 0x106
    u8 pad_0x107;

    void Init() {
        field_0x8.Clear();
        field_0x1c = 0;
        field_0x20 = 0;
        field_0x28 = 0;
        field_0x30 = 0;
        field_0x34 = 0;
        vtable = g_unk_7101a264d0 + 0x10;
        for (u32 i = 0; i < 7; i++) {
            field_0x40[i].flag = 0;
        }
        field_0x78 = 0;
        field_0x7c.Clear();
        field_0x90 = 0;
        field_0x92 = 0;
        field_0x94 = 0;
        field_0x9c = 0;
        field_0x9e = 0;
        for (u32 i = 0; i < 3; i++) {
            field_0xa0[i].Clear();
        }
        field_0xe8 = 10;
        field_0xec = 1;
        field_0xf0 = 15;
        allocator = GetDefaultAllocator();
        field_0x100 = 0;
        field_0x102 = 0;
        field_0x104 = 0;
        field_0x106 = 0;
    }
};
static_assert(__builtin_offsetof(UnkTU1118Member, field_0xa0) == 0xa0, "");
static_assert(sizeof(UnkTU1118Member) == 0x108, "");

struct UnkTU1118Object {
    void* vtable;                     // 0x0
    u8 pad_0x8[0xa4];
    s32 field_0xac;                   // 0xac
    u8 pad_0xb0[4];
    s32 field_0xb4;                   // 0xb4
    u8 pad_0xb8[4];
    UnkTU1118Slot field_0xbc;         // 0xbc
    u32 field_0xcc[12];               // 0xcc
    u8 pad_0xfc[4];
    UnkTU1118Member member;           // 0x100
    u8 field_0x208[0x12a88];          // 0x208
    UnkTU1118Slot field_0x12c90;      // 0x12c90
    f32 field_0x12ca0;                // 0x12ca0
};
static_assert(__builtin_offsetof(UnkTU1118Object, field_0x12c90) == 0x12c90, "");

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_71006fc4fc(UnkTU1118Object* self, u32 id) {
    fn_7100703814(reinterpret_cast<UnkUnit1124Object*>(self), id);
    self->vtable = g_unk_7101a6b910 + 0x10;
    self->field_0xac = -0x7fffffff;
    self->field_0xb4 = -0x7fffffff;
    self->field_0xbc.Clear();
    for (u32 i = 0; i < 12; i++) {
        self->field_0xcc[i] = 0;
    }
    self->member.Init();
    self->member.field_0x34 = 2;
    fn_71002acfe4(self->field_0x208);
    self->field_0x12c90.Clear();
    self->field_0x12ca0 = 3.4028235e38f;
}
