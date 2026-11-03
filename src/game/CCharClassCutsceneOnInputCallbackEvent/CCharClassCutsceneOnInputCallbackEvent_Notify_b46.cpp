// CCharClassCutsceneOnInputCallbackEvent unit (tools/tu_map.py unit 2229): unless the GUI object
// (g_unk_7101d47330) answers fn_7101198688, counts a use of the event's target (+0x10, its +0x44)
// and passes the event's id (+0xc) with the target's +0x10 on to the GUI object (fn_7101199544).
// Names and layouts are placeholders.
#include "types.h"

extern void* g_unk_7101d47330;
bool fn_7101198688(void* gui);
void fn_7101199544(void* gui, u32 id, void* target);

struct UnkInputCallbackTarget_b46a3 {
    u8 pad_0x0[0x10];
    u8 field_0x10[0x34];
    // A union member: no type-based alias info, so the target is reloaded after the increment.
    union {
        u32 uses;  // 0x44
    };
};

struct UnkInputCallbackEvent_b46a3 {
    u8 pad_0x0[0xc];
    u32 id;                                // 0xc
    UnkInputCallbackTarget_b46a3* target;  // 0x10
};

void fn_7100cb1b84(UnkInputCallbackEvent_b46a3* self) {
    if (fn_7101198688(g_unk_7101d47330))
        return;
    UnkInputCallbackTarget_b46a3* target = self->target;
    void* gui = g_unk_7101d47330;
    u32 id = self->id;
    ++target->uses;
    fn_7101199544(gui, id, self->target->field_0x10);
}
