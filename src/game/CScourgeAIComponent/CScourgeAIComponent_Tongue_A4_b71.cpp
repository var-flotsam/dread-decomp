// CScourgeAIComponent's unit (tools/tu_map.py): when the owner (+0x30) has a character class (+0x220) and the
// tongue part (+0x14738) exists: unless the class is the extend-tongue track and the owner's +0x2c8 does
// not answer fn_71007f9f20, it hands the part's (+0x608) entry for the owner to fn_7100d35a98 (flag off);
// otherwise it calls fn_7100709f20 for the part with the owner, the 2D projection of the part's position
// (+0x64c) and its value at +0x648. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkBroadphaseObject.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

// As src/game/CHydrogigaAIComponent/CHydrogigaAIComponent_Spawn_A3_b62.cpp declares it.
struct UnkHydrogigaTarget_b62a3;
struct UnkHydrogigaOwner_b62a3;
struct UnkHydrogigaKey_b62a3;
void fn_7100709f20(UnkHydrogigaTarget_b62a3* target, UnkHydrogigaOwner_b62a3* owner, UnkHydrogigaKey_b62a3* key,
                   f32 value);
bool fn_71007f9f20(void*);
UnkEntry* fn_7100709e9c(void* part, void* owner);

struct UnkScourgeTongue_A4_b71 {
    u8 pad_0x0[0x608];
    u8 target[0x40];  // 0x608
    f32 value;        // 0x648
    f32 x;            // 0x64c
    f32 y;            // 0x650
    f32 z;            // 0x654
};

struct UnkScourgeOwner_A4_b71 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* cls;  // 0x220
    u8 pad_0x228[0x2c8 - 0x228];
    void* field_0x2c8;  // 0x2c8
};

struct UnkScourgeAI_A4_b71 {
    u8 pad_0x0[0x30];
    UnkScourgeOwner_A4_b71* owner;  // 0x30
    u8 pad_0x38[0x14738 - 0x38];
    UnkScourgeTongue_A4_b71* tongue;  // 0x14738
};

void fn_71006a052c(UnkScourgeAI_A4_b71* self) {
    UnkScourgeOwner_A4_b71* owner = self->owner;
    UnkScourgeTongue_A4_b71** slot = &self->tongue;
    asm("" : "+r"(slot));
    UnkAnimationComponent* cls = owner->cls;
    UnkScourgeTongue_A4_b71* tongue = *slot;
    if (!cls || !tongue)
        return;
    void* other = owner->field_0x2c8;
    if (!fn_710117b348(cls, fn_7100c4bca8()) || (other && fn_71007f9f20(other))) {
        UnkScourgeTongue_A4_b71* t = *slot;
        if (!t)
            return;
        if (UnkEntry* e = fn_7100709e9c(t->target, self->owner))
            fn_7100d35a98(e, false);
    } else {
        UnkScourgeTongue_A4_b71* t = *slot;
        u8* target = t->target;
        UnkScourgeOwner_A4_b71* o = self->owner;
        CVector2D p;
        CVector3D v = {t->x, t->y, t->z};
        fn_710007b430(&p, &v);
        fn_7100709f20(reinterpret_cast<UnkHydrogigaTarget_b62a3*>(target),
                      reinterpret_cast<UnkHydrogigaOwner_b62a3*>(o), reinterpret_cast<UnkHydrogigaKey_b62a3*>(&p),
                      (*slot)->value);
    }
}
