// Unit 2689 (CPlayerMovement hint): true when the object is not blocked by its virtual at +0x350 with a
// nonzero value at +0x1dc, and none of the game manager's checks fn_7100df5874 or fn_7100df5a4c hold and
// fn_7100df5630 is false. Names and layouts are placeholders.
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "types.h"

struct UnkMovement_l0d94f4 {
    void** vtable;
    u8 pad_0x8[0x1dc - 0x8];
    f32 field_0x1dc;

    bool vf_0x350() { return reinterpret_cast<bool (*)(UnkMovement_l0d94f4*)>(vtable[0x350 / 8])(this); }
};

bool fn_71010d94f4(UnkMovement_l0d94f4* self) {
    if (!self->vf_0x350() || self->field_0x1dc == 0.0f) {
        if (!fn_7100df5874(reinterpret_cast<UnkPlayer*>(g_unk_7101d47310)) && !fn_7100df5a4c(g_unk_7101d47310))
            return !fn_7100df5630(g_unk_7101d47310);
    }
    return false;
}
