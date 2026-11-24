// CAutofocusAimCameraCtrl unit (tools/tu_map.py unit 1150): takes every CAutofocusAimCameraCtrl out of the
// camera list at +0x58 (count, head, tail, allocator). When the one removed is the active (front) one, it is
// stopped first, finished cameras behind it are dropped too, and the next one is started unless both carry
// the id table's id at +0x1d30 and it has its +0x18 set. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CCameraCtrl.h"
#include "game/UnkGameManagerIdTable.h"
#include "types.h"

using base::global::CStrId;

struct UnkCam_l723120 {
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

struct UnkCamNode_l723120 {
    UnkCam_l723120* value;
    UnkCamNode_l723120* next;
};

struct UnkCameraOwner_l723120 {
    u8 pad_0x0[0x40];
    void* field_0x40;
    u8 pad_0x48[0x10];
    u32 count;  // 0x58
    UnkCamNode_l723120* head;
    UnkCamNode_l723120* tail;
    base::IAllocator* allocator;
};

// The id table's slot at +0x1d30 (UnkGameManagerIdTable does not name it yet).
struct UnkIdTableSlot1d30_l723120 {
    u8 pad_0x0[0x1d30];
    CStrId field_0x1d30;
};

// Signature as src/game/CChozoCommanderAIComponent/CChozoCommanderAIComponent_DropCameras_l450950.cpp declares it.
struct UnkCam_l450950;

// CAutofocusAimCameraCtrl's class (what fn_7100720464 returns).
extern __attribute__((visibility("hidden"))) base::reflection::CClass g_unk_717270f9b8;

// Whether the camera is a CAutofocusAimCameraCtrl.
static inline bool IsAutofocusAim(UnkCam_l723120* camera) {
    const base::reflection::CClass* cls = camera->vf_0x0();
    fn_7100720464();
    return cls == &g_unk_717270f9b8;
}

// Unlinks `node` and frees it; the node after it, or null when it is not in the list.
static inline UnkCamNode_l723120* Erase(UnkCameraOwner_l723120* self, UnkCamNode_l723120* node) {
    UnkCamNode_l723120* prev = nullptr;
    UnkCamNode_l723120* cur = self->head;
    UnkCamNode_l723120* next;
    UnkCamNode_l723120* last = nullptr;
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

void CAutofocusAimCameraCtrl_RemoveAllFromCameraList(void* owner) {
    UnkCameraOwner_l723120* self = static_cast<UnkCameraOwner_l723120*>(owner);
    UnkCamNode_l723120* node = self->head;
    while (node) {
        UnkCam_l723120* camera = node->value;
        if (!camera || !IsAutofocusAim(camera)) {
            node = node->next;
            continue;
        }
        if (self->head != node) {
            node = Erase(self, node);
            continue;
        }
        node->value->vf_0x38();
        UnkCam_l723120* stopped = node->value;
        const UnkIdTableSlot1d30_l723120& table =
            reinterpret_cast<const UnkIdTableSlot1d30_l723120&>(GameManagerIdTable());
        bool was_default = stopped->id.data == table.field_0x1d30.data;
        node = Erase(self, node);
        while (node && node->value->vf_0x60())
            node = Erase(self, node);
        if (node && !node->value->vf_0x60()) {
            UnkCam_l723120* next = node->value;
            if (!was_default || !next->field_0x18)
                fn_71010ed948(reinterpret_cast<UnkCam_l450950*>(next), self->field_0x40, true);
        }
    }
}
