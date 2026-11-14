// CAbilityEnergyWave (tools/tu_map.py): a start that passes the owner (+0x30) and the record at +0xe8 to
// fn_71002fd814, gives every child (+0x40, count +0x48) slot 0x40, then retargets the wave to itself as
// fn_71002fe160 does (src/game/CAbilityEnergyWave/CAbilityEnergyWave_Target_b49.cpp): the old target
// (+0xc0) drops the link (+0x78), the wave's own component takes it. Returns true. Names and layouts are
// placeholders.
#include "game/CInputComponent.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CAbilityLayout.h"
#include "types.h"

struct UnkEnergyWaveTargetComp_b49;

struct UnkWaveOwner_A2_b70 {
    u8 pad_0x0[0x1d8];
    UnkEnergyWaveTargetComp_b49* comp;
};

// The input component of a component's actor (+0x1d8), which holds the listener.
static inline UnkEnergyWaveTargetComp_b49* InputOf(UnkBaseComponent* component) {
    return OwnerOf<UnkWaveOwner_A2_b70>(component)->comp;
}


bool fn_71002fd774(CAbilityComponent* self) {
    fn_71002fd814(self->Owner<UnkWaveOwner_A2_b70>(), &self->life);
    for (CAbility **it = self->begin(), **end = self->end(); it != end; ++it)
        (*it)->Virtual(0x40);
    void* link = &self->input;
    if (self->target) {
        if (self->target->field_0x30 && InputOf(self->target))
            fn_7100872908(InputOf(self->target), link);
        self->target = nullptr;
    }
    UnkBaseComponent* target = self;
    if (target->field_0x30 && InputOf(target))
        fn_7100872860(InputOf(target), link);
    self->target = target;
    return true;
}
