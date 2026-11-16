// UnkTU811 (CEmmyAIComponent's unit): keeps the EMMI's three looping sounds (+0x294e8) in step with its
// state. When it is stopped (fn_71005436e8) or its owner's state is 2 they fade out; otherwise the first
// plays whenever it is not playing, and the other two play or fade out as the flag at +0x14c5a says.
// Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/snd/UnkSound.h"
#include "game/UnkTU811.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CStrId;
using base::global::CFilePathStrId;
using base::global::UnkStringData;

struct UnkEntityWithSound;
UnkSoundHandle fn_7100d7c374(UnkEntityWithSound* self, void* a, void* b, bool c, bool d, bool e);

using UnkPathMember_l5596c4 = base::UnkStrIdMemberOf<g_unk_7101d47200>;
__attribute__((visibility("hidden"))) extern UnkPathMember_l5596c4 g_unk_71726e2010;
__attribute__((visibility("hidden"))) extern UnkPathMember_l5596c4 g_unk_71726e2028;
__attribute__((visibility("hidden"))) extern UnkPathMember_l5596c4 g_unk_71726e2030;

// A string id copied from another (retained).
struct UnkSoundName_l5596c4 {
    UnkStringData* data;

    explicit UnkSoundName_l5596c4(const CStrId& id) : data(id.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
};

// A file path copied from another (retained).
struct UnkSoundPath_l5596c4 {
    UnkStringData* data;

    explicit UnkSoundPath_l5596c4(const CFilePathStrId& id) : data(id.data) {
        if (&g_unk_7101d47200->empty != data)
            ++data->ref_count;
    }
    void operator=(const UnkPathMember_l5596c4& other) { *reinterpret_cast<UnkPathMember_l5596c4*>(this) = other; }
};

// A sound description with its defaults; destroyed out of line.
struct UnkSoundDesc_l5596c4 {
    u32 kind;
    u32 field_0x4;
    u64 field_0x8;
    u32 field_0x10;
    u32 field_0x14;  // unset in the original
    UnkSoundName_l5596c4 name;  // 0x18
    UnkSoundPath_l5596c4 path;  // 0x20
    f32 position[3];            // 0x28
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

    UnkSoundDesc_l5596c4()
        : kind(0), field_0x4(0), field_0x8(0), field_0x10(0), name(g_unk_7101d470e0), path(g_unk_7101d470e8),
          position{0.0f, 0.0f, 0.0f}, field_0x34(-1.0f), field_0x38(1.0f), field_0x3c(500.0f), field_0x40(2000.0f),
          field_0x44(1.0f), field_0x48(0.0f), field_0x4c(0.0f), field_0x50(0.0f), field_0x54(-1.0f),
          field_0x58(-1.0f), field_0x5c(-1.0f), field_0x60(0.5f), field_0x64(5.0f), field_0x68(10.0f),
          field_0x6c(false), field_0x6d(false), field_0x6e(false) {}
    ~UnkSoundDesc_l5596c4() asm("fn_710034e0b8");
};
static_assert(__builtin_offsetof(UnkSoundDesc_l5596c4, field_0x6c) == 0x6c, "layout");

// The handle's view of the reference block.
struct UnkSoundRefView_l5596c4 {
    UnkSound* sound;
    s64 serial;
};

static inline UnkSound* Playing(const UnkSoundHandle& handle) {
    const UnkSoundRefView_l5596c4* ref = reinterpret_cast<const UnkSoundRefView_l5596c4*>(handle.ref);
    if (ref && ref->serial == handle.serial && handle.serial != -1)
        return ref->sound;
    return nullptr;
}

static inline bool IsPlaying(const UnkSoundHandle& handle) { return Playing(handle) != nullptr; }

// Fades the sound out if it is playing; says whether it was.
static inline bool FadeOut(const UnkSoundHandle& handle) {
    if (UnkSound* sound = Playing(handle)) {
        fn_7100105a80(sound, 0.2f, 0.0f);
        return true;
    }
    return false;
}

// The tunables the sounds' distances come from.
struct UnkEmmySoundParams_l5596c4 {
    u8 pad_0x0[0xb4];
    f32 min_distance;  // 0xb4
    f32 max_distance;  // 0xb8
    f32 volume;        // 0xbc
    f32 volume_2;      // 0xc0
};

struct UnkEmmySoundConfig_l5596c4;
struct UnkEmmySoundConfigVtable_l5596c4 {
    u8 pad_0x0[0xd8];
    const UnkEmmySoundParams_l5596c4* (*Params)(UnkEmmySoundConfig_l5596c4* self);  // 0xd8
};
struct UnkEmmySoundConfig_l5596c4 {
    UnkEmmySoundConfigVtable_l5596c4* vtable;

    const UnkEmmySoundParams_l5596c4* Params() { return vtable->Params(this); }
};

struct UnkEmmyState_l5596c4 {
    u8 pad_0x0[0x70];
    s32 state;  // 0x70
};

struct UnkEmmyEntity_l5596c4 {
    u8 pad_0x0[0x1b8];
    UnkEmmyState_l5596c4* state;  // 0x1b8
};

struct UnkEmmyAI_l5596c4 {
    u8 pad_0x0[0x30];
    UnkEmmyEntity_l5596c4* entity;  // 0x30
    u8 pad_0x38[0x14c58 - 0x38];
    bool field_0x14c58;
    bool field_0x14c59;
    bool loud;  // 0x14c5a
    u8 pad_0x14c5b[0x294e8 - 0x14c5b];
    UnkSoundHandle sounds[3];  // 0x294e8
    u8 pad_0x29518[0x29ee0 - 0x29518];
    UnkEmmySoundConfig_l5596c4* config;  // 0x29ee0
};
static_assert(__builtin_offsetof(UnkEmmyAI_l5596c4, config) == 0x29ee0, "layout");

static inline UnkSoundHandle Play(UnkEmmyAI_l5596c4* self, UnkSoundDesc_l5596c4* desc) {
    return fn_7100d7c374(reinterpret_cast<UnkEntityWithSound*>(self->entity), desc,
                         const_cast<CStrId*>(&g_unk_7101d470e0), false, true, true);
}

void fn_71005596c4(UnkEmmyAI_l5596c4* self) {
    self->field_0x14c58 = false;
    UnkEmmyEntity_l5596c4* entity;
    if (fn_71005436e8(reinterpret_cast<UnkEmmyAIComponent_b46a1*>(self)) ||
        ((entity = self->entity) && entity->state && entity->state->state == 2)) {
        FadeOut(self->sounds[0]);
        if (!FadeOut(self->sounds[1]))
            FadeOut(self->sounds[2]);
        return;
    }
    static UnkSoundDesc_l5596c4 s_sound asm("g_unk_71726e2390");
    s_sound.kind = 6;
    s_sound.field_0x6d = true;
    s_sound.field_0x3c = self->config->Params()->min_distance;
    s_sound.field_0x40 = self->config->Params()->max_distance;
    s_sound.field_0x44 = 1.0f;
    s_sound.field_0x6c = false;
    if (!IsPlaying(self->sounds[0])) {
        s_sound.path = g_unk_71726e2010;
        s_sound.field_0x38 = self->config->Params()->volume;
        self->sounds[0] = Play(self, &s_sound);
        self->field_0x14c59 = false;
    }
    if (self->loud) {
        if (!Playing(self->sounds[1])) {
            s_sound.path = g_unk_71726e2028;
            s_sound.field_0x38 = self->config->Params()->volume_2;
            self->sounds[1] = Play(self, &s_sound);
        }
    } else if (Playing(self->sounds[1])) {
        fn_7100105a80(Playing(self->sounds[1]), 0.2f, 0.0f);
    }
    if (self->loud) {
        if (!IsPlaying(self->sounds[2])) {
            s_sound.path = g_unk_71726e2030;
            s_sound.field_0x38 = self->config->Params()->volume_2;
            self->sounds[2] = Play(self, &s_sound);
        }
    } else if (IsPlaying(self->sounds[2])) {
        fn_7100105a80(Playing(self->sounds[2]), 0.2f, 0.0f);
    }
}
