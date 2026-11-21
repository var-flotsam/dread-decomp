// CAbility's unit (tools/tu_map.py unit 427): CAbility's constructor: the base fn_7101043904, the member at
// +0x10, the state and flags, and six value holders numbered 1 to 6. Names and layouts are placeholders.
#include "game/CAbility.h"
#include "game/CGameObject.h"
#include "types.h"

struct UnkGhostAuraBase_b0040;

struct UnkAbilityMember_l2f8414 {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
    u32 field_0x20;
};

struct UnkAbilityDelegate_l2f8414 {
    void* object;
    void (*function)();
};

// A value holder: its number, a value with its type, and three delegates.
struct UnkAbilityValueHolder_l2f8414 {
    void** vtable;
    void* owner;
    u32 id;
    u64 field_0x18;
    u64 field_0x20;
    u32 field_0x28;
    u32 field_0x2c;
    UnkAbilityDelegate_l2f8414 field_0x30;
    UnkAbilityDelegate_l2f8414 field_0x40;
    UnkAbilityDelegate_l2f8414 field_0x50;
    u64 field_0x60;
    u64 field_0x68;
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
    UnkAbilityValueHolder_l2f8414 holders[6];  // 0xe8
    u64 field_0x388;
};
static_assert(__builtin_offsetof(UnkAbility_l2f8414, state) == 0xb4, "layout");
static_assert(__builtin_offsetof(UnkAbility_l2f8414, holders) == 0xe8, "layout");

static inline void InitHolder(UnkAbilityValueHolder_l2f8414* holder, u32 id) {
    holder->owner = nullptr;
    holder->id = id;
    holder->field_0x18 = 0;
    holder->field_0x20 = 0;
    holder->field_0x28 = 2;
    holder->field_0x2c = 1;
    holder->vtable = g_unk_7101a225a0 + 2;
    holder->field_0x30 = {nullptr, nullptr};
    holder->field_0x40 = {nullptr, nullptr};
    holder->field_0x50 = {nullptr, nullptr};
    holder->field_0x60 = 0;
    holder->field_0x68 = 0;
}

void fn_71002f8414(UnkGhostAuraBase_b0040* object) {
    fn_7101043904(object);
    UnkAbility_l2f8414* self = reinterpret_cast<UnkAbility_l2f8414*>(object);
    self->vtable = g_unk_7101a223f8 + 2;

    UnkAbilityMember_l2f8414& member = self->member;
    member.field_0x20 = 0;
    member.field_0x10 = 0;
    member.field_0x18 = 0;
    member.vtable = g_unk_7101a22650 + 2;
    member.field_0x8 = 0;

    self->state = 7;
    self->field_0xb8 = 0;
    self->component = nullptr;
    self->def = nullptr;
    self->field_0xd0 = 0;
    self->field_0xd8 = 0;
    self->field_0xdc = true;
    self->field_0xdd = false;
    self->selectable = true;

    self->field_0xe4 = 0;
    InitHolder(&self->holders[0], 1);
    InitHolder(&self->holders[1], 2);
    InitHolder(&self->holders[2], 3);
    InitHolder(&self->holders[3], 4);
    InitHolder(&self->holders[4], 5);
    InitHolder(&self->holders[5], 6);
    self->field_0x388 = 0;
}
