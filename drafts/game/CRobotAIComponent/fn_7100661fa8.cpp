// CRobotAIComponent's unit: after the base step, when the player lies in front of the owner (positive dot
// product of the owner's direction at +0x218 and the vector to the player), stores true under "bImpacted"
// in the owner's blackboard section g_unk_71726fd2f8 unless the blackboard is locked. Names and layouts
// are placeholders.
#include "base/global/CStrId.h"
#include "base/math/CVectorOps.h"
#include "base/reflection/ReflectedAttributes.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTUProperty.h"
#include "types.h"
#include "game/UnkTypedValue.h"

using base::global::CStrId;

extern CStrId g_unk_71726fd2f8;

struct UnkRobotAI_l661fa8;
void fn_71011088e4(UnkRobotAI_l661fa8* self);

using base::math::CVector3D;

struct UnkRobotBlackboardOwner_l661fa8 {
    u8 pad_0x0[0x1260];
    CBlackboard blackboard;
};

struct UnkRobotFacing_l661fa8 {
    u8 pad_0x0[0x10c];
    f32 x, y, z;
};

struct UnkRobotActor_l661fa8 {
    u8 pad_0x0[0x38];
    f32 transform[3][4];
    CVector3D Position() const { return {transform[0][3], transform[1][3], transform[2][3]}; }
    u8 pad_0x68[0x218 - 0x68];
    UnkRobotFacing_l661fa8* facing;
    u8 pad_0x220[0x240 - 0x220];
    UnkRobotBlackboardOwner_l661fa8* field_0x240;
};

struct UnkRobotAI_l661fa8 {
    u8 pad_0x0[0x30];
    UnkRobotActor_l661fa8* owner;
};

struct UnkRobotPlayer_l661fa8 {
    u8 pad_0x0[0x38];
    f32 transform[3][4];
    CVector3D Position() const { return {transform[0][3], transform[1][3], transform[2][3]}; }
};

void fn_7100661fa8(UnkRobotAI_l661fa8* self) {
    fn_71011088e4(self);
    UnkRobotPlayer_l661fa8* player = reinterpret_cast<UnkRobotPlayer_l661fa8*>(fn_7100dec788(g_unk_7101d47310));
    UnkRobotActor_l661fa8* actor = self->owner;
    UnkRobotFacing_l661fa8* facing = actor->facing;
    if (!facing || !actor)
        return;
    CVector3D delta = player->Position();
    delta = delta - actor->Position();
    if (!(facing->x * delta.x + facing->y * delta.y + facing->z * delta.z > 0.0f))
        return;
    UnkRobotBlackboardOwner_l661fa8* o = actor->field_0x240;
    if (!o)
        return;
    CBlackboard* blackboard = &o->blackboard;
    static CStrId s_name("bImpacted", true);
    bool value = true;
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, g_unk_71726fd2f8)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = fn_710000d228();
            v.value = &value;
            fn_7100332c80(s, reinterpret_cast<UnkPropertyName*>(&s_name), &v);
        }
    }
}
