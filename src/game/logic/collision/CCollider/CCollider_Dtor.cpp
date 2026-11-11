// A destructor in the game::logic::collision::CCollider unit: fn_7100d3b4c8 releases the object at +0x158
// (owned when +0x160 is set: its slot 0x10 and then the collider's own slot 0x38; else unlinked through
// fn_7100d35a74), clears +0x164, empties the node list at +0x128 (nodes freed through the allocator at
// +0x148), then runs the base destructor fn_7100d35944. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/logic/collision/CBroadphaseObject.h"
#include "types.h"

struct UnkColliderListNode {
    void* value;
    UnkColliderListNode* next;
};

struct UnkColliderReleasable {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
};

struct UnkColliderDtor {
    void** vtable;
    u8 pad_0x8[0x128 - 0x8];
    void** list_vtable;  // 0x128
    u32 count;           // 0x130
    UnkColliderListNode* head;  // 0x138
    UnkColliderListNode* tail;  // 0x140
    base::UnkAllocator* allocator;  // 0x148
    u8 pad_0x150[0x8];
    UnkColliderReleasable* field_0x158;
    bool field_0x160;
    u32 field_0x164;

    void vf_0x38() { reinterpret_cast<void (*)(UnkColliderDtor*)>(vtable[7])(this); }
};

extern char g_unk_7101aa85b0[];
extern char g_unk_7101aa8660[];

void fn_7100d3b4c8(UnkColliderDtor* self) {
    self->vtable = reinterpret_cast<void**>(g_unk_7101aa85b0 + 0x10);
    if (self->field_0x160) {
        if (self->field_0x158) {
            self->field_0x158->vf_0x10();
            self->field_0x158 = nullptr;
            self->field_0x160 = false;
            self->vf_0x38();
        }
    } else if (self->field_0x158) {
        self->field_0x158 = nullptr;
        fn_7100d35a74(self);
    }
    self->field_0x164 = 0;
    self->list_vtable = reinterpret_cast<void**>(g_unk_7101aa8660 + 0x10);
    for (UnkColliderListNode* n = self->head; n;) {
        UnkColliderListNode* next = n->next;
        self->allocator->vf_0x48(n);
        n = next;
    }
    self->count = 0;
    self->head = nullptr;
    self->tail = nullptr;
    fn_7100d35944(self);
}
