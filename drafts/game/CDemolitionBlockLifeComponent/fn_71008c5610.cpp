// CDemolitionBlockLifeComponent unit (tools/tu_map.py unit 1374): stops the block's three sounds and, unless
// the scenario's blackboard section records the block as destroyed (g_unk_7172730c98), plays the first one
// again and then the husk or the plain second sound. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/CDemolitionBlockLifeComponent.h"
#include "game/CScenario.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2523.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CStrId;

struct UnkActor;
struct UnkEntityWithSound;
struct UnkStrIdPathPair;

struct UnkDemolitionCollider_l8c5610 {
    u8 pad_0x0[0x6a];
    bool enabled;  // 0x6a
};

struct UnkDemolitionOwner_l8c5610 {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* colliders;  // 0x2a0
};

struct UnkDemolitionValue_l8c5610 {
    CStrId id;
    u64 field_0x8;
};

struct UnkDemolitionSound_l8c5610 {
    u8 ids[0x18];
};

struct UnkDemolitionBlock_l8c5610 {
    u8 pad_0x0[0x30];
    UnkDemolitionOwner_l8c5610* owner;  // 0x30
    u8 pad_0x38[0x268 - 0x38];
    UnkDemolitionValue_l8c5610* values;  // 0x268
    u64* keys;                           // 0x270
    u32 count;                           // 0x278
    u8 pad_0x27c[0x298 - 0x27c];
    UnkDemolitionSound_l8c5610 intact_sound;  // 0x298
    UnkDemolitionSound_l8c5610 husk_sound;    // 0x2b0
    UnkDemolitionSound_l8c5610 plain_sound;   // 0x2c8
    UnkSoundHandle intact_handle;  // 0x2e0
    UnkSoundHandle husk_handle;    // 0x2f0
    UnkSoundHandle plain_handle;   // 0x300
};

static inline void Stop(UnkDemolitionBlock_l8c5610* self, UnkDemolitionSound_l8c5610* sound) {
    fn_7100d7c548(reinterpret_cast<UnkActor*>(self->owner), reinterpret_cast<const UnkStrIdPathPair*>(sound),
                  0.2f, 0.0f);
}

static inline UnkSoundHandle Play(UnkDemolitionBlock_l8c5610* self, UnkDemolitionSound_l8c5610* sound) {
    using PlayFn = UnkSoundHandle (*)(UnkEntityWithSound*, void*, void*, bool);
    return reinterpret_cast<PlayFn>(&fn_7100d7c45c)(reinterpret_cast<UnkEntityWithSound*>(self->owner), sound,
                                                    nullptr, false);
}

// Whether the scenario's blackboard section does not record the block as destroyed.
static inline bool IsIntact(UnkDemolitionBlock_l8c5610* self) {
    bool destroyed = false;
    UnkGameManager* manager = g_unk_7101d47310;
    CBlackboard* blackboard = &manager->field_0x968;
    CStrId section_id = fn_71011a2fbc(manager->field_0x608);
    u64 index = 0;
    u32 count = self->count;
    if (count) {
        u64* keys = self->keys;
        u64 key = (u64)g_unk_7172730c98.name.data;
        for (; index != count; ++index) {
            if (keys[index] == key)
                break;
        }
    }
    UnkDemolitionValue_l8c5610* values = self->values;
    CBlackboard::CSection* section = fn_7101047270(blackboard, section_id);
    if (!section)
        return true;
    if (fn_710006d11c(section, &values[index].id, &destroyed))
        return false;
    return !destroyed;
}

// Whether the named collider exists and is enabled.
static inline bool ColliderEnabled(UnkCollisionContainer* colliders, const char* name) {
    CStrId id(name, true);
    auto* collider = reinterpret_cast<UnkDemolitionCollider_l8c5610*>(fn_7100864194(colliders, &id));
    if (!collider || !collider->enabled)
        return false;
    return true;
}

void fn_71008c5610(UnkDemolitionBlock_l8c5610* self) {
    Stop(self, &self->intact_sound);
    Stop(self, &self->husk_sound);
    Stop(self, &self->plain_sound);
    if (!IsIntact(self))
        return;
    self->intact_handle = Play(self, &self->intact_sound);
    UnkCollisionContainer* colliders = self->owner->colliders;
    if (colliders && ColliderEnabled(colliders, "HUSK"))
        self->husk_handle = Play(self, &self->husk_sound);
    else
        self->plain_handle = Play(self, &self->plain_sound);
}
