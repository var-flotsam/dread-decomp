// CAbilityEnergyWave (tools/tu_map.py): a start that passes the owner (+0x30) and the record at +0xe8 to
// fn_71002fd814, gives every child (+0x40, count +0x48) slot 0x40, then retargets the wave to itself as
// fn_71002fe160 does (src/game/CAbilityEnergyWave/CAbilityEnergyWave_Target_b49.cpp): the old target
// (+0xc0) drops the link (+0x78), the wave's own component takes it. Returns true. Names and layouts are
// placeholders.
#include "game/CInputComponent.h"
#include "types.h"

struct UnkEnergyWaveTargetComp_b49;

struct UnkWaveOwner_A2_b70 {
    u8 pad_0x0[0x1d8];
    UnkEnergyWaveTargetComp_b49* comp;
};

struct UnkWaveTarget_A2_b70 {
    u8 pad_0x0[0x30];
    UnkWaveOwner_A2_b70* owner;
};

struct UnkWaveChild_A2_b70 {
    void** vtable;
    void vf_0x40() { reinterpret_cast<void (*)(UnkWaveChild_A2_b70*)>(vtable[0x40 / 8])(this); }
};

struct UnkWave_A2_b70 {
    u8 pad_0x0[0x30];
    UnkWaveOwner_A2_b70* owner;
    u8 pad_0x38[0x8];
    UnkWaveChild_A2_b70** children;
    u32 count;
    u8 pad_0x4c[0x2c];
    u8 field_0x78[0x48];
    UnkWaveTarget_A2_b70* target;  // 0xc0
    u8 pad_0xc8[0x20];
    u8 field_0xe8[0x8];
};

void fn_71002fd814(UnkWaveOwner_A2_b70* owner, void* record);

bool fn_71002fd774(UnkWave_A2_b70* self) {
    fn_71002fd814(self->owner, self->field_0xe8);
    for (UnkWaveChild_A2_b70 **it = self->children, **end = it + self->count; it != end; ++it)
        (*it)->vf_0x40();
    void* link = self->field_0x78;
    if (self->target) {
        if (self->target->owner && self->target->owner->comp)
            fn_7100872908(self->target->owner->comp, link);
        self->target = nullptr;
    }
    UnkWaveTarget_A2_b70* target = reinterpret_cast<UnkWaveTarget_A2_b70*>(self);
    if (target->owner && target->owner->comp)
        fn_7100872860(target->owner->comp, link);
    self->target = target;
    return true;
}
