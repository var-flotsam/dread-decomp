// CCameraToRailLogicAction unit (tools/tu_map.py): takes every CRailCameraCtrl out of the camera list at
// +0x58 (count, head, tail, allocator). When the one removed is the active (front) one, it is stopped first,
// finished cameras behind it are dropped too, and the next one is started unless both carry the id table's
// id at +0x1d30 and it has its +0x18 set. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CCameraCtrl.h"
#include "game/UnkGameManagerIdTable.h"
#include "types.h"

using base::global::CStrId;

struct UnkCam_la4dffc {
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

struct UnkCamNode_la4dffc {
    UnkCam_la4dffc* value;
    UnkCamNode_la4dffc* next;
};

struct UnkCameraHolder_la4dffc {
    u8 pad_0x0[0x40];
    void* field_0x40;
    u8 pad_0x48[0x10];
    u32 count;  // 0x58
    UnkCamNode_la4dffc* head;
    UnkCamNode_la4dffc* tail;
    base::IAllocator* allocator;
};

// The id table's slot at +0x1d30 (UnkGameManagerIdTable does not name it yet).
struct UnkIdTableSlot1d30_la4dffc {
    u8 pad_0x0[0x1d30];
    CStrId field_0x1d30;
};

// Signature as src/game/CChozoCommanderAIComponent/CChozoCommanderAIComponent_DropCameras_l450950.cpp declares it.
struct UnkCam_l450950;

// Unlinks `node` and frees it; the node after it, or null when it is not in the list.
static inline UnkCamNode_la4dffc* Erase(UnkCameraHolder_la4dffc* self, UnkCamNode_la4dffc* node) {
    UnkCamNode_la4dffc* prev = nullptr;
    UnkCamNode_la4dffc* cur = self->head;
    UnkCamNode_la4dffc* next;
    UnkCamNode_la4dffc* last = nullptr;
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

void fn_7100a4dffc(void* holder) {
    UnkCameraHolder_la4dffc* self = static_cast<UnkCameraHolder_la4dffc*>(holder);
    UnkCamNode_la4dffc* node = self->head;
    while (node) {
        UnkCam_la4dffc* camera = node->value;
        if (!camera || camera->vf_0x0() != fn_71009d6164()) {
            node = node->next;
            continue;
        }
        if (self->head != node) {
            node = Erase(self, node);
            continue;
        }
        node->value->vf_0x38();
        UnkCam_la4dffc* stopped = node->value;
        const UnkIdTableSlot1d30_la4dffc& table =
            reinterpret_cast<const UnkIdTableSlot1d30_la4dffc&>(GameManagerIdTable());
        bool was_default = stopped->id.data == table.field_0x1d30.data;
        node = Erase(self, node);
        while (node && node->value->vf_0x60())
            node = Erase(self, node);
        if (node && !node->value->vf_0x60()) {
            UnkCam_la4dffc* next = node->value;
            if (!was_default || !next->field_0x18)
                fn_71010ed948(reinterpret_cast<UnkCam_l450950*>(next), self->field_0x40, true);
        }
    }
}
