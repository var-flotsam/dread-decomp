// Unit 2666 Lua binding: pushes a property bag holding four zeroed values ("LR", "LF", "RR", "RF")
// as a table. Names and layouts are placeholders.
#include "../../../lib/lua/lua.h"
#include "../../../lib/lua/lauxlib.h"
#include "base/UnkVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CAIManager/CTunableAIManager.h"
#include "game/CCharClassAIComponent.h"
#include "game/CDroppableLifeComponent.h"
#include "game/CLifeComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkManager_71726af468.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "../../game_s7/UnkPropBagS7.h"
#include "game/UnkTU0.h"
#include "game/UnkTU1370.h"
#include "game/UnkTU2677.h"
#include "game/UnkTU2730.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStrIdBase;
using base::global::UnkStringPool;
using base::global::UnkStringData;
using base::reflection::UnkReflValue;
using unk_prop_bag::UnkPropBag;

s32 fn_710108ea80(lua_State* L) {
    UnkPropBag bag;
    fn_710033187c(&bag, CStrId("LR", true), 0.0f);
    fn_710033187c(&bag, CStrId("LF", true), 0.0f);
    fn_710033187c(&bag, CStrId("RR", true), 0.0f);
    fn_710033187c(&bag, CStrId("RF", true), 0.0f);
    return fn_71010c0b40(L, "t", &bag);
}

// ---- a pass over the game manager's entities ----

struct UnkStruct_71008c0c74;
struct UnkBase_71003cc474;
struct UnkCharacterLife_b0050;

struct UnkEntity_b0165;
void* fn_7100362248(UnkEntity_b0165* entity);
void* fn_710077368c(UnkEntity_b0165* entity);
void* fn_7100ba4680(UnkEntity_b0165* entity);
void* fn_710031caf8(UnkEntity_b0165* entity);

struct UnkAiMgrTask_b0165 {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkAiMgr_b0165 {
    u8 pad_0x0[0x2410];
    UnkAiMgrTask_b0165* field_0x2410;
};

static inline UnkAiMgr_b0165* AiMgr_b0165() {
    UnkManager_71726af468* m = g_unk_71726af468;
    if (!m) {
        m = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(m);
        g_unk_71726af468 = m;
    }
    return reinterpret_cast<UnkAiMgr_b0165*>(m);
}

struct UnkEntityNode_b0165 {
    void* field_0x0;
    UnkEntity_b0165* entity;
    UnkEntityNode_b0165* next;
};

// The boss AI component's owner of a sub-object (+0x10) with a phase counter (+0x1140).
struct UnkBossAi_b0165 {
    void** vtable;
    u8 pad_0x8[8];
    base::reflection::UnkReflObject* field_0x10;
    u8 pad_0x18[0x1140 - 0x18];
    s32 field_0x1140;

    void vf_0x208(s32 phase, bool flag) {
        reinterpret_cast<void (*)(UnkBossAi_b0165*, s32, bool)>(vtable[0x208 / 8])(this, phase, flag);
    }
};

struct UnkLifeComp_b0165 {
    void** vtable;
    void vf_0x168(f32 value, bool flag) {
        reinterpret_cast<void (*)(UnkLifeComp_b0165*, f32, bool)>(vtable[0x168 / 8])(this, value, flag);
    }
    void vf_0x180(bool a, bool b) {
        reinterpret_cast<void (*)(UnkLifeComp_b0165*, bool, bool)>(vtable[0x180 / 8])(this, a, b);
    }
};

struct UnkEntityTarget_b0165 {
    void** vtable;
};

struct UnkRequest_b0165 {
    u8 pad_0x0[0x100];
    UnkRequest_b0165() { fn_71008c0c74(reinterpret_cast<UnkStruct_71008c0c74*>(this)); }
    ~UnkRequest_b0165() { fn_71008c1558(reinterpret_cast<UnkBase_71003cc474*>(this)); }
};

static inline void Send_b0165(UnkEntityTarget_b0165* target, UnkRequest_b0165* request) {
    reinterpret_cast<void (*)(UnkEntityTarget_b0165*, UnkRequest_b0165*)>(target->vtable[0x2e0 / 8])(target, request);
}

s32 fn_7101080060(lua_State* L) {
    if (fn_7101196fd8(g_unk_7101d47330))
        return 0;
    UnkAiMgr_b0165* ai = AiMgr_b0165();
    if (ai->field_0x2410 && ai->field_0x2410->field_0x10)
        return 0;
    if (fn_7100da9254(reinterpret_cast<UnkManager_71726af468*>(AiMgr_b0165())))
        return 0;
    for (UnkEntityNode_b0165* node = *reinterpret_cast<UnkEntityNode_b0165**>(
             reinterpret_cast<char*>(g_unk_7101d47310) + 0x2638);
         node; node = node->next) {
        UnkEntity_b0165* entity = node->entity;
        if (UnkBossAi_b0165* boss = static_cast<UnkBossAi_b0165*>(fn_7100362248(entity))) {
            base::reflection::UnkReflObject* part = boss->field_0x10;
            if (part && fn_710009d50c(part->vf_0x0(), fn_71004186ec())) {
                s32 next_phase = boss->field_0x1140 + 1;
                if (next_phase < fn_7100413fe0(part)) {
                    boss->vf_0x208(next_phase, true);
                    continue;
                }
            }
        } else if (!fn_710077368c(entity)) {
            continue;
        }
        UnkLifeComp_b0165* life = static_cast<UnkLifeComp_b0165*>(fn_71007bb940(entity));
        if (!life)
            continue;
        if (fn_7101161088(life))
            continue;
        if (!fn_7101160408(reinterpret_cast<UnkCharacterLife_b0050*>(life)) || fn_7100ba4680(entity)) {
            life->vf_0x180(true, false);
            continue;
        }
        life->vf_0x168(0.0f, true);
        if (UnkEntityTarget_b0165* target = static_cast<UnkEntityTarget_b0165*>(fn_710031caf8(entity))) {
            UnkRequest_b0165 request;
            Send_b0165(target, &request);
        }
    }
    return 0;
}
