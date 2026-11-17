// CPickableComponent's unit (tools/tu_map.py unit 1492): starts the pickable's ammo loop sound unless
// it is already playing. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkEntityWithSound;
UnkSoundHandle fn_7100d7c374(UnkEntityWithSound* self, void* a, void* b, bool c, bool d, bool e);

// The sound description the entity plays, with its defaults (as fn_7100552af0's).
struct UnkSoundDesc_l9cd654 {
    u32 kind;
    u32 field_0x4;
    u64 field_0x8;
    u32 field_0x10;
    u32 field_0x14;  // unset in the original
    CStrId name;          // 0x18
    CFilePathStrId path;  // 0x20
    f32 position[3];      // 0x28
    f32 field_0x34;
    f32 field_0x38;
    f32 field_0x3c;
    f32 field_0x40;
    f32 field_0x44;
    f32 field_0x48;
    f32 field_0x4c;
    f32 field_0x50;
    f32 field_0x54;
    f32 field_0x58;
    f32 field_0x5c;
    f32 field_0x60;
    f32 field_0x64;
    f32 field_0x68;
    bool field_0x6c;
    bool field_0x6d;
    bool field_0x6e;

    UnkSoundDesc_l9cd654()
        : kind(0), field_0x4(0), field_0x8(0), field_0x10(0), name(g_unk_7101d470e0), path(g_unk_7101d470e8),
          position{0.0f, 0.0f, 0.0f}, field_0x34(-1.0f), field_0x38(1.0f), field_0x3c(500.0f), field_0x40(2000.0f),
          field_0x44(1.0f), field_0x48(0.0f), field_0x4c(0.0f), field_0x50(0.0f), field_0x54(-1.0f),
          field_0x58(-1.0f), field_0x5c(-1.0f), field_0x60(0.5f), field_0x64(5.0f), field_0x68(10.0f),
          field_0x6c(false), field_0x6d(false), field_0x6e(false) {}
    ~UnkSoundDesc_l9cd654() asm("fn_710034e0b8");
};
static_assert(__builtin_offsetof(UnkSoundDesc_l9cd654, field_0x6c) == 0x6c, "layout");

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
    static UnkSoundDesc_l9cd654 s_desc;
    s_desc.kind = 0;
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
