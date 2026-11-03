#include "types.h"

struct UnkSamusMovement;
void fn_71009e0160(UnkSamusMovement* self, bool flag, f32 dt);
void fn_71010d1c9c();

// Runs fn_71010d1c9c, then the legs' override update with the flag cleared. The original passes
// the time before the flag, as if through a (self, f32, bool) declaration.
void fn_71009e2bfc(UnkSamusMovement* self, f32 dt) {
    fn_71010d1c9c();
    reinterpret_cast<void (*)(UnkSamusMovement*, f32, bool)>(&fn_71009e0160)(self, dt, false);
}

struct UnkSamusMovementTuning {
    u8 unk_0x0[0xc4];
    u32 field_0xc4;
};

struct UnkSamusMovement2 {
    u8 unk_0x0[0x2290];
    u32 field_0x2290;
    u8 unk_0x2294[0x4];
    f32 field_0x2298;
    u8 unk_0x229c[0x5458 - 0x229c];
    UnkSamusMovementTuning* tuning;  // 0x5458
};

bool fn_71010d7a28(UnkSamusMovement2* self);
void fn_71009e0754(UnkSamusMovement2* self);

// When fn_71010d7a28 accepts, runs fn_71009e0754 and reloads 0x2290 from the tuning with 0x2298 unset.
bool fn_71009e070c(UnkSamusMovement2* self) {
    bool ok = fn_71010d7a28(self);
    if (ok) {
        fn_71009e0754(self);
        self->field_0x2290 = self->tuning->field_0xc4;
        self->field_0x2298 = -1.0f;
    }
    return ok;
}
