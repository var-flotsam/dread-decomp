// In the CSpittailMagmaBallMovementComponent unit: an override that runs its base (fn_71006dfa84) and,
// when that succeeds and the flag at +0x5d8 is clear, passes the fourth argument on to
// fn_71006ca5d4. Returns the base result. Names and layouts are placeholders.
#include "types.h"

struct UnkSpittailMagmaBallMovement {
    u8 pad_0x0[0x5d8];
    bool field_0x5d8;
};

bool fn_71006dfa84(UnkSpittailMagmaBallMovement* self, void* a, void* b, void* c);
void fn_71006ca5d4(UnkSpittailMagmaBallMovement* self, void* c);

bool fn_71006ca588(UnkSpittailMagmaBallMovement* self, void* a, void* b, void* c) {
    bool ok = fn_71006dfa84(self, a, b, c);
    if (ok && !self->field_0x5d8)
        fn_71006ca5d4(self, c);
    return ok;
}
