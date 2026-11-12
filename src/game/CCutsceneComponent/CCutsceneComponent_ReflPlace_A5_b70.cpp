// CCutsceneComponent unit (1208): the reflected invoker of fn_71007a9d8c (CCutsceneComponent_b40.cpp), inlined:
// places the cutscene object at +0x200 at the actor's position plus the current take's offset and returns an
// empty reflected value. The types repeat CCutsceneComponent_b40.cpp's. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkActor.h"
#include "game/UnkTU2661.h"
#include "game/UnkTU83.h"
#include "types.h"

using base::reflection::UnkReflValue;

struct UnkVec3_cs {
    f32 x, y, z;
};

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

static inline UnkVec3_cs Add_A5_b70(const UnkVec3_cs& a, const UnkVec3_cs& b) {
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}

UnkReflValue fn_71007ad814(void*, char* obj) {
    UnkCutsceneComponent_cs* self = *reinterpret_cast<UnkCutsceneComponent_cs**>(obj + 8);
    UnkCutsceneObject_cs* object = self->object;
    const base::global::CStrId* name = object->take ? object->take->name : &g_unk_7101d470e0;
    if (object) {
        void* entry = fn_710106cf74(object, name);
        const UnkVec3_cs& offset = *fn_71000cc374(self->field_0x58, fn_7101068c80(self->object, entry));
        UnkActor* actor = self->actor;
        UnkVec3_cs base = {actor->field_0x44, actor->field_0x54, actor->field_0x64};
        fn_710106bcfc(self->object, Add_A5_b70(base, offset));
    }
    UnkReflValue r;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    r.kind = 2;
    r.field_0x14 = 1;
    return r;
}
