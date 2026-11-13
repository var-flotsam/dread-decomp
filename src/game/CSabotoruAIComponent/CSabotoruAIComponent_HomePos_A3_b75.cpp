// CSabotoruAIComponent's unit (tools/tu_map.py), fn_71006798b8: false when the actor's spawn point (+0x398, its
// component at +0x10) is a CSabotoruSpawnPointComponent; otherwise runs fn_71006799dc and stores the vector at
// +0x14704 as "vHomePos" in the blackboard section g_unk_71726b7cb8 (+0x1260), unless the blackboard is locked
// (as src/game/CYamplotXAIComponent/CYamplotXAIComponent_Message_b69a4.cpp stores "bCombat"), returning what
// fn_71006799dc returned. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CBlackboard.h"
#include "game/UnkTUProperty.h"
#include "game/behaviortree/CBehavior.h"
#include "types.h"
#include "game/UnkTypedValue.h"

using base::global::CStrId;

// As src/game/CAIComponent/CAIComponent_StoreFloat_b49.cpp declares them.
void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);

struct UnkSabotoruSpawnComp_A3_b75 {
    void** vtable;

    const base::reflection::CClass* vf_0x0() {
        return reinterpret_cast<const base::reflection::CClass* (*)(UnkSabotoruSpawnComp_A3_b75*)>(vtable[0])(this);
    }
};

struct UnkSabotoruSpawnHolder_A3_b75 {
    u8 pad_0x0[0x10];
    UnkSabotoruSpawnComp_A3_b75* component;
};

struct UnkSabotoruActor_A3_b75 {
    u8 pad_0x0[0x398];
    UnkSabotoruSpawnHolder_A3_b75* spawn;
};

struct UnkSabotoruAI_A3_b75 {
    u8 pad_0x0[0x30];
    UnkSabotoruActor_A3_b75* actor;
    u8 pad_0x38[0x1260 - 0x38];
    CBlackboard blackboard;  // 0x1260
    u8 pad_0x1350[0x14704 - 0x1260 - sizeof(CBlackboard)];
    base::math::CVector3D home;  // 0x14704
};

bool fn_71006799dc(UnkSabotoruAI_A3_b75* self);

bool fn_71006798b8(UnkSabotoruAI_A3_b75* self) {
    if (UnkSabotoruSpawnHolder_A3_b75* spawn = self->actor->spawn) {
        if (UnkSabotoruSpawnComp_A3_b75* point = spawn->component) {
            if (fn_710009d50c(point->vf_0x0(), fn_7100a1b7b4()))
                return false;
        }
    }
    CBlackboard* blackboard = &self->blackboard;
    bool result = fn_71006799dc(self);
    CStrId name("vHomePos", true);
    UnkTypedValue v;
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, g_unk_71726b7cb8)) {
        if (!blackboard->field_0xe8) {
            base::math::CVector3D* home = &self->home;
            v.type = fn_71000246e4();
            v.value = home;
            fn_7100332c80(s, reinterpret_cast<UnkPropertyName*>(&name), &v);
        }
    }
    return result;
}
