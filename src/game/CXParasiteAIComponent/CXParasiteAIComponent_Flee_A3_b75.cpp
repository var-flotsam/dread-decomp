// CXParasiteAIComponent's unit (tools/tu_map.py), fn_71006e892c: after fn_71006e9384 (mode 2), stores as
// "v3FleePos" (g_unk_717270b870, built in src/game_s7/fn_71006e1010.cpp) in the blackboard section
// g_unk_71726b7cb8 of the component (+0x1260) a point far behind the owner (+0x30) as seen from its target
// (fn_7100d7c990), unless the blackboard is locked (the setter as in
// src/game/CAIComponent/CAIComponent_StoreFloat_b49.cpp). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CBlackboard.h"
#include "game/CXParasiteAIComponent.h"
#include "game/UnkActor.h"
#include "game/UnkTU1102.h"
#include "game/UnkXParasiteAI.h"
#include "game/UnkTUProperty.h"
#include "game/behaviortree/CBehavior.h"
#include "types.h"

using base::global::CStrId;

// As src/game/CAIComponent/CAIComponent_StoreFloat_b49.cpp declares them.
struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};
void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);
// As src/game_s7/fn_71006e1010.cpp declares it.

// What fn_71006e9384 is asked to do (only the kind is set here).
struct UnkXParasiteMode_A3_b75 {
    u32 kind;
    u32 unk_0x4;
    u64 unk_0x8;
};

struct UnkXParasiteVec_A3_b75 {
    f32 x, y, z;
};
static inline UnkXParasiteVec_A3_b75 operator-(const UnkXParasiteVec_A3_b75& a, const UnkXParasiteVec_A3_b75& b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}
static inline UnkXParasiteVec_A3_b75 operator+(const UnkXParasiteVec_A3_b75& a, const UnkXParasiteVec_A3_b75& b) {
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}
static inline UnkXParasiteVec_A3_b75 operator*(const UnkXParasiteVec_A3_b75& a, f32 s) {
    return {a.x * s, a.y * s, a.z * s};
}
static inline UnkXParasiteVec_A3_b75 UnkXParasitePos_A3_b75(const UnkActor* a) {
    return {a->field_0x44, a->field_0x54, a->field_0x64};
}

void fn_71006e892c(UnkXParasiteAI* self) {
    {
        UnkXParasiteMode_A3_b75 mode;
        mode.kind = 2;
        fn_71006e9384(self, &mode);
    }
    UnkActor* target = fn_7100d7c990(self->owner);
    if (!target)
        return;
    CBlackboard* blackboard = &self->blackboard;
    UnkActor* owner = self->owner;
    UnkXParasiteVec_A3_b75 from = UnkXParasitePos_A3_b75(target);
    UnkXParasiteVec_A3_b75 at = UnkXParasitePos_A3_b75(owner);
    UnkXParasiteVec_A3_b75 pos = at + (at - from) * 10000.0f;
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, g_unk_71726b7cb8)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = fn_71000246e4();
            v.value = &pos;
            fn_7100332c80(s, reinterpret_cast<UnkPropertyName*>(g_unk_717270b870), &v);
        }
    }
}
