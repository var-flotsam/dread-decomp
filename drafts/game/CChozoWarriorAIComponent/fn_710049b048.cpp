// CChozoWarriorAIComponent's unit (tools/tu_map.py): while +0x149a0 is set, counts the timer at +0x1499c
// down; while it runs the owner's timeline gets the all-flags mask, and when it runs out the warrior leaves
// the state (+0x14999), stops two animation ids, tells the team manager when attack 0x14ae0 is current and
// sends the animation a one. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/reflection/UnkReflStatics.h"
#include "../../../src/game_s2/UnkSpitSend.h"
#include "game/CAnimationComponent.h"
#include "game/CAttackComponent.h"
#include "game/CTeamManager/CTunableTeamManager.h"
#include "game/UnkBufferStringPool.h"
#include "game/UnkShakernautId.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

struct UnkChozoWarriorKey_l49b048;
extern UnkChozoWarriorKey_l49b048 g_unk_7101649a18;  // reached through the GOT

struct UnkChozoWarriorOwner_l49b048 {
    u8 pad_0x0[0x220];
    void* field_0x220;
    u8 pad_0x228[0x278 - 0x228];
    void* field_0x278;
    u8 pad_0x280[0x2a0 - 0x280];
    void* field_0x2a0;
    u8 pad_0x2a8[0x2e8 - 0x2a8];
    void* field_0x2e8;
};

struct UnkChozoWarriorAttack_l49b048 {
    u8 pad_0x0[0x50];
};

struct UnkChozoWarriorAI_l49b048 {
    u8 pad_0x0[0x30];
    UnkChozoWarriorOwner_l49b048* owner;  // 0x30
    u8 pad_0x38[0x1148 - 0x38];
    UnkChozoWarriorAttack_l49b048* field_0x1148;
    u8 pad_0x1150[0x14999 - 0x1150];
    bool field_0x14999;
    u8 pad_0x1499a[0x1499c - 0x1499a];
    f32 timer;  // 0x1499c
    bool field_0x149a0;
    u8 pad_0x149a1[0x14ae0 - 0x149a1];
    UnkChozoWarriorAttack_l49b048 attack_0x14ae0;
};
static_assert(__builtin_offsetof(UnkChozoWarriorAI_l49b048, attack_0x14ae0) == 0x14ae0, "");

bool fn_710049b048(UnkChozoWarriorAI_l49b048* self, f32 dt) {
    // Declared past a goto (no lifetime markers), so each keeps its own slot.
    goto start;
    base::reflection::UnkReflTypeIdArg mask;
    UnkShakernautId_b53 first;
    UnkShakernautId_b53 second;
    base::reflection::UnkReflTypeIdArg value;
    base::reflection::UnkReflTypeIdArg type;
start:
    if (!self->field_0x149a0)
        return false;
    UnkChozoWarriorOwner_l49b048* owner = self->owner;
    void* anim = owner->field_0x220;
    void* body = owner->field_0x2a0;
    if (!anim || !body)
        return false;
    self->timer -= dt;
    if (!(self->timer <= 0.0f)) {
        auto* timeline = static_cast<UnkTimelineComponent*>(owner->field_0x2e8);
        if (timeline) {
            mask.id = UnkAll_SpitSend().value;
            fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_7101649a18), &mask.id, false, -1.0f);
        }
        return true;
    }
    self->field_0x14999 = false;
    first.value = 0x10;
    fn_71011785a8(static_cast<UnkShakernautCtrl_b53*>(anim), &first, true, false);
    second.value = 0x20;
    fn_71011785a8(static_cast<UnkShakernautCtrl_b53*>(anim), &second, true, false);
    void* object = self->owner->field_0x278;
    if (object && !fn_7101136308(object) && self->field_0x1148 == &self->attack_0x14ae0) {
        char* instance = g_unk_71726b2398;
        if (!instance) {
            instance = static_cast<char*>(fn_71000839a0(0x108, 0x3f));
            fn_71011a664c(instance);
            g_unk_71726b2398 = instance;
        }
        fn_71011a75f0(instance, reinterpret_cast<UnkCancelOwner_ovb0128*>(self->owner));
    }
    value.id = 1;
    type.id = base::reflection::UnkReflTypeId<base::reflection::UnkReflTypeIdTag>::id();
    fn_7101176690(anim, 0x6eb7ed2e6750a7b0, 0xf4061ff25a2163d6, &value, &g_unk_710164b958, &g_unk_7172769740, &type);
    return true;
}
