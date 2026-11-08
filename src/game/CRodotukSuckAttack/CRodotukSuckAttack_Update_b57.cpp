// The CRodotukSuckAttack unit's update: after the base update, in state 1 (+0x60): when the owner's (+0x8)
// AI component (+0x240) has its flag at +0x121e set, the attack's slot 0xe8 runs with dt; otherwise the
// owner's sub-object at +0x220 is sent 0x4f36c2305fee37d4 under the usual track kind with mode 0
// (UnkSpitSend, src/game_s2/UnkSpitSend.h) and slot 0xf0 runs with 3. Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game_s2/UnkSpitAttack.h"
#include "types.h"


struct UnkRodotukSuckAI_b57a4 {
    u8 pad_0x0[0x121e];
    bool field_0x121e;
};

struct UnkRodotukSuckOwner_b57a4 {
    u8 pad_0x0[0x220];
    void* field_0x220;
    u8 pad_0x228[0x18];
    UnkRodotukSuckAI_b57a4* field_0x240;
};

struct UnkRodotukSuckAttack_b57a4 {
    void** vtable;
    UnkRodotukSuckOwner_b57a4* owner;
    u8 pad_0x10[0x50];
    u32 field_0x60;

    void vf_0xe8(f32 dt) {
        reinterpret_cast<void (*)(UnkRodotukSuckAttack_b57a4*, f32)>(vtable[0xe8 / 8])(this, dt);
    }
    void vf_0xf0(u32 state) {
        reinterpret_cast<void (*)(UnkRodotukSuckAttack_b57a4*, u32)>(vtable[0xf0 / 8])(this, state);
    }
};

void fn_710066a9e0(UnkRodotukSuckAttack_b57a4* self, f32 dt) {
    fn_71003609a8(reinterpret_cast<UnkSpitAttack*>(self), dt);
    if (self->field_0x60 != 1)
        return;
    UnkRodotukSuckOwner_b57a4* owner = self->owner;
    if (owner->field_0x240->field_0x121e) {
        self->vf_0xe8(dt);
        return;
    }
    if (void* target = owner->field_0x220)
        UnkSpitSend(target, 0x4f36c2305fee37d4, 0xf4061ff25a2163d6);
    self->vf_0xf0(3);
}
