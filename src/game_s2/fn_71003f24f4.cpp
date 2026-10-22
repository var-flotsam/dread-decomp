// CBigkranXSpitAttack: switches to the state `state` (kept at +0x34) when it differs, and sets
// up what that state needs: states 0 and 1 take a distance from the attack's owner object
// (+0x28, at +0xf0); state 2 takes a position from the sub-object that fn_7100d7c990 finds in
// the owner (+0x8) plus two values of the owner object; state 3 forwards a request (under two
// fixed 64-bit ids) to the sub-object at +0x220 of the owner.
#include "UnkSpitSend.h"

struct UnkVec3_71003f24f4 {
    f32 x;
    f32 y;
    f32 z;
};

struct UnkSpitTarget {
    u8 pad_0x0[0x44];
    f32 field_0x44;
    u8 pad_0x48[0xc];
    f32 field_0x54;
    u8 pad_0x58[0xc];
    f32 field_0x64;
};

struct UnkSpitOwnerObject {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkSpitConfig {
    u8 pad_0x0[0xec];
    f32 field_0xec;
    f32 field_0xf0;
};

struct UnkSpitStateObject {
    u8 pad_0x0[0x8];
    UnkSpitOwnerObject* field_0x8;
    u8 pad_0x10[0x18];
    UnkSpitConfig* field_0x28;
    u8 pad_0x30[4];
    s32 field_0x34;
    u8 pad_0x38[0xa4];
    f32 field_0xdc;
    f32 field_0xe0;
    f32 field_0xe4;
    f32 field_0xe8;
    UnkVec3_71003f24f4 field_0xec;
};

UnkSpitTarget* fn_7100d7c990(UnkSpitOwnerObject* owner);

void fn_71003f24f4(UnkSpitStateObject* self, u32 state) {
    if (self->field_0x34 == state)
        return;
    self->field_0x34 = state;
    switch (state) {
    case 0:
    case 1:
        self->field_0xe0 = self->field_0x28->field_0xf0;
        break;
    case 2:
        if (self->field_0x8) {
            if (UnkSpitTarget* target = fn_7100d7c990(self->field_0x8)) {
                if (UnkSpitConfig* config = self->field_0x28) {
                    self->field_0xdc = config->field_0xec;
                    self->field_0xe4 = 0;
                    UnkVec3_71003f24f4 position = {target->field_0x44, target->field_0x54, target->field_0x64};
                    self->field_0xec = position;
                }
            }
        }
        break;
    case 3:
        if (self->field_0x8) {
            if (void* target = self->field_0x8->field_0x220) {
                UnkSpitSend(target, 0xcd8fc2f707c5dbb3, 0xf4061ff25a2163d6);
            }
        }
        break;
    }
}
