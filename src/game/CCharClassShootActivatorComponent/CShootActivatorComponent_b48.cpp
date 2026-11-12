// CShootActivatorComponent (unit of CCharClassShootActivatorComponent, tools/tu_map.py): fn_7100891b08 reads
// the activator's flag (g_unk_717272c420) from the blackboard section of the current scenario (created when
// missing; the name looked up in the key list at +0x490, count +0x498, values at +0x488, as in
// CEventScenarioComponent_b46.cpp) and, when it is set and its sound (+0x468) is not playing, has the owner
// (+0x30) play the sound named at +0x450 (fn_7100d7c45c), keeping the handle. Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/CCharClassShootActivatorComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2523.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;

CStrId fn_71011a2fbc(UnkGameManagerSub_0x608*);

// A sound handle as src/game/CEntity_2.cpp returns it: the sound's record (its own serial at +8) and the
// serial it had.
struct UnkSoundRef_b48 {
    void* sound;
    s64 serial;
};
struct UnkSoundHandle_b48 {
    UnkSoundRef_b48* ref;
    s64 serial;

    bool Playing() const { return ref && ref->serial == serial && serial != -1 && ref->sound; }
};

struct UnkSoundHandle;
struct UnkEntityWithSound;

struct UnkShootActivatorValue_b48 {
    CStrId id;
    u64 field_0x8;
};

struct UnkShootActivator_b48 {
    u8 pad_0x0[0x30];
    UnkEntityWithSound* owner;
    u8 pad_0x38[0x450 - 0x38];
    u8 sound_name[0x18];
    UnkSoundHandle_b48 sound;
    u8 pad_0x478[0x10];
    UnkShootActivatorValue_b48* values;
    u64* keys;
    u32 count;
};

void fn_7100891b08(UnkShootActivator_b48* self) {
    UnkGameManager* manager = g_unk_7101d47310;
    CBlackboard::CSection* section = fn_71010472e0(&manager->field_0x968, fn_71011a2fbc(manager->field_0x608));
    if (!section)
        return;
    bool activated = false;
    u64 index = 0;
    u32 count = self->count;
    if (count) {
        u64* keys = self->keys;
        u64 key = (u64)g_unk_717272c420.name.data;
        for (; index != count; ++index) {
            if (keys[index] == key)
                break;
        }
    }
    bool found = fn_710006d11c(section, &self->values[index].id, &activated);
    if (activated && found && !self->sound.Playing()) {
        using Play = UnkSoundHandle_b48 (*)(UnkEntityWithSound*, void*, void*, bool);
        self->sound = reinterpret_cast<Play>(&fn_7100d7c45c)(self->owner, self->sound_name, nullptr, false);
    }
}
