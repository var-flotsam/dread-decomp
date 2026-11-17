// sound::CSoundManager's unit (tools/tu_map.py): plays the audio preset a sound source names at a
// position, once the manager accepts it (fn_710102423c), and registers the playing sound (fn_71010244a8).
// Names and layouts are placeholders.
#include "base/UnkStrIdPathPair.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/snd/CSoundSystem.h"
#include "game/UnkTU101.h"
#include "game/UnkTU23.h"
#include "game/UnkTU2647.h"
#include "game/sound/CSoundManager.h"
#include "game/sound/UnkSoundDesc.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkSoundObject_b50;
struct UnkSoundRef {
    UnkSoundObject_b50* object;
    s64 serial;
};

struct UnkSoundDef;

// The audio preset a sound source refers to (sound::CAudioPreset).
struct UnkAudioPreset_l024b28 : base::reflection::UnkReflObject {
    u8 pad_0x8[0x56c];
    u32 field_0x574;
    u8 pad_0x578[0x30];
    f32 field_0x5a8;
    u8 pad_0x5ac[0x19];
    bool field_0x5c5;
};
static_assert(__builtin_offsetof(UnkAudioPreset_l024b28, field_0x5a8) == 0x5a8, "layout");

// A sound source: its name, its file and the preset it was resolved to.
struct UnkSoundSource_l024b28 {
    CStrId id;
    CFilePathStrId path;
    UnkAudioPreset_l024b28* preset;
};

struct UnkSoundManager_l024b28 {
    u8 pad_0x0[0x90];
    u32 field_0x90;
};

// The index to play with: the one given, else the manager's default.
inline u32 PlayIndex_l024b28(const UnkSoundManager_l024b28& manager, u32 index) {
    return index ? index : manager.field_0x90;
}

// The sound a handle names while it is still the one the handle was made for.
inline UnkSoundObject_b50* GetSoundObject_l024b28(const UnkSoundHandle* handle) {
    if (handle->ref && handle->serial != -1 && handle->ref->serial == handle->serial)
        return handle->ref->object;
    return nullptr;
}

// Starts the described sound; a positioned preset plays at `pos`, unless it is muted (field_0x5c5).
inline UnkSoundHandle Start_l024b28(const UnkAudioPreset_l024b28* preset, UnkInlineSoundDesc& sound,
                                    const f32* pos) {
    if (preset->field_0x574) {
        if (preset->field_0x5c5)
            return g_unk_7108b6c460;
        sound.position[0] = pos[0];
        sound.position[1] = pos[1];
        sound.position[2] = pos[2];
    }
    return fn_71001080b0(g_unk_7101d4d820, reinterpret_cast<const UnkSoundRequest_b50*>(&sound));
}

UnkSoundHandle fn_7101024b28(UnkDropTarget* self, void* record, void* source, void* a, u32 b) {
    const UnkSoundSource_l024b28* sound_source = static_cast<const UnkSoundSource_l024b28*>(record);
    const f32* pos = static_cast<const f32*>(source);
    UnkAudioPreset_l024b28* preset = nullptr;
    if (sound_source->id.data != &g_unk_7101d47108->empty &&
        sound_source->path.data != &g_unk_7101d47200->empty) {
        preset = sound_source->preset;
        if (preset && !fn_710009d50c(preset->vf_0x0(), fn_710102df18()))
            preset = nullptr;
    }
    if (!fn_710102423c(self, reinterpret_cast<UnkAudioPreset_b0038*>(preset)))
        return g_unk_7108b6c460;

    u32 index = PlayIndex_l024b28(*reinterpret_cast<UnkSoundManager_l024b28*>(self), b);
    UnkInlineSoundDesc sound;
    fn_7101037bc0(reinterpret_cast<UnkSoundDef*>(preset), &sound,
                  static_cast<f32*>(a), index);

    UnkSoundHandle handle;
    if (sound.path.data == &g_unk_7101d47200->empty && preset->field_0x5a8 > 0.0f) {
        handle = g_unk_7108b6c460;
    } else {
        handle = Start_l024b28(preset, sound, pos);
        if (GetSoundObject_l024b28(&handle))
            fn_71010244a8(self, handle);
    }
    return handle;
}
