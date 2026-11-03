// CCharClassSetSlomoCustomEvent (unit 2290 of tools/tu_map.py): takes a reference on the shared slow-motion
// settings (+0x18, count at +0x44) and hands them (+0x10 of that object) with the event's two values (+0xc,
// +0x10) to the game manager's fn_7100df6d3c. Names and layouts are placeholders.
#include "game/UnkGameManager.h"
#include "types.h"

void fn_7100df6d3c(UnkGameManager* self, void* settings, f32 a, f32 b);

struct UnkSlomoSettings {
    u8 pad_0x0[0x10];
    u8 body[0x34];
    // A union member: no type-based alias info, so the settings pointer is reloaded after the increment.
    union {
        s32 ref_count;  // 0x44
    };
};

struct UnkSetSlomoCustomEvent {
    u8 pad_0x0[0xc];
    f32 a;
    f32 b;
    u8 pad_0x14[0x4];
    UnkSlomoSettings* settings;
};

void fn_7100cd8d78(UnkSetSlomoCustomEvent* self) {
    UnkGameManager* manager = g_unk_7101d47310;
    f32 a = self->a, b = self->b;
    ++self->settings->ref_count;
    fn_7100df6d3c(manager, self->settings->body, a, b);
}
