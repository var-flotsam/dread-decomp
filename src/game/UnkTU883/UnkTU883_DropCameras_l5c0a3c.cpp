// CHydrogigaAIComponent's unit: takes every CHydrogigaGrabCameraCtrl out of the camera list at +0x58
// (count, head, tail, allocator), stopping the front one and starting the next as the other camera owners
// do. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CCameraCtrl.h"
#include "game/UnkGameManagerIdTable.h"
#include "types.h"

using base::global::CStrId;

struct UnkCam_l5c0a3c {
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

struct UnkCamNode_l5c0a3c {
    UnkCam_l5c0a3c* value;
    UnkCamNode_l5c0a3c* next;
};

struct UnkHydrogigaCameras_l5c0a3c {
    u8 pad_0x0[0x40];
    void* field_0x40;
    u8 pad_0x48[0x10];
    u32 count;  // 0x58
    UnkCamNode_l5c0a3c* head;
    UnkCamNode_l5c0a3c* tail;
    base::UnkAllocator* allocator;
};

// The id table's slot at +0x1d30 (UnkGameManagerIdTable does not name it yet).
struct UnkIdTableSlot1d30_l5c0a3c {
    u8 pad_0x0[0x1d30];
    CStrId field_0x1d30;
};

// Declared with this type in game/CCameraCtrl.h.
struct UnkCam_l450950;

// Whether the camera is a CHydrogigaGrabCameraCtrl.
static inline bool IsGrabCamera(UnkCam_l5c0a3c* camera) {
    return camera->vf_0x0() == fn_710086e570();
}

// Unlinks `node` and frees it; the node after it, or null when it is not in the list.
static inline UnkCamNode_l5c0a3c* Erase(UnkHydrogigaCameras_l5c0a3c* self, UnkCamNode_l5c0a3c* node) {
    UnkCamNode_l5c0a3c* prev = nullptr;
    UnkCamNode_l5c0a3c* cur = self->head;
    UnkCamNode_l5c0a3c* next;
    UnkCamNode_l5c0a3c* last = nullptr;
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

void fn_71005c0a3c(UnkHydrogigaCameras_l5c0a3c* self) {
    UnkCamNode_l5c0a3c* node = self->head;
    while (node) {
        UnkCam_l5c0a3c* camera = node->value;
        if (!camera || !IsGrabCamera(camera)) {
            node = node->next;
            continue;
        }
        if (self->head != node) {
            node = Erase(self, node);
            continue;
        }
        node->value->vf_0x38();
        UnkCam_l5c0a3c* stopped = node->value;
        const UnkIdTableSlot1d30_l5c0a3c& table =
            reinterpret_cast<const UnkIdTableSlot1d30_l5c0a3c&>(GameManagerIdTable());
        bool was_default = stopped->id.data == table.field_0x1d30.data;
        node = Erase(self, node);
        while (node && node->value->vf_0x60())
            node = Erase(self, node);
        if (node && !node->value->vf_0x60()) {
            UnkCam_l5c0a3c* next = node->value;
            if (!was_default || !next->field_0x18)
                fn_71010ed948(reinterpret_cast<UnkCam_l450950*>(next), self->field_0x40, true);
        }
    }
}
