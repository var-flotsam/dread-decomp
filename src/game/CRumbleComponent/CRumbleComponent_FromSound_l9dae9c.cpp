// CRumbleComponent's unit (tools/tu_map.py): builds a rumble's parameters from a sound definition.
// The definition is read into a sound description (fn_7101037bc0), whose
// file, kind and tunables then fill `out`. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/UnkTU2647.h"
#include "game/UnkTU49.h"
#include "game/sound/UnkSoundDesc.h"
#include "types.h"

using base::global::CFilePathStrId;

// What a rumble is played from (UnkRumbleParams of src/game/CRumbleComponent.cpp).
struct UnkRumbleDesc_l9dae9c {
    CFilePathStrId path;
    u64 field_0x8;
    u32 kind;        // 0x10
    f32 offset[3];   // 0x14
    f32 field_0x20[4];
    f32 field_0x30;
    f32 field_0x34;
    f32 field_0x38;
    f32 field_0x3c;
    bool field_0x40;
    bool field_0x41;
};

struct UnkSoundDef;

void CRumbleComponent_MakeRumbleDescFromSoundDef(void* self, UnkSoundDef* def, f32* scales, u32 index, UnkRumbleDesc_l9dae9c* out) {
    if (!def)
        return;
    UnkInlineSoundDesc sound;
    fn_7101037bc0(def, &sound, scales, index);
    out->path = sound.path;
    out->field_0x8 = 0;
    out->kind = fn_7100071680(sound.kind.field_0x0);
    out->offset[0] = sound.position[0];
    out->offset[1] = sound.position[1];
    out->offset[2] = sound.position[2];
    out->field_0x20[0] = sound.field_0x40;
    out->field_0x20[1] = sound.field_0x3c;
    out->field_0x20[2] = sound.field_0x5c > 0.0f ? sound.field_0x5c : sound.field_0x38;
    out->field_0x20[3] = sound.field_0x44;
    out->field_0x30 = sound.field_0x48 * 0.5f + 0.5f;
    out->field_0x34 = sound.field_0x4c;
    out->field_0x38 = sound.field_0x50;
    out->field_0x3c = 1.0f;
    out->field_0x40 = sound.field_0x6d;
    out->field_0x41 = sound.field_0x6c;
}
