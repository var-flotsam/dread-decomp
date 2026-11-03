// CCharClassSluggerAIComponent: after the base character class's check (which wins when it answers),
// 1 unless the actor's track allows crazy mode (CCharClassAllowCrazyTrack); then 2 when the other
// actor is in front of the actor's AI position along its facing (the third column of the actor's
// matrix), else 1. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

// A position returned in memory (not a plain vector: the original builds it through x8).
struct UnkSluggerPos {
    f32 x;
    f32 y;
    f32 z;
    UnkSluggerPos(const UnkSluggerPos& other);
};

// The base character class's check, called with the same arguments (declared without them in its
// generated definition).
u32 fn_7100414160();
UnkSluggerPos fn_71011008a8(void* ai, u32 index);

u32 fn_71006c53a8(void* self, UnkActor* actor, UnkActor* other) {
    if (u32 result = fn_7100414160())
        return result;
    UnkAnimationComponent* track = static_cast<UnkAnimationComponent*>(actor->field_0x220);
    if (!track)
        return 1;
    void* ai = actor->field_0x240;
    if (!fn_710117b348(track, fn_7100ce9338()))
        return 1;
    f32 x = other->field_0x44;
    f32 y = other->field_0x54;
    f32 z = other->field_0x64;
    UnkSluggerPos pos = fn_71011008a8(ai, 0);
    f32 dx = x - pos.x;
    f32 dy = y - pos.y;
    f32 dz = z - pos.z;
    const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(actor->field_0x38));
    f32 fx = m.m[2];
    f32 fy = m.m[6];
    f32 fz = m.m[10];
    return dx * fx + dy * fy + dz * fz > 0.0f ? 2 : 1;
}
