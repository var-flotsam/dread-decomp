// CAbility's unit (tools/tu_map.py unit 427): CAbility's constructor: the base fn_7101043904, the member at
// +0x10, the state and flags, and six value holders numbered 1 to 6. Names and layouts are placeholders.
#include <new>

#include "base/reflection/UnkReflValue.h"
#include "game/CAbility.h"
#include "game/CGameObject.h"
#include "types.h"

struct UnkGhostAuraBase_b0040;

extern void* g_unk_7101a22650[];  // the vtable of the member at +0x10
extern void* g_unk_7101a225a0[];  // the value holders' vtable

struct UnkAbilityMember_l2f8414 {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
    u32 field_0x20;

    UnkAbilityMember_l2f8414() : vtable(&g_unk_7101a22650[2]), field_0x8(0) {
        field_0x20 = 0;
        field_0x10 = 0;
        field_0x18 = 0;
    }
};

struct UnkClearedPair_l2f8414 {
    u64 a = 0;
    u64 b = 0;
};

// A value holder: its number, a reflected value and four pairs.
struct UnkAbilityValueHolder_l2f8414 {
    void** vtable;
    u64 field_0x8;
    u32 id;
    base::reflection::UnkReflValue value;
    UnkClearedPair_l2f8414 pairs[4];

    explicit UnkAbilityValueHolder_l2f8414(u32 id_) : vtable(&g_unk_7101a225a0[2]), field_0x8(0), id(id_) {
        value.field_0x0 = nullptr;
        value.type = nullptr;
        value.kind = 2;
        value.field_0x14 = 1;
    }
};
static_assert(sizeof(UnkAbilityValueHolder_l2f8414) == 0x70, "holder size");

struct UnkAbility_l2f8414 {
    void** vtable;
    u8 pad_0x8[0x8];
    UnkAbilityMember_l2f8414 member;  // 0x10
    u8 pad_0x38[0x7c];
    s32 state;  // 0xb4
    u16 field_0xb8;
    u8 pad_0xba[0x6];
    void* component;  // 0xc0
    void* def;        // 0xc8
    u64 field_0xd0;
    u32 field_0xd8;
    bool field_0xdc;
    bool field_0xdd;
    bool selectable;  // 0xde
    u8 pad_0xdf[0x5];
    u32 field_0xe4;
    UnkAbilityValueHolder_l2f8414 holder1;  // 0xe8
    UnkAbilityValueHolder_l2f8414 holder2;
    UnkAbilityValueHolder_l2f8414 holder3;
    UnkAbilityValueHolder_l2f8414 holder4;
    UnkAbilityValueHolder_l2f8414 holder5;
    UnkAbilityValueHolder_l2f8414 holder6;

    UnkAbility_l2f8414()
        : vtable(reinterpret_cast<void**>(&g_unk_7101a223f8[2])), state(7), field_0xb8(0), component(nullptr), def(nullptr), field_0xd0(0), field_0xd8(0),
          field_0xdc(true), field_0xdd(false), selectable(true), field_0xe4(0), holder1(1), holder2(2),
          holder3(3), holder4(4), holder5(5), holder6(6) {}
};
static_assert(__builtin_offsetof(UnkAbility_l2f8414, state) == 0xb4, "layout");
static_assert(__builtin_offsetof(UnkAbility_l2f8414, holder1) == 0xe8, "layout");

void fn_71002f8414(UnkGhostAuraBase_b0040* self) {
    fn_7101043904(self);
    new (self) UnkAbility_l2f8414;
}
