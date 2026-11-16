// sound::CSoundManager's unit (tools/tu_map.py): plays the audio preset a sound source names at a
// position, once the manager accepts it (fn_710102423c), and registers the playing sound (fn_71010244a8).
// Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkStrIdPathPair.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/snd/CSoundSystem.h"
#include "game/UnkTU101.h"
#include "game/UnkTU23.h"
// Needs CSoundManager.h to declare the result UnkSoundHandle (it says void); include it once it does.
struct UnkDropTarget;
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;
using base::global::UnkStringData;

struct UnkSoundObject_b50;
struct UnkSoundRef {
    UnkSoundObject_b50* object;
    s64 serial;
};

struct UnkSoundDef;
struct UnkSoundDesc;
void fn_7101037bc0(UnkSoundDef* self, UnkSoundDesc* out, f32* scales, u32 index);

struct UnkAudioPreset_b0038;
bool fn_710102423c(UnkDropTarget* self, UnkAudioPreset_b0038* preset);
void fn_71010244a8(UnkDropTarget* self, UnkSoundHandle handle);

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

// A string id copied from another (retained), released in two steps.
struct UnkSoundName_l024b28 {
    UnkStringData* data;

    explicit UnkSoundName_l024b28(const CStrId& id) : data(id.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    ~UnkSoundName_l024b28() {
        using base::UnkStrIdMember;
        reinterpret_cast<UnkStrIdMember*>(this)->~UnkStrIdMember();
    }
};

using UnkPathMember_l024b28 = base::UnkStrIdMemberOf<g_unk_7101d47200>;

// A file path copied from another (retained), released in two steps.
struct UnkSoundPath_l024b28 {
    UnkStringData* data;

    explicit UnkSoundPath_l024b28(const CFilePathStrId& id) : data(id.data) {
        if (&g_unk_7101d47200->empty != data)
            ++data->ref_count;
    }
    ~UnkSoundPath_l024b28() { reinterpret_cast<UnkPathMember_l024b28*>(this)->~UnkPathMember_l024b28(); }
    bool IsEmpty() const { return data == &g_unk_7101d47200->empty; }
};

// The sound description fn_7101037bc0 fills, with its defaults.
struct UnkSoundDesc_l024b28 {
    u32 kind;
    u32 field_0x4;
    u64 field_0x8;
    u32 field_0x10;
    u32 field_0x14;  // unset in the original
    UnkSoundName_l024b28 name;  // 0x18
    UnkSoundPath_l024b28 path;  // 0x20
    f32 offset[3];              // 0x28
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

    UnkSoundDesc_l024b28()
        : kind(0), field_0x4(0), field_0x8(0), field_0x10(0), name(g_unk_7101d470e0), path(g_unk_7101d470e8),
          offset{0.0f, 0.0f, 0.0f}, field_0x34(-1.0f), field_0x38(1.0f), field_0x3c(500.0f), field_0x40(2000.0f),
          field_0x44(1.0f), field_0x48(0.0f), field_0x4c(0.0f), field_0x50(0.0f), field_0x54(-1.0f),
          field_0x58(-1.0f), field_0x5c(-1.0f), field_0x60(0.5f), field_0x64(5.0f), field_0x68(10.0f),
          field_0x6c(false), field_0x6d(false), field_0x6e(false) {}
};
static_assert(__builtin_offsetof(UnkSoundDesc_l024b28, field_0x6c) == 0x6c, "layout");

// The sound a handle names while it is still the one the handle was made for.
inline UnkSoundObject_b50* GetSoundObject_l024b28(const UnkSoundHandle* handle) {
    if (handle->ref && handle->serial != -1 && handle->ref->serial == handle->serial)
        return handle->ref->object;
    return nullptr;
}

// Starts the described sound; a positioned preset plays at `pos`, unless it is muted (field_0x5c5).
inline UnkSoundHandle Start_l024b28(const UnkAudioPreset_l024b28* preset, UnkSoundDesc_l024b28& sound,
                                    const f32* pos) {
    if (preset->field_0x574) {
        if (preset->field_0x5c5)
            return g_unk_7108b6c460;
        sound.offset[0] = pos[0];
        sound.offset[1] = pos[1];
        sound.offset[2] = pos[2];
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
    UnkSoundDesc_l024b28 sound;
    fn_7101037bc0(reinterpret_cast<UnkSoundDef*>(preset), reinterpret_cast<UnkSoundDesc*>(&sound),
                  static_cast<f32*>(a), index);

    UnkSoundHandle handle;
    if (sound.path.IsEmpty() && preset->field_0x5a8 > 0.0f) {
        handle = g_unk_7108b6c460;
    } else {
        handle = Start_l024b28(preset, sound, pos);
        if (GetSoundObject_l024b28(&handle))
            fn_71010244a8(self, handle);
    }
    return handle;
}
