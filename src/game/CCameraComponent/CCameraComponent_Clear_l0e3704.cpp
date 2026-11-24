// CCameraComponent (unit 2691): shuts down and removes every object of the component's list at
// +0x58 (count, head, tail, allocator), freeing each node. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "types.h"

struct UnkCamListed_l0e3704 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
};

struct UnkCamNode_l0e3704 {
    UnkCamListed_l0e3704* value;
    UnkCamNode_l0e3704* next;
};

struct UnkCamComponent_l0e3704 {
    u8 pad_0x0[0x58];
    u32 count;
    UnkCamNode_l0e3704* head;
    UnkCamNode_l0e3704* tail;
    base::UnkAllocator* allocator;
};

void CCameraComponent_RemoveAllQueuedCameras(void* object) {
    UnkCamComponent_l0e3704* self = static_cast<UnkCamComponent_l0e3704*>(object);
    UnkCamNode_l0e3704* node = self->head;
    while (node) {
        if (node->value)
            node->value->vf_0x38();
        if (node->value)
            node->value->vf_0x78();
        UnkCamNode_l0e3704* prev = nullptr;
        UnkCamNode_l0e3704* cur = self->head;
        UnkCamNode_l0e3704* next;
        UnkCamNode_l0e3704* last = nullptr;
        for (;;) {
            if (!cur)
                return;
            prev = last;
            next = cur->next;
            if (cur == node)
                break;
            last = cur;
            cur = next;
        }
        if (prev) {
            prev->next = next;
            if (self->tail == node)
                self->tail = prev;
        } else {
            self->head = next;
        }
        if (--self->count == 0) {
            self->head = nullptr;
            self->tail = nullptr;
        }
        self->allocator->vf_0x48(node);
        node = next;
    }
}
