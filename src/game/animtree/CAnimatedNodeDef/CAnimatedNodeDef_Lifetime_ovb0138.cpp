// animtree::CAnimatedNodeDef's unit (tools/tu_map.py): the definition's destructor (fn_71002d3d10),
// and the copy constructor (fn_71002d4108) and move assignment (fn_71002d44dc) its reflected class
// uses (src/reflection/animtree_CAnimatedNodeDef.cpp). Layout from the reflected fields: vBones at
// 0x18, vBoneHierarchies at 0x40, bLaunchEvents and bAllowRootMotion at 0x68/0x69; the base class
// (vtable 0x7101a21718) holds a name at 0x8. Names of the rest are placeholders.
#include <new>

#include "base/UnkCloneRecords.h"
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;
using base::UnkStrIdMember;

extern void* g_unk_7101a216e8[];  // the class's vtable
extern void* g_unk_7101a21718[];  // the base class's vtable

struct UnkAnimatedNodeDef_ovb0138 {
    char* vtable;
    UnkStrIdMember name;                         // 0x8
    u64 field_0x10;
    base::UnkVector<UnkStrIdMember> bones;        // 0x18
    base::UnkVector<UnkStrIdMember> hierarchies;  // 0x40
    bool launch_events;                           // 0x68
    bool allow_root_motion;                       // 0x69
};
static_assert(__builtin_offsetof(UnkAnimatedNodeDef_ovb0138, launch_events) == 0x68,
              "UnkAnimatedNodeDef_ovb0138 layout");

void fn_71002d3d10(UnkAnimatedNodeDef_ovb0138* self) {
    self->vtable = reinterpret_cast<char*>(g_unk_7101a216e8) + 0x10;
    self->hierarchies.Free();
    self->bones.Free();
    self->vtable = reinterpret_cast<char*>(g_unk_7101a21718) + 0x10;
    self->name.~UnkStrIdMember();
}

// Copy construction: the ids are retained and the arrays copied element by element.
void fn_71002d4108(void* dst, const void* src) {
    UnkAnimatedNodeDef_ovb0138* self = static_cast<UnkAnimatedNodeDef_ovb0138*>(dst);
    const UnkAnimatedNodeDef_ovb0138* other = static_cast<const UnkAnimatedNodeDef_ovb0138*>(src);
    self->vtable = reinterpret_cast<char*>(g_unk_7101a21718) + 0x10;
    new (&self->name) CStrId(reinterpret_cast<const CStrId&>(other->name));
    self->vtable = reinterpret_cast<char*>(g_unk_7101a216e8) + 0x10;
    self->field_0x10 = other->field_0x10;
    new (&self->bones) base::UnkStrIdVec(reinterpret_cast<const base::UnkStrIdVec&>(other->bones));
    new (&self->hierarchies) base::UnkStrIdVec(reinterpret_cast<const base::UnkStrIdVec&>(other->hierarchies));
    __builtin_memcpy(&self->launch_events, &other->launch_events, 2);
}

// Move assignment: the name is copied, the arrays taken over.
void* fn_71002d44dc(void* dst, const void* src) {
    UnkAnimatedNodeDef_ovb0138* self = static_cast<UnkAnimatedNodeDef_ovb0138*>(dst);
    UnkAnimatedNodeDef_ovb0138* other =
        const_cast<UnkAnimatedNodeDef_ovb0138*>(static_cast<const UnkAnimatedNodeDef_ovb0138*>(src));
    if (self != other) {
        self->name = other->name;
        self->field_0x10 = other->field_0x10;
        self->bones = static_cast<base::UnkVector<UnkStrIdMember>&&>(other->bones);
        self->hierarchies = static_cast<base::UnkVector<UnkStrIdMember>&&>(other->hierarchies);
        __builtin_memcpy(&self->launch_events, &other->launch_events, 2);
    }
    return self;
}
