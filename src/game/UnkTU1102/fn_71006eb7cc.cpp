// Unit 1102 (tools/tu_map.py; the X parasite AI component): the component's pick among its entries (the
// table at +0x14740). Of the entries whose holder passes fn_710112a3bc and has an actor, it returns a counted
// reference to the one whose actor is farthest from the actor fn_7100d7c990 gives for the owner, or nothing.
// Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "base/math/CVectorOps.h"
#include "game/UnkActor.h"
#include "game/UnkTU1102.h"
#include "game/UnkXParasiteAI.h"
#include "game/UnkXParasiteResult.h"
#include "types.h"

bool fn_710112a3bc(void* holder);

using base::math::CVector3D;
static inline CVector3D Pos(const UnkActor* a) { return {a->field_0x44, a->field_0x54, a->field_0x64}; }

UnkXParasiteResult_b0252 XParasiteAI_FindFarthestEntry(UnkXParasiteAI* self) {
    UnkXParasiteResult_b0252 result;
    result.ptr = nullptr;
    UnkActor* actor = fn_7100d7c990(self->owner);
    if (actor) {
        UnkWeakHandle** it = self->entries.data;
        UnkWeakHandle** end = self->entries.count + it;
        f32 best = -1.0f;
        for (; end != it; ++it) {
            UnkWeakHandle* entry = *it;
            if (!entry || !entry->target || !Holder(entry)->actor)
                continue;
            if (!fn_710112a3bc(Holder(entry)))
                continue;
            CVector3D a = Pos(actor);
            UnkActor* other = Holder(*it)->actor;
            CVector3D b = Pos(other);
            f32 d2 = DistanceSq(a, b);
            if (d2 > best) {
                fn_71006eb8cc(reinterpret_cast<base::UnkRefHandle*>(&result), reinterpret_cast<const base::UnkRefHandle*>(it));
                best = d2;
            }
        }
    }
    return result;
}
