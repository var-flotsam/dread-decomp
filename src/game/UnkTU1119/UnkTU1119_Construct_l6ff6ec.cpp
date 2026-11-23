// Unit 1119: constructor of an object derived from unit 1118's large AI object: runs that constructor, sets
// the vtable and builds the member at +0x12ca8 (a derived kind of 1118's member). Names and layouts are
// placeholders.
#include "base/global/CRntVector.h"
#include "game/behaviortree/CFlyingFollowPathAction.h"
#include "types.h"

struct UnkTU1118Object;
void fn_71006fc4fc(UnkTU1118Object* self, u32 id);

extern char g_unk_7101a6b9c0[];
extern char g_unk_7101a264a8[];

struct UnkTU1119Slot {
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

struct UnkTU1119Flagged {
    u8 flag;
    u64 field_0x8;
    u64 field_0x10;

    void Clear() {
        flag = 0;
        field_0x8 = 0;
        field_0x10 = 0;
    }
};

struct UnkTU1119Entry {
    u8 flag;
    u8 pad_0x1[7];
};

// The member's base part, laid out as unit 1118's member.
struct UnkTU1119MemberBase {
    void* vtable;                    // 0x0
    UnkTU1119Slot field_0x8;         // 0x8
    u8 pad_0x18[4];
    u8 field_0x1c;                   // 0x1c
    u64 field_0x20;                  // 0x20
    u64 field_0x28;                  // 0x28
    u8 field_0x30;                   // 0x30
    u32 field_0x34;                  // 0x34
    u8 pad_0x38[8];
    UnkTU1119Entry field_0x40[7];    // 0x40
    u8 field_0x78;                   // 0x78
    UnkTU1119Slot field_0x7c;        // 0x7c
    u8 pad_0x8c[4];
    u8 field_0x90;                   // 0x90
    u8 pad_0x91;
    u8 field_0x92;                   // 0x92
    u8 pad_0x93;
    u8 field_0x94;                   // 0x94
    u8 pad_0x95[7];
    u8 field_0x9c;                   // 0x9c
    u8 pad_0x9d;
    u8 field_0x9e;                   // 0x9e
    UnkTU1119Flagged field_0xa0[3];  // 0xa0
    u32 field_0xe8;                  // 0xe8
    u8 field_0xec;                   // 0xec
    u32 field_0xf0;                  // 0xf0
    base::UnkAllocator* allocator;   // 0xf8
    u8 field_0x100;                  // 0x100
    u8 pad_0x101;
    u8 field_0x102;                  // 0x102
    u8 pad_0x103;
    u8 field_0x104;                  // 0x104
    u8 pad_0x105;
    u8 field_0x106;                  // 0x106
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
        field_0x90 = 0;
        field_0x92 = 0;
        field_0x94 = 0;
        field_0x9c = 0;
        field_0x9e = 0;
        for (u32 i = 0; i < 3; i++) {
            field_0xa0[i].Clear();
        }
        field_0x7c.Clear();
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
static_assert(sizeof(UnkTU1119MemberBase) == 0x108, "");

struct UnkTU1119Member {
    UnkTU1119MemberBase base;  // 0x0
    u8 pad_0x108[4];
    u8 field_0x10c;  // 0x10c
    u8 pad_0x10d[7];
    u8 field_0x114;  // 0x114
    u8 pad_0x115;
    u8 field_0x116;  // 0x116
    u8 pad_0x117;
    u8 field_0x118;  // 0x118
    u8 pad_0x119;
    u8 field_0x11a;  // 0x11a
    u8 pad_0x11b[5];
    u8 field_0x120;  // 0x120
    u8 pad_0x121[7];
    u8 field_0x128;  // 0x128
    u8 pad_0x129;
    u8 field_0x12a;  // 0x12a
    u8 pad_0x12b[5];
    u8 field_0x130;  // 0x130
    u8 pad_0x131[7];
    u8 field_0x138;  // 0x138
    u8 pad_0x139;
    u8 field_0x13a;  // 0x13a
    u8 pad_0x13b;
    u8 field_0x13c;  // 0x13c
    u8 pad_0x13d[7];
    u8 field_0x144;  // 0x144
    u8 pad_0x145[7];
    u8 field_0x14c;  // 0x14c
    u8 pad_0x14d;
    u8 field_0x14e;  // 0x14e
    u8 pad_0x14f;
    UnkTU1119Entry field_0x150[5];  // 0x150

    void Init() {
        base.Init();
        base.vtable = g_unk_7101a264a8 + 0x10;
        field_0x10c = 0;
        field_0x114 = 0;
        field_0x116 = 0;
        field_0x118 = 0;
        field_0x11a = 0;
        field_0x120 = 0;
        field_0x128 = 0;
        field_0x12a = 0;
        field_0x130 = 0;
        field_0x138 = 0;
        field_0x13a = 0;
        field_0x13c = 0;
        field_0x144 = 0;
        field_0x14c = 0;
        field_0x14e = 0;
        base.field_0x34 = 4;
        for (u32 i = 0; i < 5; i++) {
            field_0x150[i].flag = 0;
        }
    }
};
static_assert(sizeof(UnkTU1119Member) == 0x178, "");

// Three words cleared together (a begin, end and capacity, perhaps).
struct UnkTU1119Triple {
    u64 field_0x0;
    u64 field_0x8;
    u64 field_0x10;

    void Clear() {
        field_0x0 = 0;
        field_0x8 = 0;
        field_0x10 = 0;
    }
};

struct UnkTU1119Object {
    void* vtable;  // 0x0
    u8 pad_0x8[0x12ca0];
    UnkTU1119Member member;  // 0x12ca8
    UnkTU1119Triple field_0x12e20;  // 0x12e20
};
static_assert(__builtin_offsetof(UnkTU1119Object, member) == 0x12ca8, "");

void fn_71006ff6ec(UnkTU1119Object* self, u32 id) {
    fn_71006fc4fc(reinterpret_cast<UnkTU1118Object*>(self), id);
    self->vtable = g_unk_7101a6b9c0 + 0x10;
    self->member.Init();
    self->field_0x12e20.Clear();
}
