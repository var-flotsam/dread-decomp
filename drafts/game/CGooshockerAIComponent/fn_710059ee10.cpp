// CGooshockerAIComponent unit (tools/tu_map.py unit 854): on a hit (any of the flags 0x20e of the
// info's +0xb4) while the owner's attack (+0x278) is active and its movement (+0x218) is in state 3,
// sets a pending move and sends the owner away from its target (fn_710059b0c0 with the normalised
// direction). Names and layouts are placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "base/math/CVectorOps.h"
#include "game/CAttackComponent.h"
#include "game/CEnemyMovement.h"
#include "game/UnkActor.h"
#include "types.h"

using base::math::CVector3D;

struct UnkGooshockerOwner_l59ee10 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x1b0];
    void* movement;  // 0x218
    u8 pad_0x220[0x58];
    void* attack;  // 0x278
};

struct UnkGooshockerAI_l59ee10 {
    u8 pad_0x0[0x30];
    UnkGooshockerOwner_l59ee10* owner;  // 0x30
    u8 pad_0x38[0x146c8 - 0x38];
    bool field_0x146c8;
    u8 pad_0x146c9[0x148e0 - 0x146c9];
    bool field_0x148e0;
    bool field_0x148e1;
};

struct UnkGooshockerHit_l59ee10 {
    u8 pad_0x0[0xb4];
    u16 flags;  // 0xb4
};

void fn_710059b0c0(UnkGooshockerAI_l59ee10* self, const CVector3D* dir, bool arg2, bool arg3);

void fn_710059ee10(UnkGooshockerAI_l59ee10* self, const UnkGooshockerHit_l59ee10* hit) {
    UnkGooshockerOwner_l59ee10* owner = self->owner;
    if (!owner || !owner->attack)
        return;
    void* movement = owner->movement;
    bool attacking = fn_7101136308(owner->attack);
    if (!movement || !attacking || fn_71010cc3a4(movement) != 3 || !(hit->flags & 0x20e))
        return;
    self->field_0x146c8 = false;
    self->field_0x148e1 = true;
    owner = self->owner;
    CVector3D from = {owner->x, owner->y, owner->z};
    UnkActor* target = fn_7100d7c990(reinterpret_cast<UnkActor*>(owner));
    CVector3D to = {target->field_0x44, target->field_0x54, target->field_0x64};
    CVector3D dir = Normalized(from - to);
    self->field_0x148e0 = false;
    fn_710059b0c0(self, &dir, true, false);
}
