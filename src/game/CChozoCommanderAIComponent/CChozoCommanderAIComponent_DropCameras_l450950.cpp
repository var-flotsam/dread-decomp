// CChozoCommanderAIComponent unit (tools/tu_map.py): takes every CAnimatedCameraCtrl out of the camera list
// at +0x58 (count, head, tail, allocator). When the one removed is the active (front) one, it is stopped
// first, finished cameras behind it are dropped too, and the next one is started unless both carry the
// id table's id at +0x1d30 and it has its +0x18 set. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CCameraCtrl.h"
#include "game/UnkCamNode_l450950.h"
#include "game/UnkCam_l450950.h"
#include "game/UnkGameManagerIdTable.h"
#include "types.h"

using base::global::CStrId;

struct UnkCommanderAI_l450950 {
    u8 pad_0x0[0x40];
    void* field_0x40;
    u8 pad_0x48[0x10];
    u32 count;  // 0x58
    UnkCamNode_l450950* head;
    UnkCamNode_l450950* tail;
    base::UnkAllocator* allocator;
};

// The id table's slot at +0x1d30 (UnkGameManagerIdTable does not name it yet).
struct UnkIdTableSlot1d30_l450950 {
    u8 pad_0x0[0x1d30];
    CStrId field_0x1d30;
};

// Unlinks `node` and frees it; the node after it, or null when it is not in the list.
static inline UnkCamNode_l450950* Erase(UnkCommanderAI_l450950* self, UnkCamNode_l450950* node) {
    UnkCamNode_l450950* prev = nullptr;
    UnkCamNode_l450950* cur = self->head;
    UnkCamNode_l450950* next;
    UnkCamNode_l450950* last = nullptr;
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

void fn_7100450950(UnkCommanderAI_l450950* self) {
    UnkCamNode_l450950* node = self->head;
    while (node) {
        UnkCam_l450950* camera = node->value;
        if (!camera || camera->vf_0x0() != fn_71010e7f90()) {
            node = node->next;
            continue;
        }
        if (self->head != node) {
            node = Erase(self, node);
            continue;
        }
        node->value->vf_0x38();
        UnkCam_l450950* stopped = node->value;
        const UnkIdTableSlot1d30_l450950& table =
            reinterpret_cast<const UnkIdTableSlot1d30_l450950&>(GameManagerIdTable());
        bool was_default = stopped->id.data == table.field_0x1d30.data;
        node = Erase(self, node);
        while (node && node->value->vf_0x60())
            node = Erase(self, node);
        if (node && !node->value->vf_0x60()) {
            UnkCam_l450950* next = node->value;
            if (!was_default || !next->field_0x18)
                fn_71010ed948(next, self->field_0x40, true);
        }
    }
}
