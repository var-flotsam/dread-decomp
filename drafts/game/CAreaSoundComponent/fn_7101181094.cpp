// CAreaSoundComponent's unit (tools/tu_map.py unit 2721): after the base step, unless the owner is
// disabled (+0x1b5) or the component is busy (+0x9b8), plays the component's sound path: a path under
// "streams/" as a stream (group 5, fn_7100108620), anything else as a sound (kind 5, fn_71001080b0),
// placed at `position` when the component is positional; then fades out and forgets the previous
// sound. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/global/UnkStringPoolBody.h"
#include "../../../src/game_s7/UnkPropBagS7.h"
#include "base/math/CVector.h"
#include "base/snd/CSoundSystem.h"
#include "base/snd/UnkSound.h"
#include "game/UnkTU101.h"
#include "game/UnkTU23.h"
#include "game/sound/UnkSoundDesc.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStringEntry;

// As src/game/CAreaMusicComponent/CAreaMusicComponent_b52.cpp declares it.
void fn_7100718388(void* self, void* arg);

// As src/game/CAreaSoundComponent/CAreaSoundComponent_Paths_ovb0062.cpp defines it.
struct UnkSoundRef {
    UnkSound* object;
    s64 serial;
};

// As src/game/CCharClassPlayEnvironmentStreamEvent/CCharClassPlayEnvironmentStreamEvent_Play_ovb0138.cpp
// defines it.
struct UnkPlayingRef_ovb0074;
struct UnkPlayingHandle_ovb0074 {
    UnkPlayingRef_ovb0074* ref;
    u64 serial;
};

// The text of a string id (its use count at +0x44 goes up first).
static inline const CRntString* Text(base::global::UnkStringData* data) {
    UnkStringEntry* entry = reinterpret_cast<UnkStringEntry*>(data);
    ++entry->tail.field_0x44;
    return &entry->buffer;
}

// The text of a file path; its use count goes up first (as a shared name's, game_s7/UnkPropBagS7.h).
static inline const CRntString* PathText(const CFilePathStrId& path) {
    ++reinterpret_cast<unk_prop_bag::UnkName*>(path.data)->use_count;
    return &reinterpret_cast<UnkStringEntry*>(path.data)->buffer;
}

// The file path of a string id's text.
struct UnkAreaStreamPath_l181094 {
    base::global::UnkStringData* data;

    explicit UnkAreaStreamPath_l181094(const CStrId& id) {
        fn_710001a270(reinterpret_cast<base::global::UnkFilePathStrIdBase*>(this), *Text(id.data), true);
    }
    void operator=(const CFilePathStrId& other) { *reinterpret_cast<CFilePathStrId*>(this) = other; }
};

// A string id copied from another (retained).
struct UnkAreaStreamName_l181094 {
    base::global::UnkStringData* data;

    explicit UnkAreaStreamName_l181094(const CStrId& id) : data(id.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
};

// What the sound system plays as a stream (the shape of UnkSoundDesc_ovb0074).
struct UnkAreaStreamDesc_l181094 {
    s32 group;
    UnkAreaStreamPath_l181094 path;  // 0x8
    UnkAreaStreamName_l181094 name;  // 0x10
    s32 direction;                   // 0x18
    u32 field_0x1c;
    u32 field_0x20;
    u32 variant;  // 0x24
    u32 field_0x28;
    f32 volume;  // 0x2c
    f32 field_0x30;
    f32 field_0x34;
    f32 field_0x38;

    UnkAreaStreamDesc_l181094()
        : group(0), path(g_unk_7101d470e0), name(g_unk_7101d470e0), direction(1), field_0x1c(0),
          field_0x20(0xffffffff), variant(0), volume(1.0f), field_0x30(0.0f), field_0x34(0.0f),
          field_0x38(-1.0f) {}
    ~UnkAreaStreamDesc_l181094();  // fn_7100d8ef0c
};

struct UnkAreaSoundOwner_l181094 {
    u8 pad_0x0[0x1b5];
    u8 disabled;  // 0x1b5
};

struct UnkAreaSound_l181094 {
    u8 pad_0x0[0x30];
    UnkAreaSoundOwner_l181094* owner;  // 0x30
    u8 pad_0x38[0x9b8 - 0x38];
    u32 field_0x9b8;
    u8 pad_0x9bc[0xad0 - 0x9bc];
    void* listener;              // 0xad0
    UnkSoundHandle handles[3];   // 0xad8
    CFilePathStrId paths[3];     // 0xb08
    u32 field_0xb20;
    u32 field_0xb24;
    u32 field_0xb28;
    u32 field_0xb2c;
    u32 mode;  // 0xb30: 1 when the sound plays where the component is
    u8 pad_0xb34[0xb54 - 0xb34];
    f32 fade;  // 0xb54
    f32 volume;  // 0xb58
    f32 field_0xb5c;
    f32 field_0xb60;
    f32 field_0xb64;
};

inline UnkSound* GetSound_l181094(const UnkSoundHandle* handle) {
    if (handle->ref && handle->ref->serial == handle->serial && handle->serial != -1)
        return handle->ref->object;
    return nullptr;
}

void fn_7101181094(UnkAreaSound_l181094* self, void* arg, const base::math::CVector3D* position) {
    fn_7100718388(self, arg);
    if (self->owner->disabled || self->field_0x9b8)
        return;

    if (self->paths[2].data != &g_unk_7101d47200->empty) {
        if (CRntString_Contains(PathText(self->paths[2]), "streams/")) {
            static UnkAreaStreamDesc_l181094 s_stream;
            s_stream.group = 5;
            s_stream.path = self->paths[2];
            s_stream.volume = self->volume;
            s_stream.field_0x30 = self->field_0xb60;
            s_stream.field_0x34 = self->field_0xb64;
            UnkPlayingHandle_ovb0074 playing =
                CSoundSystem_PlayOrRestartSound(reinterpret_cast<UnkSoundSystemPlayView_ovb0074*>(g_unk_7101d4d820),
                              reinterpret_cast<UnkSoundDesc_ovb0074*>(&s_stream));
            self->handles[2].ref = reinterpret_cast<UnkSoundRef*>(playing.ref);
            self->handles[2].serial = playing.serial;
        } else {
            static UnkSoundDesc s_sound;
            s_sound.kind.field_0x0 = 5;
            s_sound.path = self->paths[2];
            if (position && self->mode == 1) {
                s_sound.field_0x6d = true;
                s_sound.field_0x3c = 500.0f;
                s_sound.field_0x40 = 2000.0f;
                s_sound.position[0] = position->x;
                s_sound.position[1] = position->y;
                s_sound.position[2] = position->z;
            }
            s_sound.field_0x38 = self->volume;
            s_sound.field_0x44 = self->field_0xb5c;
            s_sound.field_0x4c = self->field_0xb60;
            s_sound.field_0x50 = self->field_0xb64;
            self->handles[2] = fn_71001080b0(g_unk_7101d4d820, reinterpret_cast<const UnkSoundRequest_b50*>(&s_sound));
        }
        if (UnkSound* sound = GetSound_l181094(&self->handles[2]))
            fn_7100106024(reinterpret_cast<UnkSoundObject_b50*>(sound), &self->listener);
    }

    if (UnkSound* sound = GetSound_l181094(&self->handles[1])) {
        fn_7100105a80(sound, self->fade, 0.0f);
        self->handles[1] = g_unk_7108b6c460;
    }
}
