// CWeightActivableMovablePlatformComponent's load of its saved flag: clears the flag (+0x278), looks the
// component's property up in the key list (+0x298, count +0x2a0, 16-byte values at +0x290), reads it from the
// current scenario's blackboard section (as src/game/CActionSwitcherComponent/CActionSwitcherComponent_b48.cpp's
// fn_7100875d98) and, when it was set, flags the owner's (+0x30) animation (+0x2c8), sends it a request under
// the definition's (+0x250) id and notifies the owner's object with one of two ids by the definition's flag
// (+0x70). Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/CNavMeshItemComponent.h"
#include "game/CScenario.h"
#include "game/CWeightActivableMovablePlatformComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU372.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;

struct UnkWeakTarget;

struct UnkWeightValue_b0266 {
    CStrId id;
    u64 field_0x8;
};

struct UnkWeightIdHolder_b0266 {
    u8 pad_0x0[0x30];
    u64 hash;  // 0x30
};

struct UnkWeightIdRef_b0266 {
    u8 pad_0x0[0x78];
    UnkWeightIdHolder_b0266* holder;  // 0x78
};

struct UnkWeightDef_b0266 {
    u8 pad_0x0[0x70];
    bool field_0x70;
    u8 pad_0x71[7];
};

struct UnkWeightAnimation_b0266 {
    u8 pad_0x0[0x2c8];
    u32 field_0x2c8;
};

struct UnkWeightOwner_b0266 {
    u8 pad_0x0[0x220];
    UnkWeightAnimation_b0266* animation;  // 0x220
};

struct UnkWeightPlatform_b0266 {
    u8 pad_0x0[0x30];
    union {
        UnkWeakTarget* owner;
        UnkWeightOwner_b0266* owner_actor;
    };
    u8 pad_0x38[0x218];
    union {
        UnkWeightDef_b0266* def;
        UnkWeightIdRef_b0266* def_ids;
    };                                  // 0x250
    u8 pad_0x258[0x20];
    bool on;                            // 0x278
    u8 pad_0x279[0x17];
    UnkWeightValue_b0266* values;       // 0x290
    u64* keys;                          // 0x298
    u32 count;                          // 0x2a0
};
static_assert(__builtin_offsetof(UnkWeightPlatform_b0266, count) == 0x2a0, "layout");

void fn_71009533fc(UnkWeightPlatform_b0266* self) {
    UnkGameManager* manager = g_unk_7101d47310;
    CBlackboard* blackboard = &manager->field_0x968;
    CStrId section_id = fn_71011a2fbc(manager->field_0x608);
    self->on = false;
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
    UnkWeightValue_b0266* values = self->values;
    if (CBlackboard::CSection* section = fn_7101047270(blackboard, section_id)) {
        bool* on = &self->on;
        if (fn_710006d11c(section, &values[index].id, on) && *on) {
            self->owner_actor->animation->field_0x2c8 = 1;
            UnkWeightAnimation_b0266* animation = self->owner_actor->animation;
            u64 hash = self->def_ids->holder->hash;
            base::reflection::UnkReflTypeIdArg mode;
            mode.id = 0;
            base::reflection::UnkReflTypeIdArg mask;
            mask.id = UnkAll_SpitSend().value;
            fn_7101176690(animation, hash, 0xf4061ff25a2163d6ull, &mode, &g_unk_710164b958, &g_unk_717276974c, &mask);
            if (void* object = fn_71002b3b84(self->owner)) {
                u64 id;
                if (self->def->field_0x70) {
                    id = 0xb0c1699f8b57b14bull;
                    // keeps the two ids as separate branches, as the original does
                    asm("" : "+r"(id));
                } else {
                    id = 0xb15b31b835c17820ull;
                    asm("" : "+r"(id));
                }
                fn_71009c0f1c(object, id);
            }
        }
    }
}
