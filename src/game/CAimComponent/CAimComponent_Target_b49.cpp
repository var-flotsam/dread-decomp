// CAimComponent (tools/tu_map.py): the point to aim at on `actor` (zero without one): its translation,
// replaced by the position of its collision object named by its AI's (+0x240, +0x10; a
// CCharClassAIComponent) aim name (+0x150), or, when it has none, of the object named
// g_unk_71726b16e0 in the component's own target (+0x5c8, +0x10). Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkXformNode.h"
#include "game/logic/collision/CCollider.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CClass;

struct UnkShape;
extern CStrId g_unk_71726b16e0 __attribute__((visibility("hidden")));

struct UnkAimCharClass_b49 {
    void** vtable;
    u8 pad_0x8[0x148];
    CStrId aim_name;
    const CClass* vf_0x0() { return reinterpret_cast<const CClass* (*)(UnkAimCharClass_b49*)>(vtable[0])(this); }
};

struct UnkAimAI_b49 {
    u8 pad_0x0[0x10];
    UnkAimCharClass_b49* char_class;
};

struct UnkAimActor_b49 {
    u8 pad_0x0[0x38];
    UnkXform transform;
    u8 pad_0x94[0x1ac];
    UnkAimAI_b49* ai;
    u8 pad_0x248[0x58];
    UnkCollisionContainer* collision;
};

struct UnkAimLink_b49 {
    u8 pad_0x0[0x10];
    UnkAimActor_b49* actor;
};

struct UnkAim_b49 {
    u8 pad_0x0[0x5c8];
    UnkAimLink_b49* target;
};

static inline UnkVec3f Position(UnkCollisionObject* object) {
    UnkMatrix44 m = fn_7100d36a2c(reinterpret_cast<UnkShape*>(object));
    return UnkVec3f(m.m[3], m.m[7], m.m[11]);
}

UnkVec3f fn_7100351ed4(UnkAim_b49* self, UnkAimActor_b49* actor) {
    UnkVec3f result(0.0f, 0.0f, 0.0f);
    if (!actor)
        return result;
    result = actor->transform.GetTranslation();
    UnkCollisionObject* object;
    UnkAimCharClass_b49* char_class;
    if (actor->ai && (char_class = actor->ai->char_class) &&
        fn_710009d50c(char_class->vf_0x0(), fn_7100408b2c()) &&
        char_class->aim_name.data != &g_unk_7101d47108->empty && actor->collision) {
        object = fn_71008640a4(actor->collision, &char_class->aim_name);
        if (!object)
            return result;
    } else {
        UnkAimLink_b49* link = self->target;
        if (!link || !link->actor || !link->actor->collision)
            return result;
        object = fn_71008640a4(link->actor->collision, &g_unk_71726b16e0);
        if (!object)
            return result;
    }
    result = Position(object);
    return result;
}
