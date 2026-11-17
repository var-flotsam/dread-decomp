// CAbility's unit (tools/tu_map.py unit 427): CAbility's constructor: the CGameObject base, the member at
// +0x10, the state and flags, six value holders numbered 1 to 6, then virtual slot 3 (0x18) on itself.
// Names and layouts are placeholders.
#include "base/reflection/UnkReflValue.h"
#include "game/CAbility.h"
#include "game/CGameObject.h"
#include "types.h"

extern void* g_unk_7101a225a0[];

struct UnkAbilityMember_l2fa8b8 {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
    u32 field_0x20;
};

struct UnkClearedPair_l2fa8b8 {
    u64 a, b;

    void Clear() {
        a = 0;
        b = 0;
    }
};

// A value holder: its number, a reflected value and four pairs.
struct UnkAbilityValueHolder_l2fa8b8 {
    void** vtable;
    u64 field_0x8;
    s32 id;  // 0x10
    base::reflection::UnkReflValue value;  // 0x18
    UnkClearedPair_l2fa8b8 pairs[4];       // 0x30

    void Init(s32 n) {
        vtable = &g_unk_7101a225a0[2];
        field_0x8 = 0;
        id = n;
        value.field_0x0 = nullptr;
        value.type = nullptr;
        value.kind = 2;
        value.field_0x14 = 1;
        for (UnkClearedPair_l2fa8b8& pair : pairs)
            pair.Clear();
    }
};
static_assert(sizeof(UnkAbilityValueHolder_l2fa8b8) == 0x70, "holder size");

struct UnkAbility_l2fa8b8 {
    void** vtable;
    u64 field_0x8;
    UnkAbilityMember_l2fa8b8 member;  // 0x10
    u8 pad_0x38[0x7c];
    s32 state;  // 0xb4
    bool field_0xb8;
    bool field_0xb9;
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
    UnkAbilityValueHolder_l2fa8b8 holders[6];  // 0xe8
    u64 field_0x388;
};
static_assert(__builtin_offsetof(UnkAbility_l2fa8b8, holders) == 0xe8, "layout");
static_assert(sizeof(UnkAbility_l2fa8b8) == 0x390, "layout");

void fn_71002fa8b8(UnkAbility_l2fa8b8* self) {
    fn_7101043904(self);
    self->vtable = &g_unk_7101a223f8[2];
    self->member.vtable = &g_unk_7101a22650[2];
    self->member.field_0x8 = 0;
    self->member.field_0x10 = 0;
    self->member.field_0x18 = 0;
    self->member.field_0x20 = 0;
    self->state = 7;
    self->field_0xb8 = false;
    self->field_0xb9 = false;
    self->component = nullptr;
    self->def = nullptr;
    self->field_0xd0 = 0;
    self->field_0xd8 = 0;
    self->field_0xdc = true;
    self->field_0xdd = false;
    self->selectable = true;
    self->field_0xe4 = 0;
    s32 id = 0;
    for (UnkAbilityValueHolder_l2fa8b8& holder : self->holders)
        holder.Init(++id);

    self->field_0x388 = 0;
    reinterpret_cast<void (*)(UnkAbility_l2fa8b8*)>(self->vtable[3])(self);
}
