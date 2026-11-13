// CQuetzoaAIComponent's unit (tools/tu_map.py), fn_710064ab64: after the base update (fn_71010c424c), when the
// actor's spawn point (+0x398, its component at +0x10) has an area (fn_7101127964), stores in the blackboard
// section g_unk_71726b7cb8 (+0x1260) as "bIsInCombatArea" whether the area holds (vf 0x50) the 2D projection of
// the target's (fn_7100d7c990) position, unless the blackboard is locked (as
// src/game/CYamplotXAIComponent/CYamplotXAIComponent_Message_b69a4.cpp stores "bCombat"). Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CBlackboard.h"
#include "game/CComponent.h"
#include "game/UnkActor.h"
#include "game/UnkTUProperty.h"
#include "game/behaviortree/CBehavior.h"
#include "types.h"
#include "game/UnkTypedValue.h"

using base::global::CStrId;

// As src/game/CAIComponent/CAIComponent_StoreFloat_b49.cpp declares them.
void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);
// As src/game/CAbilityEnergyWave/fn_71002ffea8.cpp declares it.
struct UnkParent;

struct UnkQuetzoaArea_A3_b75 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual bool vf_0x50(const base::math::CVector2D* point);  // whether the area holds the point
};
UnkQuetzoaArea_A3_b75* fn_7101127964(void* spawn_point);

struct UnkQuetzoaSpawnHolder_A3_b75 {
    u8 pad_0x0[0x10];
    void* component;
};

struct UnkQuetzoaAI_A3_b75 {
    u8 pad_0x0[0x30];
    UnkActor* actor;
    u8 pad_0x38[0x1260 - 0x38];
    CBlackboard blackboard;  // 0x1260
};

void fn_710064ab64(UnkQuetzoaAI_A3_b75* self) {
    fn_71010c424c(reinterpret_cast<UnkParent*>(self));
    UnkQuetzoaSpawnHolder_A3_b75* holder =
        *reinterpret_cast<UnkQuetzoaSpawnHolder_A3_b75**>(reinterpret_cast<char*>(self->actor) + 0x398);
    if (!holder || !holder->component)
        return;
    UnkQuetzoaArea_A3_b75* area = fn_7101127964(holder->component);
    if (!area)
        return;
    CBlackboard* blackboard = &self->blackboard;
    bool value;
    {
        UnkActor* target = fn_7100d7c990(self->actor);
        u32 x = *reinterpret_cast<const u32*>(&target->field_0x44);
        u32 y = *reinterpret_cast<const u32*>(&target->field_0x54);
        u32 z = *reinterpret_cast<const u32*>(&target->field_0x64);
        alignas(8) u32 pos[4];  // 16 bytes: the original keeps this slot above the point's
        pos[0] = x;
        pos[1] = y;
        pos[2] = z;
        base::math::CVector2D point;
        fn_710007b430(&point, reinterpret_cast<const base::math::CVector3D*>(pos));
        value = area->vf_0x50(&point);
    }
    CStrId name("bIsInCombatArea", true);
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, g_unk_71726b7cb8)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = fn_710000d228();
            v.value = &value;
            fn_7100332c80(s, reinterpret_cast<UnkPropertyName*>(&name), &v);
        }
    }
}
