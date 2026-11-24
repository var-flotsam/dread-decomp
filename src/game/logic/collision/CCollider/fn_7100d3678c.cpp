// CCollider's unit (2503): the broadphase object's move constructor, on the game object base (fn_7101043904):
// the vtable, the fields and name id taken over (the source's id left empty), the boxes and transforms copied,
// and the handler list copied node by node; the source's two handles are cleared last.
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/UnkVtableList.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CGameObject.h"
#include "game/logic/collision/CBroadphaseObject.h"
#include "types.h"

using base::global::UnkStringData;

struct UnkBroadphaseHandler;

struct UnkHandlerEntry_ld3678c {
    UnkBroadphaseHandler* handler;
    u32 mask;
};

struct UnkHandlerNode_ld3678c {
    UnkHandlerEntry_ld3678c entry;
    UnkHandlerNode_ld3678c* next;  // 0x10

    UnkHandlerNode_ld3678c(const UnkHandlerEntry_ld3678c& e) : entry(e), next(nullptr) {}
};

// The handler list (a vtable list of handler entries).
struct UnkHandlerList_ld3678c : base::UnkVtableList<UnkHandlerNode_ld3678c> {
    // Copies another list's allocator and nodes (nodes that cannot be allocated are left out).
    UnkHandlerList_ld3678c(const UnkHandlerList_ld3678c& other) {
        vtable = g_unk_7101aa8590 + 0x10;
        count = 0;
        head = nullptr;
        tail = nullptr;
        allocator = other.allocator;
        flags = other.flags;
        for (UnkHandlerNode_ld3678c* node = other.head; node; node = node->next) {
            void* mem = allocator->vf_0x40(sizeof(UnkHandlerNode_ld3678c), 8, flags);
            if (mem)
                PushBack(new (mem) UnkHandlerNode_ld3678c(node->entry));
        }
    }
};

struct UnkBox_ld3678c {
    f32 min_x;
    f32 min_y;
    f32 max_x;
    f32 max_y;
};

struct UnkBroadphaseObject_ld3678c {
    char* vtable;
    u8 pad_0x8[0x8];
    u64 field_0x10;
    s64 field_0x18;
    UnkStringData* name;  // 0x20
    bool field_0x28;
    u8 pad_0x29[7];
    u64 field_0x30;
    s32 field_0x38;
    bool field_0x3c;
    u8 pad_0x3d[3];
    UnkBox_ld3678c box_0x40;
    UnkBox_ld3678c box_0x50;
    bool field_0x60;
    u8 pad_0x61[3];
    u32 field_0x64;
    bool field_0x68;
    bool field_0x69;
    bool field_0x6a;
    bool field_0x6b;
    UnkMatrix44 xform;       // 0x6c
    UnkMatrix44 prev_xform;  // 0xac
    u8 pad_0xec[4];
    UnkHandlerList_ld3678c handlers;  // 0xf0
    bool busy;                        // 0x120
};
static_assert(__builtin_offsetof(UnkBroadphaseObject_ld3678c, busy) == 0x120, "layout");

static inline void CopyBox(UnkBox_ld3678c& dst, const UnkBox_ld3678c& src) {
    dst.min_x = src.min_x;
    dst.min_y = src.min_y;
    dst.max_x = src.max_x;
    dst.max_y = src.max_y;
}

void CCollider_MoveConstructBroadphaseObject(UnkBroadphaseObject_ld3678c* self, UnkBroadphaseObject_ld3678c* other) {
    fn_7101043904(self);
    self->vtable = g_unk_7101aa84e0 + 0x10;
    self->field_0x10 = other->field_0x10;
    self->field_0x18 = other->field_0x18;
    self->name = other->name;
    other->name = &g_unk_7101d47108->empty;
    self->field_0x28 = other->field_0x28;
    self->field_0x30 = other->field_0x30;
    self->field_0x38 = other->field_0x38;
    self->field_0x3c = other->field_0x3c;
    CopyBox(self->box_0x40, other->box_0x40);
    CopyBox(self->box_0x50, other->box_0x50);
    self->field_0x60 = other->field_0x60;
    self->field_0x64 = other->field_0x64;
    self->field_0x68 = other->field_0x68;
    self->field_0x69 = other->field_0x69;
    self->field_0x6a = other->field_0x6a;
    self->field_0x6b = other->field_0x6b;
    UnkCopyMatrix(&self->xform, other->xform);
    UnkCopyMatrix(&self->prev_xform, other->prev_xform);

    // The handler list is copied, not taken over.
    new (&self->handlers) UnkHandlerList_ld3678c(other->handlers);
    self->busy = other->busy;
    other->field_0x10 = 0;
    other->field_0x30 = 0;
}
