// Refreshes a list of child items: counts the items of kind 6 or 5 (looking through a
// kind-7 wrapper to its target), then, if any were found or `all` is set, sets a flag on every
// item (or only on those that accept it) that is true when fewer than two were found and
// `force_off` is clear; with two or more, also runs a follow-up. Finally updates two attached
// components of the owner.
#include "game/CAbilityEnergyWave.h"
#include "game/CAbilityLayout.h"
#include "types.h"

struct UnkComponentA_71002fda50;
struct UnkComponentB_71002fda50;
void fn_710113f2f4(UnkComponentA_71002fda50* self);
void fn_710094f974(UnkComponentB_71002fda50* self);

struct UnkOwner_71002fda50 {
    u8 unk_0x0[0x210];
    UnkComponentB_71002fda50* field_0x210;
    u8 unk_0x218[0x48];
    UnkComponentA_71002fda50* field_0x260;
};

void fn_71003014e0(CAbilityComponent* self, void* arg);

void fn_71002fda50(CAbilityComponent* self, bool all, bool force_off) {
    CAbility** begin = self->abilities.data;
    CAbility** end = begin + self->abilities.count;
    u32 found = 0;
    for (CAbility** it = begin; it != end; ++it) {
        if ((*it)->State() == 6 || (*it)->State() == 5)
            ++found;
    }
    if (found != 0 || all) {
        bool single = found < 2;
        bool value = single && !force_off;
        for (CAbility** it = begin; it != end; ++it) {
            CAbility* ability = *it;
            if (all || ability->Virtual<bool>(0xb0))
                ability->Virtual(0xa0, value);
        }
        if (!single)
            fn_71003014e0(self, nullptr);
    }
    if (UnkComponentA_71002fda50* a = self->Owner<UnkOwner_71002fda50>()->field_0x260)
        fn_710113f2f4(a);
    if (UnkComponentB_71002fda50* b = self->Owner<UnkOwner_71002fda50>()->field_0x210)
        fn_710094f974(b);
}
