// Looks up two entries by key in the owner's collision component (the object at +0x2a0 of the
// owner when it is of that class) and, when both exist, runs fn_7100d35a98 on the second with 0 and
// on the first with 1.
#include "game/CAutectorAIComponent.h"
#include "game/UnkCollisionRecord.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkSelf.h"
#include "types.h"

using base::reflection::CClass;

struct UnkComponent_3bb408 {
    void** vtable;
    const CClass* vf_0x0() { return reinterpret_cast<const CClass* (*)(UnkComponent_3bb408*)>(vtable[0])(this); }
};

struct UnkOwner {
    u8 pad_0x0[0x2a0];
    UnkComponent_3bb408* field_0x2a0;
};

struct UnkEntry;

#include "game/UnkBehaviorTree.h"
#include "game/UnkBroadphaseObject.h"

void fn_71003bb408(UnkSelf* self) {
    fn_7100369650(reinterpret_cast<char*>(self));
    UnkComponent_3bb408* object = self->field_0x30->field_0x2a0;
    UnkComponent_3bb408* component = nullptr;
    if (object && fn_710009d50c(object->vf_0x0(), fn_710085fc44()))
        component = object;
    UnkEntry* first = reinterpret_cast<UnkEntry*>(fn_7100864194(
        reinterpret_cast<UnkCollisionContainer*>(component), reinterpret_cast<const base::global::CStrId*>(&g_unk_71726bd8d0[0])));
    UnkEntry* second = reinterpret_cast<UnkEntry*>(fn_7100864194(
        reinterpret_cast<UnkCollisionContainer*>(component), reinterpret_cast<const base::global::CStrId*>(&g_unk_71726bd8d0[1])));
    if (first && second) {
        fn_7100d35a98(second, 0);
        fn_7100d35a98(first, 1);
    }
}
