// CCutsceneComponent unit (tools/tu_map.py unit 1208): fn_71007a9d8c places the cutscene object at
// +0x200: it looks up the entry for the name of its current take (+0x178; the empty id without one,
// fn_710106cf74, then its index, fn_7101068c80), takes that index's offset from the array at +0x58
// (fn_71000cc374), and moves the object to the actor's position (+0x18) plus that offset
// (fn_710106bcfc). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkActor.h"
#include "types.h"

struct UnkVec3_cs {
    f32 x, y, z;
};

static inline UnkVec3_cs Add_cs(const UnkVec3_cs& a, const UnkVec3_cs& b) {
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}

struct UnkTake_cs {
    const base::global::CStrId* name;
};

struct UnkCutsceneObject_cs {
    u8 pad_0x0[0x178];
    UnkTake_cs* take;
};

struct UnkCutsceneComponent_cs {
    u8 pad_0x0[0x18];
    UnkActor* actor;
    u8 pad_0x20[0x38];
    u8 field_0x58[0x1a8];
    UnkCutsceneObject_cs* object;  // 0x200
};

void* fn_710106cf74(UnkCutsceneObject_cs* object, const base::global::CStrId* name);
u32 fn_7101068c80(UnkCutsceneObject_cs* object, void* entry);
UnkVec3_cs* fn_71000cc374(void* array, u32 index);
void fn_710106bcfc(UnkCutsceneObject_cs* object, const UnkVec3_cs& pos);

void fn_71007a9d8c(UnkCutsceneComponent_cs* self) {
    UnkCutsceneObject_cs* object = self->object;
    if (!object)
        return;
    const base::global::CStrId* name = object->take ? object->take->name : &g_unk_7101d470e0;
    void* entry = fn_710106cf74(object, name);
    const UnkVec3_cs& offset = *fn_71000cc374(self->field_0x58, fn_7101068c80(self->object, entry));
    UnkActor* actor = self->actor;
    UnkVec3_cs base = {actor->field_0x44, actor->field_0x54, actor->field_0x64};
    fn_710106bcfc(self->object, Add_cs(base, offset));
}
