// CXParasiteAIComponent unit (tools/tu_map.py, the name from reflection): clears the state at +0x146f0,
// then stores `true` and the given position under two properties of the blackboard section
// g_unk_71726b7cb8 (blackboard at +0x1260, skipped while it is locked), as fn_71006ea3fc in
// src/game/CXParasiteAIComponent.cpp does. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/UnkTU1102.h"
#include "game/UnkXParasiteAI.h"
#include "game/behaviortree/CBehavior.h"
#include "types.h"
#include "game/UnkTypedValue.h"

struct UnkPropertyName;

extern UnkPropertyName g_unk_717270b8c0 __attribute__((visibility("hidden")));  // a CVector3D in it

struct UnkVector3_b55;

void XParasiteAI_AttractToPowerBomb(UnkXParasiteAI* self, UnkVector3_b55* position) {
    CBlackboard* blackboard = &self->blackboard;
    bool* locked = &blackboard->field_0xe8;
    self->field_0x146f0 = 0;
    self->field_0x146f8 = 0;
    UnkTypedValue v;
    bool flag = true;
    if (CBlackboard::CSection* section = fn_71010472e0(&self->blackboard, g_unk_71726b7cb8)) {
        if (!*locked) {
            v.type = fn_710000d228();
            v.value = &flag;
            fn_7100332c80(section, reinterpret_cast<UnkPropertyName*>(&g_unk_717270b8e0), &v);
        }
    }
    if (CBlackboard::CSection* section = fn_71010472e0(&self->blackboard, g_unk_71726b7cb8)) {
        if (!*locked) {
            v.type = fn_71000246e4();
            v.value = position;
            fn_7100332c80(section, &g_unk_717270b8c0, &v);
        }
    }
}
