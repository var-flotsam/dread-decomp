// In unit 437 (CAimComponent's neighbourhood): takes every CMeleeCameraCtrl out of the camera list at
// +0x58 (count, head, tail, allocator), stopping the front one and starting the next as the other camera
// owners do. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CCameraCtrl.h"
#include "game/UnkGameManagerIdTable.h"
#include "types.h"

using base::global::CStrId;

struct UnkCam_l3530d4 {
    virtual const base::reflection::CClass* vf_0x0();
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

    u8 pad_0x8[0x10];
    void* field_0x18;
    CStrId id;  // 0x20
};

struct UnkCamNode_l3530d4 {
    UnkCam_l3530d4* value;
    UnkCamNode_l3530d4* next;
};

struct UnkCameraOwner_l3530d4 {
    u8 pad_0x0[0x40];
    void* field_0x40;
    u8 pad_0x48[0x10];
    u32 count;  // 0x58
    UnkCamNode_l3530d4* head;
    UnkCamNode_l3530d4* tail;
    base::IAllocator* allocator;
};

// The id table's slot at +0x1d30 (UnkGameManagerIdTable does not name it yet).
struct UnkIdTableSlot1d30_l3530d4 {
    u8 pad_0x0[0x1d30];
    CStrId field_0x1d30;
};

// Declared with this type in game/CCameraCtrl.h.
struct UnkCam_l450950;

// Whether the camera is a CMeleeCameraCtrl.
static inline bool IsMeleeCamera(UnkCam_l3530d4* camera) {
    return camera->vf_0x0() == fn_7100927674();
}

// Unlinks `node` and frees it; the node after it, or null when it is not in the list.
static inline UnkCamNode_l3530d4* Erase(UnkCameraOwner_l3530d4* self, UnkCamNode_l3530d4* node) {
    UnkCamNode_l3530d4* prev = nullptr;
    UnkCamNode_l3530d4* cur = self->head;
    UnkCamNode_l3530d4* next;
    UnkCamNode_l3530d4* last = nullptr;
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

void fn_71003530d4(UnkCameraOwner_l3530d4* self) {
    UnkCamNode_l3530d4* node = self->head;
    while (node) {
        UnkCam_l3530d4* camera = node->value;
        if (!camera || !IsMeleeCamera(camera)) {
            node = node->next;
            continue;
        }
        if (self->head != node) {
            node = Erase(self, node);
            continue;
        }
        node->value->vf_0x38();
        UnkCam_l3530d4* stopped = node->value;
        const UnkIdTableSlot1d30_l3530d4& table =
            reinterpret_cast<const UnkIdTableSlot1d30_l3530d4&>(GameManagerIdTable());
        bool was_default = stopped->id.data == table.field_0x1d30.data;
        node = Erase(self, node);
        while (node && node->value->vf_0x60())
            node = Erase(self, node);
        if (node && !node->value->vf_0x60()) {
            UnkCam_l3530d4* next = node->value;
            if (!was_default || !next->field_0x18)
                fn_71010ed948(reinterpret_cast<UnkCam_l450950*>(next), self->field_0x40, true);
        }
    }
}
