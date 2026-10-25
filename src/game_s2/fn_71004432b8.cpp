// Resets an effect's timing: when the flag at +0x13d is set it asks the owner's object (+0x220 of the
// object at +0x8) for a float with a fixed name hash and passes it on, otherwise -1.0; clears the
// four floats at +0x140 to (0, -1, -1, -1) and runs fn_710043fd04 with the id of the object at
// +0x98 (0x7fffffff when there is none) and the float.
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
    u8 pad_0x10[0x88];
    UnkIdObject* field_0x98;
    u8 pad_0xa0[0x9d];
    u8 field_0x13d;
    u8 pad_0x136[2];
    UnkVec4 field_0x140;
};

void fn_710043fd04(UnkEffect* self, u32 id, f32 time);

void fn_71004432b8(UnkEffect* self) {
    if (self->field_0x13d) {
        f32 time = fn_7101177ea0(self->field_0x8->field_0x220, 0xf4061ff25a2163d6);
        self->field_0x140.field_0x0 = 0.0f;
        self->field_0x140.field_0x4 = -1.0f;
        self->field_0x140.field_0x8 = -1.0f;
        self->field_0x140.field_0xc = -1.0f;
        if (self->field_0x98) {
            fn_710043fd04(self, self->field_0x98->field_0x10, time);
            return;
        }
        fn_710043fd04(self, 0x7fffffff, time);
    } else {
        self->field_0x140.field_0x0 = 0.0f;
        self->field_0x140.field_0x4 = -1.0f;
        self->field_0x140.field_0x8 = -1.0f;
        self->field_0x140.field_0xc = -1.0f;
        fn_710043fd04(self, self->field_0x98 ? self->field_0x98->field_0x10 : 0x7fffffff, -1.0f);
    }
}
