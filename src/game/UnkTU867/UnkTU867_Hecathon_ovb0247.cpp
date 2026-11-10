// Unit 867 (tools/tu_map.py): the Hecathon AI components of the other actors in a list which are
// within a distance (in the plane) of an origin actor, or have no offset set, are handed to
// fn_71005ab918. Names and layouts are placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkHecathonComponent_b0247 {
    virtual const base::reflection::CType* vf_0x0();
    u8 pad_0x8[0x14794];
    f32 field_0x1479c;
    u8 pad_0x147a0[0x60];
};
static_assert(__builtin_offsetof(UnkHecathonComponent_b0247, field_0x1479c) == 0x1479c, "layout");

void fn_71005ab918(UnkHecathonComponent_b0247* component);

static inline bool InRange(UnkHecathonComponent_b0247& component, f32 distance, f32 radius) {
    return (distance < radius) | (component.field_0x1479c == -1.0f);
}

struct UnkActorNode_b0247 {
    UnkActor* actor;
    UnkActorNode_b0247* next;  // 0x8
};

struct UnkActorList_b0247 {
    u8 pad_0x0[0x18];
    UnkActorNode_b0247* head;  // 0x18
};

void fn_71005aca7c(UnkActorList_b0247* self, UnkActor* origin, f32 radius) {
    for (UnkActorNode_b0247* node = self->head; node; node = node->next) {
        UnkActor* actor = node->actor;
        if (actor != origin) {
            auto* component = static_cast<UnkHecathonComponent_b0247*>(actor->field_0x240);
            if (component && fn_710009d50c(component->vf_0x0(), fn_71005a7e74())) {
                f32 ax = origin->field_0x44;
                f32 ay = origin->field_0x54;
                f32 az = origin->field_0x64;
                base::math::CVector2D a2;
                base::math::CVector3D a = {ax, ay, az};
                fn_710007b430(&a2, &a);
                f32 bx = actor->field_0x44;
                f32 by = actor->field_0x54;
                f32 bz = actor->field_0x64;
                base::math::CVector2D b2;
                base::math::CVector3D b = {bx, by, bz};
                fn_710007b430(&b2, &b);
                f32 dx = a2.x - b2.x;
                f32 dy = a2.y - b2.y;
                f32 distance = sqrtf(dx * dx + dy * dy);
                if (InRange(*component, distance, radius))
                    fn_71005ab918(component);
            }
        }
    }
}
