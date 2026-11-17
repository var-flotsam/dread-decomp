// CPickableComponent's unit (tools/tu_map.py unit 1492): starts the pickable's ammo loop sound unless
// it is already playing. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/UnkTU2523.h"
#include "game/sound/UnkSoundDesc.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;


// The handle's view of the reference block.
struct UnkSoundRefView_l9cd654 {
    void* sound;
    s64 serial;
};

static inline bool Playing(const UnkSoundHandle& handle) {
    const UnkSoundRefView_l9cd654* ref = reinterpret_cast<const UnkSoundRefView_l9cd654*>(handle.ref);
    return ref && ref->serial == handle.serial && handle.serial != -1 && ref->sound;
}

struct UnkPickableComponent_l9cd654 {
    u8 pad_0x0[0x30];
    UnkEntityWithSound* entity;  // 0x30
    u8 pad_0x38[0x28];
    UnkSoundHandle loop_sound;  // 0x60
};

void fn_71009cd654(UnkPickableComponent_l9cd654* self) {
    if (Playing(self->loop_sound))
        return;
    static CFilePathStrId s_path("generic/pickableammoloop.wav", true);
    static UnkSoundDesc s_desc;
    s_desc.kind.field_0x0 = 0;
    s_desc.path = s_path;
    s_desc.field_0x38 = 1.0f;
    s_desc.field_0x3c = 500.0f;
    s_desc.field_0x40 = 2000.0f;
    s_desc.field_0x44 = 1.0f;
    s_desc.field_0x6c = true;
    s_desc.field_0x6d = true;
    s_desc.field_0x4c = 1.0f;
    self->loop_sound = fn_7100d7c374(self->entity, &s_desc, &g_unk_7101d470e0, false, true, true);
}
