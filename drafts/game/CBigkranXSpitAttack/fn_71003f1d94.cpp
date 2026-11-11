// CBigkranXSpitAttack unit: the point the spit is aimed at. A stored point of (0, 0, 666) means
// "the target's position"; it is then offset by the target's state and by the spit origin.
// Names and layouts are placeholders.

#include "base/UnkMatrix44.h"
#include "game/UnkActor.h"
#include "game/UnkXformNode.h"
#include "types.h"

bool fn_71009e12d0(void*);
bool fn_7100979ef0(void* tracked);
f32 fn_7100977b20(void* tracked);

struct UnkSpitOffset {
    f32 x, y, z;
};

struct UnkSpitOwner {
    u8 pad_0x0[0x148];
    UnkSpitOffset* field_0x148;
};

struct UnkSpitAttack {
    u8 pad_0x0[0x8];
    UnkActor* field_0x8;
    u8 pad_0x10[0x18];
    UnkSpitOwner* field_0x28;
    u8 pad_0x30[0xb8];
    u32 field_0xe8;
    UnkVec3f field_0xec;  // the aim point
};

struct UnkActorTracked {
    u8 pad_0x0[0x208];
    void* field_0x208;
    void* field_0x210;
};

inline bool Near(f32 a, f32 b) {
    return a - 0.1f <= b && a + 0.1f >= b;
}

UnkVec3f fn_71003f1d94(UnkSpitAttack* self, bool keepOffset) {
    UnkVec3f r = self->field_0xec;
    UnkActor* actor = self->field_0x8;
    if (!actor)
        return r;
    UnkActor* target = fn_7100d7c990(actor);
    if (!target)
        return r;
    f32 x = r.x;
    f32 y = r.y;
    if (Near(x, 0.0f) && Near(y, 0.0f) && Near(r.z, 666.0f)) {
        r = UnkVec3f(target->field_0x44, target->field_0x54, target->field_0x64);
        x = r.x;
        y = r.y;
    }
    UnkActorTracked* tracked = reinterpret_cast<UnkActorTracked*>(target);
    void* movement = tracked->field_0x208;
    void* other = tracked->field_0x210;
    if (movement && fn_71009e12d0(movement)) {
        if (self->field_0x8->field_0x54 < target->field_0x54) {
            y += -50.0f;
            r.y = y;
        }
        UnkVec3f angle = reinterpret_cast<UnkXform*>(target->field_0x38)->GetAngle();
        x += angle.y > 0.0f ? 20.0f : -20.0f;
        r.x = x;
    } else if (other) {
        if (fn_7100979ef0(other)) {
            f32 d = target->field_0x44 - self->field_0x8->field_0x44;
            f32 side = (f32)((d < 0.0f) - (d > 0.0f)) * 0.5f;
            x += fn_7100977b20(other) * side;
            r.x = x;
        }
    } else {
        y += -50.0f;
        r.y = y;
    }
    if (!keepOffset) {
        const UnkSpitOffset& offset = self->field_0x28->field_0x148[self->field_0xe8];
        f32 ox = offset.x;
        f32 oy = offset.y;
        const UnkMatrix44& m =
            fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->field_0x8->field_0x38));
        r.x = x + (m.m[2] > 0.0f ? ox : -ox);
        r.y = oy + y;
    }
    return r;
}
