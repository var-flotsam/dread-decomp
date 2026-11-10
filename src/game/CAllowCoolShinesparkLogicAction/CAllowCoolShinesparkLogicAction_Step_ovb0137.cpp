// CAllowCoolShinesparkLogicAction's unit (tools/tu_map.py): the action's step, handing the player's CAbilityShinespark the action's two lists as bit masks.
// Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkTutoExitAction_b47;
void fn_7100a55ac8(UnkTutoExitAction_b47* self);
u64 fn_7100327d14();
struct UnkAbilitySet;
struct UnkAbility;
struct UnkCoolShinesparkMask_ovb0137;
UnkAbility* fn_71003012f8(UnkAbilitySet* set, u64 id);
void fn_71003312a8(UnkReflObject* ability, const UnkCoolShinesparkMask_ovb0137* allowed_a, const UnkCoolShinesparkMask_ovb0137* allowed_b, void* arg, bool flag);
void fn_710032a3b4(UnkReflObject* ability, const UnkCoolShinesparkMask_ovb0137* allowed_a, const UnkCoolShinesparkMask_ovb0137* allowed_b, bool flag);

// A flag set as the ability takes it: the bits, 8-byte aligned.
struct alignas(8) UnkCoolShinesparkMask_ovb0137 {
    u32 bits;
    u8 pad_0x4[0x4];
};

struct UnkCoolShinesparkPlayer_ovb0137 {
    u8 pad_0x0[0x208];
    void* field_0x208;
    void* field_0x210;
    u8 pad_0x218[0xc8];
    UnkAbilitySet* abilities;  // 0x2e0
};

struct UnkCoolShinesparkAction_ovb0137 {
    u8 pad_0x0[0x10];
    u32* list_a;  // 0x10
    u32 count_a;  // 0x18
    u8 pad_0x1c[0x1c];
    u32* list_b;  // 0x38
    u32 count_b;  // 0x40
    u8 pad_0x44[0x1c];
    u8 field_0x60[8];
    bool field_0x68;
    bool field_0x69;
};

// After the base step, hands the player's shinespark ability the two lists as bit masks.
void fn_7100a333e0(UnkCoolShinesparkAction_ovb0137* self, UnkCoolShinesparkPlayer_ovb0137* player) {
    fn_7100a55ac8(reinterpret_cast<UnkTutoExitAction_b47*>(self));
    if (!player->field_0x208 && !player->field_0x210)
        return;
    UnkAbilitySet* abilities = player->abilities;
    if (!abilities)
        return;
    UnkReflObject* ability = reinterpret_cast<UnkReflObject*>(fn_71003012f8(abilities, fn_7100327d14()));
    if (!ability || !fn_710009d50c(ability->vf_0x0(), fn_71003255a4()))
        return;
    UnkCoolShinesparkMask_ovb0137 mask_a;
    mask_a.bits = 0;
    for (u32* it = self->list_a; it != self->list_a + self->count_a; ++it)
        mask_a.bits |= 1u << *it;
    UnkCoolShinesparkMask_ovb0137 mask_b;
    mask_b.bits = 0;
    for (u32* it = self->list_b; it != self->list_b + self->count_b; ++it)
        mask_b.bits |= 1u << *it;
    if (self->field_0x68)
        fn_71003312a8(ability, &mask_a, &mask_b, self->field_0x60, self->field_0x69);
    else
        fn_710032a3b4(ability, &mask_a, &mask_b, self->field_0x69);
}
