// CInfesterAIComponent's unit (tools/tu_map.py), fn_71005cb0b8: as fn_71006792e8
// (src/game/CSabotoruAIComponent/CSabotoruAIComponent_PlayerPos_A3_b75.cpp): after the base update
// (fn_7100369c44), unless the actor's +0x1b8 is in state 2, runs fn_71005cb1e4 while the state at +0x146d0 is 1
// and stores the target's position as "vPlayerPos" in the blackboard. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CBlackboard.h"
#include "game/UnkActor.h"
#include "game/UnkTUProperty.h"
#include "game/behaviortree/CBehavior.h"
#include "types.h"
#include "game/UnkBehaviorTree.h"

using base::global::CStrId;

// As src/game/CAIComponent/CAIComponent_StoreFloat_b49.cpp declares them.
struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};
void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);
// As src/game/CGobblerAIComponent/fn_71005820c4.cpp declares it.
struct UnkGobblerComponent;

struct UnkInfesterState_A3_b75 {
    u8 pad_0x0[0x70];
    s32 state;
};

struct UnkInfesterUpdAI_A3_b75 {
    u8 pad_0x0[0x30];
    UnkActor* actor;
    u8 pad_0x38[0x1260 - 0x38];
    CBlackboard blackboard;  // 0x1260
    u8 pad_0x1350[0x146d0 - 0x1260 - sizeof(CBlackboard)];
    s32 field_0x146d0;
};

void fn_71005cb1e4(UnkInfesterUpdAI_A3_b75* self, f32 dt);

void fn_71005cb0b8(UnkInfesterUpdAI_A3_b75* self, f32 dt) {
    fn_7100369c44(reinterpret_cast<UnkSelf_7100369c44*>(self), dt);  // with `dt`, as the original
    if (static_cast<UnkInfesterState_A3_b75*>(self->actor->field_0x1b8)->state == 2)
        return;
    if (self->field_0x146d0 == 1)
        fn_71005cb1e4(self, dt);
    CBlackboard* blackboard = &self->blackboard;
    CStrId name("vPlayerPos", true);
    UnkTypedValue v;
    UnkActor* target = fn_7100d7c990(self->actor);
    u32 x = *reinterpret_cast<const u32*>(&target->field_0x44);
    u32 y = *reinterpret_cast<const u32*>(&target->field_0x54);
    u32 z = *reinterpret_cast<const u32*>(&target->field_0x64);
    alignas(8) u32 pos[3];
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, g_unk_71726b7cb8)) {
        if (!blackboard->field_0xe8) {
            v.type = fn_71000246e4();
            v.value = pos;
            fn_7100332c80(s, reinterpret_cast<UnkPropertyName*>(&name), &v);
        }
    }
}
