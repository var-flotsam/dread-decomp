// CBillboardComponent's unit (tools/tu_map.py): runs the billboard's callback on one of its
// entries and, when it answers 1 and the configuration names a sound, plays a random variant of
// it ("<name><n>.wav") at the given position. Names and layouts are placeholders.
#include <stdio.h>
#include <stdlib.h>

#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;
using base::global::UnkStrIdInline;

struct UnkEntityWithSound;
UnkSoundHandle fn_7100d7c2f0(UnkEntityWithSound* self, const u32* params, bool a, bool b, bool c);

// The sound description the entity plays, with its defaults (as in
// src/game/UnkTU811/UnkTU811_PlaySound_l552af0.cpp).
struct UnkBillboardSoundDesc_l728a34 {
    u32 kind;
    u32 field_0x4;
    u64 field_0x8;
    u32 field_0x10;
    u32 field_0x14;  // unset in the original
    CStrId name;            // 0x18
    CFilePathStrId path;    // 0x20
    f32 position[3];        // 0x28
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

    UnkBillboardSoundDesc_l728a34()
        : kind(0), field_0x4(0), field_0x8(0), field_0x10(0), name(g_unk_7101d470e0), path(g_unk_7101d470e8),
          position{0.0f, 0.0f, 0.0f}, field_0x34(-1.0f), field_0x38(1.0f), field_0x3c(500.0f), field_0x40(2000.0f),
          field_0x44(1.0f), field_0x48(0.0f), field_0x4c(0.0f), field_0x50(0.0f), field_0x54(-1.0f),
          field_0x58(-1.0f), field_0x5c(-1.0f), field_0x60(0.5f), field_0x64(5.0f), field_0x68(10.0f),
          field_0x6c(false), field_0x6d(false), field_0x6e(false) {}
};
static_assert(__builtin_offsetof(UnkBillboardSoundDesc_l728a34, field_0x6c) == 0x6c, "layout");

struct UnkBillboardConfig_l728a34 {
    u8 pad_0x0[0x150];
    CStrId sound;  // 0x150
    s32 sound_variants;  // 0x158
    f32 sound_distance[2];  // 0x15c
};

struct UnkBillboardEntry_l728a34 {
    u8 pad_0x0[0x24];
    bool field_0x24;
    u8 pad_0x25[0x148 - 0x25];
};

struct UnkBillboardSource_l728a34 {
    u8 pad_0x0[0x6c];
    f32 position[3];  // 0x6c
};

struct UnkBillboard_l728a34 {
    u8 pad_0x0[0x30];
    UnkEntityWithSound* entity;  // 0x30
    u8 pad_0x38[0x8];
    UnkBillboardConfig_l728a34* config;  // 0x40
    u8 pad_0x48[0x8];
    UnkBillboardEntry_l728a34* entries;  // 0x50
    u8 pad_0x58[0x30];
    void* context;  // 0x88
    s32 (*callback)(void* context, UnkBillboardEntry_l728a34* entry);  // 0x90
    u8 pad_0x98[0x78];
    s32 entry_count;  // 0x110
};

// Whether the entry at `index` is in use.
static inline bool IsActive(const UnkBillboard_l728a34* self, s32 index) {
    return index < self->entry_count && self->entries[index].field_0x24;
}

static inline void PlaySound(UnkBillboard_l728a34* self, const UnkBillboardSource_l728a34* source,
                             const char* prefix) {
    s32 variants = self->config->sound_variants;
    s32 variant = variants ? (rand() & 0x7fff) % variants : 0;
    char path[0x100];
    sprintf(path, "%s%d.wav", prefix, variant);

    static UnkBillboardSoundDesc_l728a34 s_desc;
    s_desc.kind = 0;
    s_desc.path = CFilePathStrId(UnkStrIdInline(), path, true);
    s_desc.field_0x6d = true;
    s_desc.position[0] = source->position[0];
    s_desc.position[1] = source->position[1];
    s_desc.position[2] = source->position[2];
    s_desc.field_0x38 = 1.0f;
    s_desc.field_0x3c = self->config->sound_distance[0];
    s_desc.field_0x40 = self->config->sound_distance[1];
    fn_7100d7c2f0(self->entity, reinterpret_cast<const u32*>(&s_desc), false, true, true);
}

s32 fn_7100728a34(UnkBillboard_l728a34* self, s32 index, const UnkBillboardSource_l728a34* source) {
    s32 result = 0;
    if (index < 0 || !self->callback)
        return result;
    if (IsActive(self, index))
        result = self->callback(self->context, &self->entries[index]);
    if (result != 1)
        return result;
    const CStrId& sound = self->config->sound;
    if (sound.data != &g_unk_7101d47108->empty)
        PlaySound(self, source, sound.Text());
    return result;
}
