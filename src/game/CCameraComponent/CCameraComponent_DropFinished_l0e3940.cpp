// CCameraComponent (unit 2691): drops the finished cameras at the front of the list at +0x58 (count, head,
// tail, allocator), stopping the first one, and starts the first unfinished one behind them.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/CCameraCtrl.h"
#include "types.h"

struct UnkCamListed_l0e3940 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();  // stops the camera
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual bool vf_0x60();  // whether it has finished
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
};

struct UnkCamNode_l0e3940 {
    UnkCamListed_l0e3940* value;
    UnkCamNode_l0e3940* next;
};

struct UnkCamComponent_l0e3940 {
    u8 pad_0x0[0x40];
    void* field_0x40;
    u8 pad_0x48[0x10];
    u32 count;  // 0x58
    UnkCamNode_l0e3940* head;
    UnkCamNode_l0e3940* tail;
    base::IAllocator* allocator;
};

// Unlinks `node` and frees it; the node after it, or null when it is not in the list.
static inline UnkCamNode_l0e3940* Erase(UnkCamComponent_l0e3940* self, UnkCamNode_l0e3940* node) {
    UnkCamNode_l0e3940* prev = nullptr;
    UnkCamNode_l0e3940* cur = self->head;
    UnkCamNode_l0e3940* next;
    UnkCamNode_l0e3940* last = nullptr;
    for (;;) {
        if (!cur)
            return nullptr;
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
    return next;
}

void fn_71010e3940(UnkCamComponent_l0e3940* self) {
    bool removed = false;
    UnkCamNode_l0e3940* node = self->head;
    while (node) {
        UnkCamListed_l0e3940* camera = node->value;
        if (!camera) {
            node = Erase(self, node);
            continue;
        }
        if (!camera->vf_0x60()) {
            if (removed)
                fn_71010ed948(reinterpret_cast<UnkCam_l450950*>(camera), self->field_0x40, true);
            return;
        }
        if (!removed)
            camera->vf_0x38();
        node->value->vf_0x78();
        node = Erase(self, node);
        removed = true;
    }
}
