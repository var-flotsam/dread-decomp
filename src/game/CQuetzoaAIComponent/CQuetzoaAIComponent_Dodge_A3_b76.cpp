// CQuetzoaAIComponent's unit (tools/tu_map.py), fn_710064ba50: on message 1, runs fn_7101105e14 with 0, drops
// the weak handle at +0x14b18 (unlinking +0x14b00 from its target first), clears the flag at +0x14ae8, stores what
// fn_710064b0b4 makes of the actor's (+0x30) position at +0x14af4 and stores false as "bIsDodging" in the blackboard
// section g_unk_71726b7cb8 (+0x1260), unless the blackboard is locked. Returns whether the message was ignored.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CBlackboard.h"
#include "game/UnkActor.h"
#include "game/UnkTUProperty.h"
#include "game/behaviortree/CBehavior.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

// As src/game/CAIComponent/CAIComponent_StoreFloat_b49.cpp declares them.
struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};
void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);

struct UnkQuetzoaPos_A3_b76 {
    alignas(8) u32 v[4];  // 16 bytes: shares the name's slot and keeps its own place
    UnkQuetzoaPos_A3_b76(u32 x, u32 y, u32 z) {
        v[0] = x;
        v[1] = y;
        v[2] = z;
    }
};

// Returned in memory: the type is not trivially copyable.
// 16 bytes: the original keeps its slot above the position's.
struct UnkQuetzoaVec_A3_b76 {
    f32 x, y, z;
    u32 unk_0xc;
    UnkQuetzoaVec_A3_b76() = default;
    UnkQuetzoaVec_A3_b76(const UnkQuetzoaVec_A3_b76& o) : x(o.x), y(o.y), z(o.z) {}
    UnkQuetzoaVec_A3_b76& operator=(const UnkQuetzoaVec_A3_b76& o) {
        x = o.x;
        y = o.y;
        z = o.z;
        return *this;
    }
};

struct UnkQuetzoaDodge_A3_b76 {
    bool flag;  // 0x14ae8
    u8 pad_0x1[0xc - 0x1];
    f32 x, y, z;  // 0x14af4

    u8 link[0x18];  // 0x14b00
    UnkWeakHandle* handle;  // 0x14b18
};

struct UnkQuetzoaDodgeAI_A3_b76 {
    u8 pad_0x0[0x30];
    UnkActor* actor;
    u8 pad_0x38[0x1260 - 0x38];
    CBlackboard blackboard;  // 0x1260
    u8 pad_0x1350[0x14ae8 - 0x1260 - sizeof(CBlackboard)];
    UnkQuetzoaDodge_A3_b76 dodge;  // 0x14ae8
};

void fn_7101105e14(UnkQuetzoaDodgeAI_A3_b76* self, f32 value);
UnkQuetzoaVec_A3_b76 fn_710064b0b4(UnkQuetzoaDodgeAI_A3_b76* self, const UnkQuetzoaPos_A3_b76& pos);

bool fn_710064ba50(UnkQuetzoaDodgeAI_A3_b76* self, const u32* message) {
    if (*message != 1)
        return true;
    UnkQuetzoaDodge_A3_b76* dodge = &self->dodge;
    fn_7101105e14(self, 0.0f);
    if (UnkWeakHandle* handle = dodge->handle) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, dodge->link);
        ResetRef(&dodge->handle);
    }
    dodge->flag = false;
    {
        UnkActor* actor = self->actor;
        u32 x = *reinterpret_cast<const u32*>(&actor->field_0x44);
        u32 y = *reinterpret_cast<const u32*>(&actor->field_0x54);
        u32 z = *reinterpret_cast<const u32*>(&actor->field_0x64);
        UnkQuetzoaVec_A3_b76 r = fn_710064b0b4(self, UnkQuetzoaPos_A3_b76(x, y, z));
        dodge->x = r.x;
        dodge->y = r.y;
        dodge->z = r.z;
    }
    CBlackboard* blackboard = &self->blackboard;
    CStrId name("bIsDodging", true);
    bool value = false;
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, g_unk_71726b7cb8)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = fn_710000d228();
            v.value = &value;
            fn_7100332c80(s, reinterpret_cast<UnkPropertyName*>(&name), &v);
        }
    }
    return false;
}
