// CCharClassTakumakuAIComponent: picks the actor's AI state from its frozen, attack, animation-track
// (crazy and push tracks) and Takumaku AI components, and the other actor's melee component.
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAttackComponent.h"
#include "game/CCharClassAIComponent.h"
#include "game/CFrozenComponent.h"
#include "game/CMeleeComponent.h"
#include "game/CTakumakuAIComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU2702.h"
#include "types.h"

using base::reflection::UnkReflObject;

// A position returned in memory (not a plain vector: the original builds it through x8).
struct UnkSluggerPos {
    f32 x;
    f32 y;
    f32 z;
    UnkSluggerPos(const UnkSluggerPos& other);
};

bool fn_71006d6144(void* ai);

struct UnkTakumakuActor {
    u8 pad_0x0[0x38];
    u8 field_0x38[0xc];  // the transform; fn_710002614c returns its matrix
    f32 field_0x44;
    u8 pad_0x48[0xc];
    f32 field_0x54;
    u8 pad_0x58[0xc];
    f32 field_0x64;
    u8 pad_0x68[0x1b8];
    UnkAnimationComponent* field_0x220;
    u8 pad_0x228[0x18];
    UnkReflObject* field_0x240;
    u8 pad_0x248[0x10];
    void* field_0x258;  // the attack component
    u8 pad_0x260[0x68];
    void* field_0x2c8;  // the frozen component
    u8 pad_0x2d0[0x28];
    void* field_0x2f8;  // the melee component
};
static_assert(__builtin_offsetof(UnkTakumakuActor, field_0x2f8) == 0x2f8, "UnkTakumakuActor layout");

struct UnkTakumakuAIState {
    u8 pad_0x0[0x15c];
    u32 field_0x15c;
};

u32 fn_71006d3558(UnkTakumakuAIState* self, UnkTakumakuActor* actor, UnkTakumakuActor* other) {
    fn_7100414160();
    void* attack = actor->field_0x258;
    UnkAnimationComponent* anim = actor->field_0x220;
    void* frozen = actor->field_0x2c8;
    UnkReflObject* ai = actor->field_0x240;
    if (ai && !fn_710009d50c(ai->vf_0x0(), fn_71006d3ba8()))
        ai = nullptr;
    void* melee = other ? other->field_0x2f8 : nullptr;
    if (frozen && fn_71007f9f20(frozen))
        return self->field_0x15c;

    bool attacking = attack && fn_7101136308(attack);
    if (anim) {
        bool chase = ai != nullptr;
        if (attacking && fn_710117b348(anim, fn_7100ce9338())) {
            if (!ai)
                return 2;
            UnkSluggerPos pos = fn_71011008a8(ai, 0);
            f32 dx = other->field_0x44 - pos.x;
            f32 dy = other->field_0x54 - pos.y;
            f32 dz = other->field_0x64 - pos.z;
            const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(actor->field_0x38));
            f32 dot = dx * m.m[2] + dy * m.m[6] + dz * m.m[10];
            if (!(dot < -0.001f) && !fn_71006d6144(ai))
                return 2;
            chase = true;
        }
        if (fn_710117b348(anim, fn_7100bfca98()))
            return 1;
        if (!chase)
            return 1;
    } else if (!ai) {
        return 1;
    }
    if (!fn_71006d44a0(ai))
        return 1;
    if (melee && fn_710092b6f8(melee))
        return 5;
    return 4;
}
