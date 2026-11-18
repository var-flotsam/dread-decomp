// CRedenkiSwarmControllerComponent's unit (tools/tu_map.py, unit 977): plays the sound a pending swarm
// change asks for (mode 1, 2 or 4 at +0x544), at the level the swarm's size reaches (the settings'
// thresholds at +0x8c and +0x90), then clears the request. Names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkTU32.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CRntVector;
using base::global::UnkStringData;

struct UnkEntityWithSound;
struct UnkStrIdPathPair;

UnkSoundHandle fn_7100d7c42c(UnkEntityWithSound* self, void* a, void* b, void* c, void* d);

// The player's sound call (src/game/CEntity_2.cpp), as called here: with flags last.
using UnkPlayAtFn_l658460 = UnkSoundHandle (*)(UnkEntityWithSound*, const UnkStrIdPathPair*, void*, void*, u32);

// A sound: its id, its file path and the resource they resolve to (game/UnkTU32.h's
// UnkStrIdPathResource), each counted.
struct UnkSoundRecord_l658460 {
    UnkStringData* id;
    UnkStringData* path;
    UnkCountedResource* resource;

    UnkSoundRecord_l658460() {
        id = base::global::CStrId::Empty().data;
        if (&g_unk_7101d47108->empty != id)
            ++id->ref_count;
        path = g_unk_7101d470e8.data;
        if (&g_unk_7101d47200->empty != path)
            ++path->ref_count;
        resource = nullptr;
    }
    UnkSoundRecord_l658460(const UnkSoundRecord_l658460& other) {
        id = other.id;
        if (&g_unk_7101d47108->empty != id)
            ++id->ref_count;
        path = other.path;
        if (&g_unk_7101d47200->empty != path)
            ++path->ref_count;
        resource = nullptr;
        if (UnkCountedResource* r = other.resource) {
            ++r->ref_count;
            resource = r;
        }
    }
    ~UnkSoundRecord_l658460() { fn_7100026f70(reinterpret_cast<UnkIdPair*>(this)); }

    // Takes over `other`'s ids and resource (fn_7100027118).
    UnkSoundRecord_l658460& operator=(UnkSoundRecord_l658460&& other) {
        fn_7100027118(reinterpret_cast<UnkStrIdPathResource*>(this), reinterpret_cast<UnkStrIdPathResource*>(&other));
        return *this;
    }

    bool IsSet() const { return id != &g_unk_7101d47108->empty && path != &g_unk_7101d47200->empty; }
};
static_assert(sizeof(UnkSoundRecord_l658460) == 0x18, "UnkSoundRecord_l658460 layout");

struct UnkSwarmSoundSettings_l658460 {
    u8 pad_0x0[0x8c];
    u32 field_0x8c;  // the swarm size the second level starts at
    u32 field_0x90;  // the swarm size the third level starts at
    u8 pad_0x94[0x750 - 0x94];
    UnkSoundRecord_l658460 field_0x750[3];  // for mode 1, by level
    UnkSoundRecord_l658460 field_0x798[3];  // for mode 2
    UnkSoundRecord_l658460 field_0x7e0[3];  // for mode 4

    // The sound of `sounds` for a swarm of `size`.
    UnkSoundRecord_l658460 Pick(const UnkSoundRecord_l658460* sounds, u32 size) const {
        u32 level;
        if (size < field_0x8c)
            level = 0;
        else
            level = size < field_0x90 ? 1 : 2;
        return sounds[level];
    }
};

// A group of the swarm: its count first.
struct UnkSwarmGroup_l658460 {
    u32 count;
    u8 pad_0x4[0xc];
};

struct UnkSwarmController_l658460 {
    u8 pad_0x0[0x30];
    UnkEntityWithSound* owner;  // 0x30
    u8 pad_0x38[0x544 - 0x38];
    u32 mode;              // 0x544: the pending sound request, 0 when none
    u8 field_0x548[0x18];  // where the sound plays
    UnkSwarmSoundSettings_l658460* settings;  // 0x560
};
static_assert(__builtin_offsetof(UnkSwarmController_l658460, settings) == 0x560, "UnkSwarmController_l658460 layout");

void fn_7100658460(UnkSwarmController_l658460* self, const CRntVector<UnkSwarmGroup_l658460>* groups) {
    if (!self->mode)
        return;
    u32 size = 0;
    for (u32 i = 0; i < groups->count; ++i)
        size += groups->data[i].count;
    UnkSoundRecord_l658460 sound;
    switch (self->mode) {
    case 1:
        sound = self->settings->Pick(self->settings->field_0x750, size);
        self->mode = 0;
        break;
    case 2:
        sound = self->settings->Pick(self->settings->field_0x798, size);
        self->mode = 0;
        break;
    case 4:
        sound = self->settings->Pick(self->settings->field_0x7e0, size);
        self->mode = 0;
        break;
    }
    if (sound.IsSet())
        reinterpret_cast<UnkPlayAtFn_l658460>(&fn_7100d7c42c)(
            self->owner, reinterpret_cast<const UnkStrIdPathPair*>(&sound), self->field_0x548, nullptr, 0);
}
