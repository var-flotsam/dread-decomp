// CSceneModelAnimationComponent unit (tools/tu_map.py): stores a string id at +0x40 (copy-assigned),
// then, when the object at +0x10 is a CCharClassSceneModelAnimationComponent (fn_71009f1b24's class),
// passes the id on to it (fn_71009f2608). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

void fn_71009f2608(UnkReflObject* self, const CStrId* id);

struct UnkSceneModelAnimation_b52 {
    u8 pad_0x0[0x10];
    UnkReflObject* owner;  // 0x10
    u8 pad_0x18[0x28];
    CStrId id;  // 0x40
};

void fn_71009f33d8(UnkSceneModelAnimation_b52* self, const CStrId* id) {
    self->id = *id;
    if (UnkReflObject* owner = self->owner) {
        if (fn_710009d50c(owner->vf_0x0(), fn_71009f1b24()))
            fn_71009f2608(owner, id);
    }
}
