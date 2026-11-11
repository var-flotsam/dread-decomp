// animtree::CBoneFilterNodeDef's unit (tools/tu_map.py): the definition's destructor (fn_71002e2f88),
// and the copy constructor (fn_71002e3264) and move assignment (fn_71002e3608) its reflected class
// uses (src/reflection/animtree_CBoneFilterNodeDef.cpp). Layout from the reflected fields: vBones at
// 0x10, vBoneHierarchies at 0x38; the base class (vtable 0x7101a21718) holds a name at 0x8.
#include <new>

#include "base/UnkCloneRecords.h"
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/animtree/CAnimatedNodeDef.h"
#include "game/animtree/CBoneFilterNodeDef.h"
#include "types.h"

using base::global::CStrId;
using base::UnkStrIdMember;

struct UnkBoneFilterNodeDef_ovb0138 {
    char* vtable;
    UnkStrIdMember name;                         // 0x8
    base::UnkVector<UnkStrIdMember> bones;        // 0x10
    base::UnkVector<UnkStrIdMember> hierarchies;  // 0x38
};
static_assert(sizeof(UnkBoneFilterNodeDef_ovb0138) == 0x60, "UnkBoneFilterNodeDef_ovb0138 layout");

void fn_71002e2f88(UnkBoneFilterNodeDef_ovb0138* self) {
    self->vtable = reinterpret_cast<char*>(g_unk_7101a21c70) + 0x10;
    self->hierarchies.Free();
    self->bones.Free();
    self->vtable = reinterpret_cast<char*>(g_unk_7101a21718) + 0x10;
    self->name.~UnkStrIdMember();
}

// Copy construction: the ids are retained and the arrays copied element by element.
void fn_71002e3264(void* dst, const void* src) {
    UnkBoneFilterNodeDef_ovb0138* self = static_cast<UnkBoneFilterNodeDef_ovb0138*>(dst);
    const UnkBoneFilterNodeDef_ovb0138* other = static_cast<const UnkBoneFilterNodeDef_ovb0138*>(src);
    self->vtable = reinterpret_cast<char*>(g_unk_7101a21718) + 0x10;
    new (&self->name) CStrId(reinterpret_cast<const CStrId&>(other->name));
    self->vtable = reinterpret_cast<char*>(g_unk_7101a21c70) + 0x10;
    new (&self->bones) base::UnkStrIdVec(reinterpret_cast<const base::UnkStrIdVec&>(other->bones));
    new (&self->hierarchies) base::UnkStrIdVec(reinterpret_cast<const base::UnkStrIdVec&>(other->hierarchies));
}

// Move assignment: the name is copied, the arrays taken over.
void* fn_71002e3608(void* dst, const void* src) {
    UnkBoneFilterNodeDef_ovb0138* self = static_cast<UnkBoneFilterNodeDef_ovb0138*>(dst);
    UnkBoneFilterNodeDef_ovb0138* other =
        const_cast<UnkBoneFilterNodeDef_ovb0138*>(static_cast<const UnkBoneFilterNodeDef_ovb0138*>(src));
    if (self != other) {
        self->name = other->name;
        self->bones = static_cast<base::UnkVector<UnkStrIdMember>&&>(other->bones);
        self->hierarchies = static_cast<base::UnkVector<UnkStrIdMember>&&>(other->hierarchies);
    }
    return self;
}
