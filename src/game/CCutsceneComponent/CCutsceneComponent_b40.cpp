// CCutsceneComponent unit (tools/tu_map.py unit 1208): fn_71007a9d8c places the cutscene object at
// +0x200: it looks up the entry for the name of its current take (+0x178; the empty id without one,
// fn_710106cf74, then its index, fn_7101068c80), takes that index's offset from the array at +0x58
// (fn_71000cc374), and moves the object to the actor's position (+0x18) plus that offset
// (fn_710106bcfc). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkActor.h"
#include "game/UnkCutsceneComponent_cs.h"
#include "game/UnkCutsceneObject_cs.h"
#include "game/UnkTU2661.h"
#include "game/UnkTU83.h"
#include "game/UnkTake_cs.h"
#include "game/UnkVec3_cs.h"
#include "types.h"

static inline UnkVec3_cs Add_cs(const UnkVec3_cs& a, const UnkVec3_cs& b) {
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}

void fn_71007a9d8c(UnkCutsceneComponent_cs* self) {
    UnkCutsceneObject_cs* object = self->object;
    if (!object)
        return;
    const base::global::CStrId* name = object->take ? object->take->name : &base::global::CStrId::Empty();
    void* entry = fn_710106cf74(object, name);
    const UnkVec3_cs& offset = *fn_71000cc374(self->field_0x58, fn_7101068c80(self->object, entry));
    UnkActor* actor = self->actor;
    UnkVec3_cs base = {actor->field_0x44, actor->field_0x54, actor->field_0x64};
    fn_710106bcfc(self->object, Add_cs(base, offset));
}
