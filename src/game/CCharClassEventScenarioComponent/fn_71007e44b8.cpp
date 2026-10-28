// Creates a 0x188-byte object, retains it and appends it to the owner's list, then sets its
// owner pointer (0x10), name (0x20) and an index (0x18). Returns the object.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkObj_71007e44b8 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();  // adds a reference

    u8 unk_0x8[0x8];
    void* owner;  // 0x10
    u64 index;    // 0x18
    CStrId name;  // 0x20
};

void fn_7100d36624(UnkObj_71007e44b8* self);  // constructor

struct UnkNode_71007e44b8 {
    UnkObj_71007e44b8* obj;
    // A union member: the original reloads the list count after storing it.
    union {
        UnkNode_71007e44b8* next;
    };
};

struct UnkStruct_71007e44b8 {
    u8 unk_0x0[0x40];
    u32 count;
    UnkNode_71007e44b8* head;
    UnkNode_71007e44b8* tail;
    base::UnkAllocator* allocator;
    u32 flags;
    u8 unk_0x64[4];
    u32 obj_flags;  // 0x68
};

UnkObj_71007e44b8* fn_71007e44b8(UnkStruct_71007e44b8* self, void* owner, const CStrId* name, u32 index) {
    UnkObj_71007e44b8* obj = static_cast<UnkObj_71007e44b8*>(fn_71000839a0(0x188, self->obj_flags));
    fn_7100d36624(obj);
    if (obj) {
        obj->vf_0x18();
        UnkNode_71007e44b8* node =
            static_cast<UnkNode_71007e44b8*>(self->allocator->vf_0x40(sizeof(UnkNode_71007e44b8), 8, self->flags));
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
    u64 idx = index;
    obj->owner = owner;
    obj->name = *name;
    obj->index = idx;
    return obj;
}
