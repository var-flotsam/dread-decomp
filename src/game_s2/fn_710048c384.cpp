// Resets an effect's timing: when the flag at +0x135 is set it asks the owner's object (+0x220 of the
// object at +0x8) for a float with a fixed name hash and passes it on, otherwise -1.0; clears the
// four floats at +0x138 to (0, -1, -1, -1) and runs fn_7100488990 with the id of the object at
// +0x90 (0x7fffffff when there is none) and the float.
#include "game/UnkAnimationComponent.h"
#include "types.h"

using UnkAttack = UnkAnimationComponent;

struct UnkIdObject {
    u8 pad_0x0[0x10];
    u32 field_0x10;
};

struct UnkOwner {
    u8 pad_0x0[0x220];
    UnkAttack* field_0x220;
};

struct UnkVec4 {
    f32 field_0x0;
    f32 field_0x4;
    f32 field_0x8;
    f32 field_0xc;
};

struct UnkEffect {
    u8 pad_0x0[0x8];
    UnkOwner* field_0x8;
    u8 pad_0x10[0x80];
    UnkIdObject* field_0x90;
    u8 pad_0x98[0x9d];
    u8 field_0x135;
    u8 pad_0x136[2];
    UnkVec4 field_0x138;
};

void fn_7100488990(UnkEffect* self, u32 id, f32 time);

void fn_710048c384(UnkEffect* self) {
    if (self->field_0x135) {
        f32 time = fn_7101177ea0(self->field_0x8->field_0x220, 0xf4061ff25a2163d6);
        self->field_0x138.field_0x0 = 0.0f;
        self->field_0x138.field_0x4 = -1.0f;
        self->field_0x138.field_0x8 = -1.0f;
        self->field_0x138.field_0xc = -1.0f;
        if (self->field_0x90) {
            fn_7100488990(self, self->field_0x90->field_0x10, time);
            return;
        }
        fn_7100488990(self, 0x7fffffff, time);
    } else {
        self->field_0x138.field_0x0 = 0.0f;
        self->field_0x138.field_0x4 = -1.0f;
        self->field_0x138.field_0x8 = -1.0f;
        self->field_0x138.field_0xc = -1.0f;
        fn_7100488990(self, self->field_0x90 ? self->field_0x90->field_0x10 : 0x7fffffff, -1.0f);
    }
}
