// behaviortree::CAutoolGoToRepairPointAction's unit (tools/tu_map.py): the action's constructor. After the
// action base constructor it builds six numbered value holders, the cleared object at +0x2d0 and the
// listener at +0x380, then calls the node's slot 0x18. Names and layouts are placeholders.
#include "game/CAbilityGhostAura.h"
#include "game/UnkBehaviorTree.h"
#include "game/behaviortree/CAutoolGoToRepairPointAction.h"
#include "types.h"

#include <new>

extern void* g_unk_7101a2d048[];

struct UnkAutoolPair_l3c269c {
    u64 a;
    u64 b;
    UnkAutoolPair_l3c269c() : a(0), b(0) {}
};

// The holders' base class (vtable g_unk_7101a2d0a0, as the destructor restores it).
struct UnkAutoolValueBase_l3c269c {
    void** vtable;
    u64 field_0x8;
    u32 index;  // 0x10
    void* value;  // 0x18
    const char* type;  // 0x20
    u32 kind;  // 0x28
    u32 by_ref;  // 0x2c
    UnkAutoolPair_l3c269c pairs[4];  // 0x30

    explicit UnkAutoolValueBase_l3c269c(u32 i)
        : vtable(reinterpret_cast<void**>(g_unk_7101a2d0a0) + 2), field_0x8(0), index(i), value(nullptr),
          type(nullptr), kind(2), by_ref(1) {}
};

struct UnkAutoolValue_l3c269c : UnkAutoolValueBase_l3c269c {
    explicit UnkAutoolValue_l3c269c(u32 i) : UnkAutoolValueBase_l3c269c(i) { vtable = &g_unk_7101a2d048[2]; }
};
static_assert(sizeof(UnkAutoolValue_l3c269c) == 0x70, "layout");

struct UnkAutoolCleared_l3c269c {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
    u32 field_0x20;

    UnkAutoolCleared_l3c269c()
        : vtable(&g_unk_7101a2d008[2]), field_0x8(0), field_0x10(0), field_0x18(0), field_0x20(0) {}
};

struct UnkAutoolListener_l3c269c {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    void* link;  // 0x18
    s64 field_0x20;
    s64 field_0x28;

    UnkAutoolListener_l3c269c()
        : vtable(&g_unk_7101a22d48[2]), field_0x8(0), field_0x10(0), link(nullptr), field_0x20(-1),
          field_0x28(-1) {}
};

struct UnkAutoolGoToRepairPoint_l3c269c : UnkBehaviorTreeNodeBase {
    u8 pad_0x2c[4];
    UnkAutoolValue_l3c269c values[6];  // 0x30
    UnkAutoolCleared_l3c269c cleared;  // 0x2d0
    u8 pad_0x2f8[0x7c];
    u32 field_0x374;
    u16 field_0x378;
    UnkAutoolListener_l3c269c listener;  // 0x380
};
static_assert(__builtin_offsetof(UnkAutoolGoToRepairPoint_l3c269c, listener) == 0x380, "layout");

// The node as its virtual interface sees it.
struct UnkAutoolNode_l3c269c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
};

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_71003c269c(UnkAutoolGoToRepairPoint_l3c269c* self) {
    fn_7100393e68(self);
    self->vtable = &g_unk_7101a2cf58[2];
    for (u32 i = 0; i < 6; ++i)
        new (&self->values[i]) UnkAutoolValue_l3c269c(i + 1);
    new (&self->cleared) UnkAutoolCleared_l3c269c();
    self->field_0x374 = 7;
    self->field_0x378 = 0;
    new (&self->listener) UnkAutoolListener_l3c269c();
    reinterpret_cast<UnkAutoolNode_l3c269c*>(self)->vf_0x18();
}
