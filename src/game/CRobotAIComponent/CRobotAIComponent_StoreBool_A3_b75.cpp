// CRobotAIComponent's unit (tools/tu_map.py), fn_7100661eb8: stores `false` under the name at +0x8e0 of the
// reflection singleton in the blackboard section g_unk_71726b7cb8 of the object at +0x240 of the owner
// (+0x30), unless its blackboard (+0x1260) is locked (the setter as in
// src/game/CAIComponent/CAIComponent_StoreFloat_b49.cpp). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CBlackboard.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;

// As src/game/CAIComponent/CAIComponent_StoreFloat_b49.cpp declares them.
struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};
void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);
extern base::global::CStrId g_unk_71726b7cb8;

struct UnkRobotBlackboardOwner_A3_b75 {
    u8 pad_0x0[0x1260];
    CBlackboard blackboard;
};

struct UnkRobotActor_A3_b75 {
    u8 pad_0x0[0x240];
    UnkRobotBlackboardOwner_A3_b75* field_0x240;
};

struct UnkRobotAI_A3_b75 {
    u8 pad_0x0[0x30];
    UnkRobotActor_A3_b75* owner;
};

void fn_7100661eb8(UnkRobotAI_A3_b75* self) {
    UnkRobotBlackboardOwner_A3_b75* o = self->owner->field_0x240;
    if (!o)
        return;
    CBlackboard* blackboard = &o->blackboard;
    UnkPropertyName* name = reinterpret_cast<UnkPropertyName*>(
        reinterpret_cast<char*>(&base::reflection::refl_GetSingleton()) + 0x8e0);
    bool value = false;
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, g_unk_71726b7cb8)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = fn_710000d228();
            v.value = &value;
            fn_7100332c80(s, name, &v);
        }
    }
}
