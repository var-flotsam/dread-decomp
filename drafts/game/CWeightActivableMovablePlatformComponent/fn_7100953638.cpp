// CWeightActivableMovablePlatformComponent unit (unit 1432): the update. After the base update, until the
// platform is on (+0x278): when the player's collision component stands on this platform's owner, it sends
// the owner's animation the definition's (+0x250) request, notifies the owner's object, turns the platform on,
// stores that in the current scenario's blackboard and calls the script function named at +0x270.
// Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/CNavMeshItemComponent.h"
#include "game/CScenario.h"
#include "game/CWeightActivableMovablePlatformComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU372.h"
#include "game/UnkTUProperty.h"
#include "game/UnkTypedValue.h"
#include "types.h"

using base::global::CStrId;

struct UnkWeakTarget;
struct UnkWeightPlatform_l953638;
void fn_71009517dc(UnkWeightPlatform_l953638* self, f32 dt);  // the base update

struct UnkWeightValue_l953638 {
    UnkPropertyName name;
    u64 field_0x8;
};

struct UnkWeightDef_l953638 {
    u8 pad_0x0[0x78];
    CStrId id;  // 0x78
};

struct UnkWeightOwner_l953638 {
    u8 pad_0x0[0x220];
    void* animation;  // 0x220
};

struct UnkWeightCollision_l953638 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkWeightOwner_l953638* owner;  // 0x30

    const base::reflection::CClass* vf_0x0() {
        return reinterpret_cast<const base::reflection::CClass* (*)(UnkWeightCollision_l953638*)>(vtable[0])(this);
    }
};

struct UnkWeightLink_l953638 {
    u8 pad_0x0[0x10];
    UnkWeightCollision_l953638* collision;  // 0x10
};

struct UnkWeightLinkHolder_l953638 {
    u8 pad_0x0[0x10];
    UnkWeightLink_l953638* link;  // 0x10
};

struct UnkWeightPlayerSub_l953638 {
    u8 pad_0x0[0x128];
    UnkWeightLinkHolder_l953638* holder;  // 0x128
};

struct UnkWeightPlayer_l953638 {
    u8 pad_0x0[0x200];
    UnkWeightPlayerSub_l953638* field_0x200;
};

struct UnkWeightPlatform_l953638 {
    u8 pad_0x0[0x30];
    UnkWeightOwner_l953638* owner;  // 0x30
    u8 pad_0x38[0x218];
    UnkWeightDef_l953638* def;  // 0x250
    u8 pad_0x258[0x18];
    CStrId on_function;  // 0x270
    bool on;  // 0x278
    u8 pad_0x279[0x17];
    UnkWeightValue_l953638* values;  // 0x290
    u64* keys;  // 0x298
    u32 count;  // 0x2a0
};
static_assert(__builtin_offsetof(UnkWeightPlatform_l953638, count) == 0x2a0, "layout");

// Stores `value` under `name` in the blackboard section `section` names (unless the
// blackboard is locked).
inline void SetSectionBool(CBlackboard* blackboard, const CStrId& section, UnkPropertyName* name, bool value) {
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = fn_710000d228();
            v.value = &value;
            fn_7100332c80(s, name, &v);
        }
    }
}

// Sends `target` a request with mode 0 under `hash`; false when it was not taken.
inline bool SendRequest(void* target, u64 hash) {
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 0;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    return fn_7101176690(target, hash, 0xf4061ff25a2163d6ull, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
}

void fn_7100953638(UnkWeightPlatform_l953638* self, f32 dt) {
    fn_71009517dc(self, dt);
    if (self->on)
        return;
    if (self->def->id.data == &g_unk_7101d47108->empty)
        return;
    auto* player = reinterpret_cast<UnkWeightPlayer_l953638*>(fn_7100dec788(g_unk_7101d47310));
    if (!player || !player->field_0x200 || !player->field_0x200->holder || !player->field_0x200->holder->link)
        return;
    UnkWeightCollision_l953638* collision = player->field_0x200->holder->link->collision;
    if (!collision || !fn_710009d50c(collision->vf_0x0(), fn_710085fc44()))
        return;
    UnkWeightOwner_l953638* owner = collision->owner;
    if (owner != self->owner)
        return;
    if (!SendRequest(owner->animation, self->def->id.data->Key()))
        return;
    if (void* object = fn_71002b3b84(reinterpret_cast<UnkWeakTarget*>(self->owner)))
        fn_71009c0f1c(object, 0xb15b31b835c17820ull);
    self->on = true;

    UnkGameManager* manager = g_unk_7101d47310;
    CBlackboard* blackboard = &manager->field_0x968;
    CStrId section_id = fn_71011a2fbc(manager->field_0x608);
    u64 index = 0;
    u32 count = self->count;
    if (count) {
        u64* keys = self->keys;
        u64 key = (u64)g_unk_7172739f40.name.data;
        for (; index != count; ++index) {
            if (keys[index] == key)
                break;
        }
    }
    UnkWeightValue_l953638* values = self->values;
    SetSectionBool(blackboard, section_id, &values[index].name, true);
    if (self->on_function.data != &g_unk_7101d47108->empty)
        fn_7100000790(self->on_function.Text(), "o", self->owner);
}
