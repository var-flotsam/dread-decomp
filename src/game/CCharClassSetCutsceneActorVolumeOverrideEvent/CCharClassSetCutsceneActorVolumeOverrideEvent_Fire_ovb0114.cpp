// CCharClassSetCutsceneActorVolumeOverrideEvent's unit (tools/tu_map.py): the event's fire. The actor's
// id (+0x20, the id table's default at +0xd90 when empty) picks a bit (fn_7100106820 of its text), and
// the GUI (g_unk_7101d47330) gets the target (+0x10), that bit mask and the two volumes (+0x18, +0x1c)
// through fn_7101199ac4. Names and layouts are placeholders.
#include "base/UnkAtExit.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU0.h"
#include "game/UnkTU102.h"
#include "game/UnkTU2730.h"
#include "types.h"

using base::global::CStrId;

// A string id's data as this event reads it: its text and a use count.
struct UnkVolumeIdData_ovb0114 {
    u8 pad_0x0[0x10];
    const char* text;  // 0x10
    u8 pad_0x18[0x2c];
    union {
        s32 uses;  // 0x44 (a union member: no type-based alias info, so the id is read again after it)
    };
};

// The GUI's volume override for the actors in `mask`.
static inline void SetVolume(void* target, u32 mask, f32 a, f32 b) {
    void* gui = g_unk_7101d47330;
    alignas(8) u32 bits = mask;
    fn_7101199ac4(gui, target, &bits, a, b);
}

struct UnkVolumeOverrideEvent_ovb0114 {
    u8 pad_0x0[0x10];
    u8 target[0x8];  // 0x10
    f32 field_0x18;
    f32 field_0x1c;
    CStrId actor;  // 0x20
};

void fn_7100bd28d0(UnkVolumeOverrideEvent_ovb0114* self) {
    const CStrId* id = &self->actor;
    if (id->data == &g_unk_7101d47108->empty)
        id = &GameManagerIdTable().field_0xd90;
    ++reinterpret_cast<UnkVolumeIdData_ovb0114*>(id->data)->uses;
    const char* text = reinterpret_cast<UnkVolumeIdData_ovb0114*>(id->data)->text;
    u32 bit = fn_7100106820(text ? text : g_unk_710162e2ed);
    SetVolume(self->target, 1u << bit, self->field_0x18, self->field_0x1c);
}
