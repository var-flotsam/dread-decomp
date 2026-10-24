// Creates a CEnemyPreset through its reflected class and stores it in the object's list of
// owned objects: at `index` (replacing and deleting what was there) or, for a negative
// index, at the end. Returns the new object.
#include "UnkClone.h"
#include "base/UnkVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::CClass;
using base::reflection::CType;

const CClass* fn_7100576258();  // CEnemyPreset::refl_GetClass

struct UnkClone_fn_71003b2904;
using UnkOwner_71003b24fc = UnkCloneOwner<UnkClone_fn_71003b2904>;

// The element accessor is the shared out-of-line copy (src/clones/, fn_71003b2904).
extern template UnkOwner_71003b24fc& base::UnkVector<UnkOwner_71003b24fc>::GrowAt(u32);

struct UnkStruct_71003b24fc {
    u8 unk_0x0[0x58];
    base::UnkVector<UnkOwner_71003b24fc> field_0x58;
};

static inline UnkCloneObject* CreateEnemyPreset() {
    const CClass* cls = fn_7100576258();
    if (fn_710009d50c(cls, fn_7100576258()) && cls->m_fn_0x28) {
        void* p = fn_7100083990()->vf_0x40(cls->m_size, 8, 4);
        if (p)
            reinterpret_cast<void (*)(void*)>(cls->m_fn_0x28)(p);
        return static_cast<UnkCloneObject*>(p);
    }
    return nullptr;
}

UnkCloneObject* fn_71003b24fc(UnkStruct_71003b24fc* self, s32 index) {
    UnkCloneObject* obj = CreateEnemyPreset();
    base::UnkVector<UnkOwner_71003b24fc>* v = &self->field_0x58;
    UnkOwner_71003b24fc owner;
    owner.ptr = obj;
    if (index < 0) {
        v->EmplaceBack(static_cast<UnkOwner_71003b24fc&&>(owner));
    } else {
        if (index < (s32)v->count)
            delete v->GrowAt(index).ptr;
        v->GrowAt(index) = static_cast<UnkOwner_71003b24fc&&>(owner);
    }
    return obj;
}
