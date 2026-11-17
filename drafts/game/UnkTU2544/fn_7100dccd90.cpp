// CBossRushSurvivalConfiguration's unit (tools/tu_map.py unit 2544): ends a boss rush. A survival rush only
// flags itself finished (when the player is not dead); otherwise the first finish is noted in the user data,
// the run is counted and its record saved. Then the game switches to the rewards mode.
// Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2544.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2768.h"
#include "game/UnkTypedValue.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

struct UnkUserDataOwner;
struct UnkRushSurvival_dcd278;
void fn_7100dcd278(UnkRushSurvival_dcd278* self);

extern UnkPropertyName g_unk_71727bd298;

struct UnkPlayerLife_dccd90 {
    u8 pad_0x0[0x70];
    s32 state;  // 2: dead
};

struct UnkPlayer_dccd90 {
    u8 pad_0x0[0x1c0];
    UnkPlayerLife_dccd90* life;
};

struct UnkBossRush_dccd90 {
    u8 pad_0x0[0xba];
    bool survival;  // 0xba
    bool field_0xbb;
    u8 pad_0xbc[0xd0 - 0xbc];
    s32 runs;  // 0xd0
    u8 pad_0xd4[0xc78 - 0xd4];
    bool finished;  // 0xc78
};

bool fn_7100dc8d30();
void fn_7100dcd070(UnkBossRush_dccd90* self);

static inline UnkUserDataOwner* GetOwner() {
    if (!g_unk_7101d47308) {
        UnkUserDataOwner* owner = static_cast<UnkUserDataOwner*>(fn_71000839a0(0x2b8, 0x3f));
        fn_710120320c(owner);
        g_unk_7101d47308 = owner;
    }
    return g_unk_7101d47308;
}

static inline void StoreFlag(CBlackboard* data, UnkPropertyName* name, const bool& value, const CStrId& section_id) {
    if (CBlackboard::CSection* section = fn_71010472e0(data, section_id)) {
        if (!data->field_0xe8) {
            UnkTypedValue v;
            v.type = fn_710000d228();
            v.value = const_cast<bool*>(&value);
            fn_7100332c80(section, name, &v);
        }
    }
}

void fn_7100dccd90(UnkBossRush_dccd90* self) {
    if (self->survival) {
        UnkPlayer_dccd90* player = reinterpret_cast<UnkPlayer_dccd90*>(fn_7100dec788(g_unk_7101d47310));
        if (!player || !player->life || player->life->state == 2)
            return;
        self->finished = true;
        fn_7100deac9c(g_unk_7101d47310, CRntString("BOSSRUSHREWARDS"));
    } else {
        if (!self->field_0xbb && !fn_7100dc8d30()) {
            CBlackboard* data = fn_710120384c(GetOwner());
            bool done = false;
            {
                CStrId section_id("USER_DATA", true);
                if (CBlackboard::CSection* section = fn_7101047270(data, section_id))
                    fn_710006d11c(section, reinterpret_cast<const CStrId*>(&g_unk_71727bd2a0), &done);
            }
            if (!done) {
                StoreFlag(data, &g_unk_71727bd2a0, true, CStrId("USER_DATA", true));
                StoreFlag(data, &g_unk_71727bd298, true, CStrId("USER_DATA", true));
            }
            fn_7101203854(GetOwner());
        }
        if (self->field_0xbb) {
            ++self->runs;
            fn_7100dcd070(self);
        }
        fn_7100dcd278(reinterpret_cast<UnkRushSurvival_dcd278*>(self));
        fn_7100deac9c(g_unk_7101d47310, CRntString("BOSSRUSHREWARDS"));
    }
}
