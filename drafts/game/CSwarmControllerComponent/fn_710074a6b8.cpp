// CSwarmControllerComponent's unit: keeps one looping sound per group list in step with the list's size.
// An empty list fades its sound out; otherwise the size picks a level (the tunables' thresholds at +0x8c and
// +0x90), the controller's vf_0x238 the sound for it, and the sound is (re)started at the list's position
// unless the one playing is that sound at that level already, which is only moved. Names and layouts are
// placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/snd/UnkSound.h"
#include "game/CFlockingSwarmControllerComponent.h"
#include "game/CSwarmControllerComponent.h"
#include "game/UnkTU32.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CStrId;
using base::global::UnkStringData;
using base::math::CVector3D;

struct UnkEntityWithSound;
UnkSoundHandle fn_7100d7c42c(UnkEntityWithSound* self, void* a, void* b, void* c, void* d);

// Moves a playing sound to `position`.
void fn_7100105ebc(UnkSound* self, const CVector3D* position);

// A playing sound as this function sees it: its name.
struct UnkSwarmPlayingSound_l74a6b8 {
    u8 pad_0x0[0x88];
    CStrId name;  // 0x88

    UnkSound* Sound() { return reinterpret_cast<UnkSound*>(this); }
};

struct UnkSwarmSoundRef_l74a6b8 {
    UnkSwarmPlayingSound_l74a6b8* sound;
    s64 serial;
};

// A weak handle to a playing sound: valid while the serials agree.
struct UnkSwarmSoundHandle_l74a6b8 {
    UnkSwarmSoundRef_l74a6b8* ref;
    s64 serial;

    bool IsValid() const { return ref && ref->serial == serial && serial != -1; }
    UnkSwarmPlayingSound_l74a6b8* Get() const { return IsValid() ? ref->sound : nullptr; }
};

// A sound preset (sound::CAudioPreset when the class check passes): its name.
struct UnkSwarmAudioPreset_l74a6b8 : base::reflection::UnkReflObject {
    u8 pad_0x8[0x50];
    CStrId name;  // 0x58
};

// A sound to play: its id, its path and the preset they resolve to.
struct UnkSwarmSoundRecord_l74a6b8 {
    UnkStringData* id;
    UnkStringData* path;
    UnkSwarmAudioPreset_l74a6b8* preset;

    ~UnkSwarmSoundRecord_l74a6b8() { fn_7100026f70(reinterpret_cast<UnkIdPair*>(this)); }

    // The preset's name, or the empty id when there is no preset.
    const CStrId& PresetName() const {
        UnkSwarmAudioPreset_l74a6b8* p = preset;
        if (p && fn_710009d50c(p->vf_0x0(), fn_710102df18()))
            return p->name;
        return g_unk_7101d470e0;
    }
};

// The player's sound call (src/game/CEntity_2.cpp), as called here: with flags last.
using UnkPlayAtFn_l74a6b8 = UnkSwarmSoundHandle_l74a6b8 (*)(UnkEntityWithSound*, const UnkSwarmSoundRecord_l74a6b8*,
                                                            const CVector3D*, void*, u32);

// A group list's state: the level its sound plays at and that sound's name.
struct UnkSwarmGroupList_l74a6b8 {
    u8 pad_0x0[0x28];
    u32 level;    // 0x28
    u8 pad_0x2c[4];
    CStrId sound;  // 0x30
};

// One list's size and where it is.
struct UnkSwarmListSize_l74a6b8 {
    u32 count;
    CVector3D position;
};

struct UnkSwarmGroupLists_l74a6b8 {
    void* data;
    u32 count;  // 0x8
};

struct UnkSwarmSoundTunables_l74a6b8 {
    u8 pad_0x0[0x8c];
    u32 midLevelAt;   // 0x8c
    u32 highLevelAt;  // 0x90

    u32 Level(u32 size) const {
        if (size < midLevelAt)
            return 0;
        return size < highLevelAt ? 1 : 2;
    }
};

struct UnkSwarmSoundController_l74a6b8 {
    virtual void vf_0x0();
    u8 pad_0x8[0x28];
    UnkEntityWithSound* owner;  // 0x30
    u8 pad_0x38[0x170];
    UnkSwarmGroupLists_l74a6b8 groupLists;  // 0x1a8
    u8 pad_0x1b8[0x28];
    UnkSwarmSoundTunables_l74a6b8* tunables;  // 0x1e0
    u8 pad_0x1e8[0x48];
    CRntVector<UnkShotWeakRef> sounds;  // 0x230
    u8 pad_0x258[4];
    bool restart;  // 0x25c

    UnkSwarmGroupList_l74a6b8* List(u32 i) {
        return reinterpret_cast<UnkSwarmGroupList_l74a6b8*>(
            fn_71007410a4(reinterpret_cast<UnkArray_fs*>(&groupLists), i));
    }
    UnkSwarmSoundHandle_l74a6b8* Sound(u32 i) {
        return reinterpret_cast<UnkSwarmSoundHandle_l74a6b8*>(fn_7100748064(&sounds, i));
    }
    // vf_0x238: the sound for a level.
    UnkSwarmSoundRecord_l74a6b8 SoundFor(u32 level) {
        typedef UnkSwarmSoundRecord_l74a6b8 (*Fn)(UnkSwarmSoundController_l74a6b8*, u32);
        return (*reinterpret_cast<Fn**>(this))[0x238 / 8](this, level);
    }
    UnkSwarmSoundHandle_l74a6b8 Play(const UnkSwarmSoundRecord_l74a6b8* record, const CVector3D* position) {
        return reinterpret_cast<UnkPlayAtFn_l74a6b8>(&fn_7100d7c42c)(owner, record, position, nullptr, 0);
    }
};
static_assert(__builtin_offsetof(UnkSwarmSoundController_l74a6b8, groupLists) == 0x1a8, "layout");
static_assert(__builtin_offsetof(UnkSwarmSoundController_l74a6b8, tunables) == 0x1e0, "layout");
static_assert(__builtin_offsetof(UnkSwarmSoundController_l74a6b8, sounds) == 0x230, "layout");
static_assert(__builtin_offsetof(UnkSwarmSoundController_l74a6b8, restart) == 0x25c, "layout");

void fn_710074a6b8(UnkSwarmSoundController_l74a6b8* self, const CRntVector<UnkSwarmListSize_l74a6b8>* sizes) {
    u32 n = self->groupLists.count;
    for (u32 i = 0; i < n; ++i) {
        const UnkSwarmListSize_l74a6b8& size = sizes->data[i];
        UnkSwarmSoundHandle_l74a6b8* sound = self->Sound(i);
        if (size.count == 0) {
            if (sound->IsValid())
                fn_7100105a80(sound->ref->sound->Sound(), 0.2f, 0.0f);
            continue;
        }
        u32 level = self->tunables->Level(size.count);
        UnkSwarmSoundRecord_l74a6b8 record = self->SoundFor(level);
        if (!sound->IsValid()) {
            *sound = self->Play(&record, &size.position);
            const CStrId& name = sound->IsValid() ? sound->ref->sound->name : g_unk_7101d470e0;
            self->List(i)->sound = name;
            *self->Sound(i) = *sound;
        } else {
            CStrId presetName = record.PresetName();
            if (!self->restart && level == self->List(i)->level && presetName.data == self->List(i)->sound.data) {
                fn_7100105ebc(sound->Get()->Sound(), &size.position);
            } else {
                self->restart = false;
                fn_7100105a80(sound->Get()->Sound(), 0.2f, 0.0f);
                *sound = self->Play(&record, &size.position);
                if (sound->IsValid())
                    self->List(i)->sound = sound->ref->sound->name;
                *self->Sound(i) = *sound;
            }
        }
        self->List(i)->level = level;
    }
}
