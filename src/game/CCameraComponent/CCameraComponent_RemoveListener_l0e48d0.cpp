// CCameraComponent (unit 2691): removes an object from the component's list at +0xb0 and then calls
// the object's virtual function 0x48. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "types.h"

struct UnkCamListed_l0e48d0 {
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
};

struct UnkCamNode_l0e48d0 {
    UnkCamListed_l0e48d0* value;
    UnkCamNode_l0e48d0* next;
};

struct UnkCamComponent_l0e48d0 {
    u8 pad_0x0[0xb0];
    u32 count;
    UnkCamNode_l0e48d0* head;
    UnkCamNode_l0e48d0* tail;
    base::IAllocator* allocator;
};

void fn_71010e48d0(UnkCamComponent_l0e48d0* self, UnkCamListed_l0e48d0* object) {
    if (!object)
        return;
    UnkCamNode_l0e48d0* node = self->head;
    for (; node; node = node->next) {
        if (node->value == object)
            break;
    }
    if (node) {
        UnkCamNode_l0e48d0* prev = nullptr;
        UnkCamNode_l0e48d0* cur = self->head;
        UnkCamNode_l0e48d0* next;
        UnkCamNode_l0e48d0* last = nullptr;
        for (;;) {
            if (!cur)
                goto done;
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
    }
done:
    object->vf_0x48();
}
