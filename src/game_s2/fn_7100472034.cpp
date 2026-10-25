// Whether the owner's object at +0x220 (+0x30 of the component) is in state 4 on its object at +0x218
// and the integer fn_7101177da8 gives for it exceeds the float fn_7101177ea0 gives by more than 10.
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkOwner {
    u8 pad_0x0[0x218];
    void* field_0x218;
    UnkAnimationComponent* field_0x220;
};

struct UnkSelf {
    u8 pad_0x0[0x30];
    UnkOwner* field_0x30;
};

u32 fn_71010cc3a4(void* object);
s32 fn_7101177da8(void* object, u64 hash);

bool fn_7100472034(UnkSelf* self) {
    if (self->field_0x30->field_0x220 && self->field_0x30->field_0x218 &&
        fn_71010cc3a4(self->field_0x30->field_0x218) == 4) {
        f32 base = fn_7101177ea0(self->field_0x30->field_0x220, 0xf4061ff25a2163d6ull);
        f32 diff = (f32)fn_7101177da8(self->field_0x30->field_0x220, 0xf4061ff25a2163d6ull) - base;
        if (diff > 10.0f)
            return true;
    }
    return false;
}
