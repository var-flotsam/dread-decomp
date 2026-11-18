// CChozoWarriorXAIComponent unit: after the base step, removes a key from the player's ability list
// (+0x5a8), clears id 0x2000 on the owner's controller, and puts every X parasite AI in the registry into
// state 6 for 1.5 seconds, marking it (+0x146e9). Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAnimationComponent.h"
#include "game/CChozoWarriorXAIComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkShakernautId.h"
#include "game/UnkTU1102.h"
#include "game/UnkTU2553.h"
#include "game/UnkXParasiteAI.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkChozoWarriorAI_l498ff0;
void fn_7100498ff0(UnkChozoWarriorAI_l498ff0*);

void fn_71000312c4(void* list, const void* key_ptr);

struct UnkChozoXAbility_l48e5b4 {
    u8 pad_0x0[0x5a8];
    u8 keys[1];  // 0x5a8
};

struct UnkChozoXOwner_l48e5b4 {
    u8 pad_0x0[0x220];
    UnkShakernautCtrl_b53* ctrl;  // 0x220
};

// The registry of X parasite AI components, as src/game/UnkTU2553/UnkTU2553_Manager_ovb0205.cpp declares it.
struct UnkTgmHolder_b205 {
    u8 pad_0x0[0x18];
    void* entity;
};
struct UnkTgmRef_b205 : base::UnkRefCounted {
    u8 pad_0xc[4];
    UnkTgmHolder_b205* target;
};
struct UnkTgmRefVector_b205 {
    UnkTgmRef_b205** data;
    u32 count;
};
void fn_71006e9778(UnkXParasiteAI* self, const s32* state, f32 time);

// Puts the parasite into `state` for `time` seconds.
inline void SetParasiteState(UnkXParasiteAI* parasite, s32 state, f32 time) {
    fn_71006e9778(parasite, &state, time);
}

void fn_710048e5b4(UnkChozoWarriorAI_l498ff0* self) {
    fn_7100498ff0(self);
    UnkAbility* ability = fn_7100df5928(reinterpret_cast<UnkPlayer*>(g_unk_7101d47310));
    fn_71000312c4(reinterpret_cast<UnkChozoXAbility_l48e5b4*>(ability)->keys, g_unk_71726cfa68);
    if (UnkChozoXOwner_l48e5b4* owner = reinterpret_cast<UnkChozoXOwner_l48e5b4*>(fn_7100defcd4(g_unk_7101d47310))) {
        if (UnkShakernautCtrl_b53* ctrl = owner->ctrl) {
            // A 4-byte bit set in the original (UnkShakernautId_b53 reads only its first word).
            u32 bits = 0x2000;
            fn_71011785a8(ctrl, reinterpret_cast<const UnkShakernautId_b53*>(&bits), true, false);
        }
    }
    UnkTgmRefVector_b205* list = fn_71006e9fb4();
    for (UnkTgmRef_b205 **it = list->data, **end = list->data + list->count; it != end; ++it) {
        UnkTgmRef_b205* handle = *it;
        if (!handle)
            continue;
        ++handle->ref_count;
        UnkReflObject* target = reinterpret_cast<UnkReflObject*>(handle->target);
        if (target && fn_710009d50c(target->vf_0x0(), fn_71006e5f28())) {
            UnkXParasiteAI* parasite = reinterpret_cast<UnkXParasiteAI*>(target);
            SetParasiteState(parasite, 6, 1.5f);
            parasite->field_0x146e9 = true;
        }
        if (--handle->ref_count == 0)
            delete handle;
    }
}
