// game::logic::collision::CBroadphaseObject's unit (tools/tu_map.py): the destructor of the object whose handler
// chain fn_7100d35cfc (fn_7100d35cfc.cpp) dispatches to: both vtables set (+0x0 and the second base at +0xf0), the
// handler nodes (handler, mask, next; count +0xf8, head +0x100, tail +0x108) freed through the list's allocator
// (+0x110), the name id at +0x20 released, then the base destructor fn_7101043934. Names and layouts are
// placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/CGameObject.h"
#include "game/logic/collision/CBroadphaseObject.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

extern char g_unk_7101aa84e0[];
extern char g_unk_7101aa8590[];
struct UnkCameraCtrl_ovb0049;

struct UnkBroadphaseHandler;

struct UnkBroadphaseHandlerNode {
    UnkBroadphaseHandler* handler;
    u32 mask;
    UnkBroadphaseHandlerNode* next;
};

struct UnkBroadphaseHandlerOwner {
    char* vtable;
    u8 pad_0x8[0x18];
    UnkStringData* name;  // 0x20
    u8 pad_0x28[0xc8];
    char* vtable_0xf0;
    s32 count;                       // 0xf8
    UnkBroadphaseHandlerNode* head;  // 0x100
    UnkBroadphaseHandlerNode* tail;  // 0x108
    base::UnkAllocator* allocator;   // 0x110
    u32 flags;                       // 0x118
    u8 pad_0x11c[4];
    bool busy;  // 0x120
};
static_assert(__builtin_offsetof(UnkBroadphaseHandlerOwner, busy) == 0x120, "layout");

void fn_7100d35944(UnkBroadphaseHandlerOwner* self) {
    self->vtable = g_unk_7101aa84e0 + 0x10;
    self->vtable_0xf0 = g_unk_7101aa8590 + 0x10;
    for (UnkBroadphaseHandlerNode* node = self->head; node;) {
        UnkBroadphaseHandlerNode* next = node->next;
        self->allocator->vf_0x48(node);
        node = next;
    }
    self->count = 0;
    self->head = nullptr;
    self->tail = nullptr;
    if (UnkStringPool* pool = g_unk_7101d47108) {
        if (self->name && self->name != &pool->empty) {
            --self->name->ref_count;
            if (self->name->ref_count == 0) {
                fn_71000803a4(pool, self->name);
                pool = g_unk_7101d47108;
            }
        }
        self->name = &pool->empty;
    }
    fn_7101043934(reinterpret_cast<UnkCameraCtrl_ovb0049*>(self));
}
