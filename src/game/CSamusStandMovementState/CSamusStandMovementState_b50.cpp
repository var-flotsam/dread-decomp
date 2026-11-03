// CSamusStandMovementState unit (tools/tu_map.py, unit 1470): fn_71009b5460 picks the id (a hash)
// of the animation to play from the movement (+0x38): with a positive speed (+0x7c) the moving one,
// unless the settings' +0x79 allows the alternative and fn_71009ea3e4 holds while fn_71009ea664
// doesn't; standing, one of two by the flag at +0x7e4. Always true. Names and layouts are placeholders.
#include "types.h"

// The settings object of src/game/CSamusMovement.cpp (reached through the GOT); +0x79 is read here.
struct UnkSamusSettings {
    u8 pad_0x0[0x78];
    bool field_0x78;
};
extern UnkSamusSettings g_unk_7172740de0;

struct UnkSamusMove_b50a5 {
    u8 unk_0x0[0x7c];
    f32 speed;  // 0x7c
    u8 unk_0x80[0x764];
    bool field_0x7e4;
};

bool fn_71009ea3e4(UnkSamusMove_b50a5* self, f32 speed);
bool fn_71009ea664(UnkSamusMove_b50a5* self);

struct UnkSamusStandState_b50a5 {
    u8 unk_0x0[0x38];
    UnkSamusMove_b50a5* movement;
};

// The id is written through a may_alias word: the original reloads the movement after the store.
typedef u64 __attribute__((may_alias)) UnkHashWord_b50a5;

bool fn_71009b5460(UnkSamusStandState_b50a5* self, UnkHashWord_b50a5* id) {
    f32 speed = self->movement->speed;
    if (speed > 0.0f) {
        *id = 0xbca4fe615b9507aeull;
        if (!reinterpret_cast<const bool*>(&g_unk_7172740de0)[0x79] || !fn_71009ea3e4(self->movement, speed) ||
            fn_71009ea664(self->movement))
            return true;
    } else if (self->movement->field_0x7e4) {
        *id = 0x861f06fd9f7439ffull;
        return true;
    }
    *id = 0x2cd1f90b8f722a39ull;
    return true;
}
