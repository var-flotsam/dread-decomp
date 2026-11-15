// Unit 365 (navmesh::CNavMeshGenerator): creates a 0x188-byte object, retains it, appends it to the
// owner's list, then sets its owner (0x10), name (0x20) and an index taken from the name's data (0x18).
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/logic/collision/CCollider.h"
#include "types.h"

using base::global::CStrId;

struct UnkObj_l289ec0 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();  // adds a reference

    u8 unk_0x8[0x8];
    void* owner;  // 0x10
    u64 index;    // 0x18
    CStrId name;  // 0x20
};

struct UnkNode_l289ec0 {
    UnkObj_l289ec0* obj;
    // A union member: the original reloads the list count after storing it.
    union {
        UnkNode_l289ec0* next;
    };
};

struct UnkList_l289ec0 {
    u8 unk_0x0[0x40];
    u32 count;
    UnkNode_l289ec0* head;
    UnkNode_l289ec0* tail;
    base::UnkAllocator* allocator;
    u32 flags;
    u8 unk_0x64[4];
    u32 obj_flags;  // 0x68
};

struct UnkNameData_l289ec0 {
    u8 pad_0x0[0x30];
    u64 index;  // 0x30
};

struct UnkNameRef_l289ec0 {
    CStrId name;
    UnkNameData_l289ec0* Data() const { return *reinterpret_cast<UnkNameData_l289ec0* const*>(&name); }
};

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
UnkObj_l289ec0* fn_7100289ec0(UnkList_l289ec0* self, void* owner, const UnkNameRef_l289ec0* ref) {
    UnkObj_l289ec0* obj = static_cast<UnkObj_l289ec0*>(fn_71000839a0(0x188, self->obj_flags));
    fn_7100d36624(reinterpret_cast<UnkObj_71007e44b8*>(obj));
    if (obj) {
        obj->vf_0x18();
        UnkNode_l289ec0* node =
            static_cast<UnkNode_l289ec0*>(self->allocator->vf_0x40(sizeof(UnkNode_l289ec0), 8, self->flags));
        if (node) {
            node->obj = obj;
            node->next = nullptr;
            if (self->count == 0) {
                self->head = node;
                self->tail = node;
            } else {
                self->tail->next = node;
                node->next = nullptr;
                self->tail = node;
            }
            ++self->count;
        }
    }
    obj->owner = owner;
    obj->name = ref->name;
    obj->index = ref->Data()->index;
    return obj;
}
