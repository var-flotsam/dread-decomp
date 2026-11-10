// Unit 347: the destructor of a map of bound objects keyed by string ids: each bound object that is still
// attached is detached from the map and removed from the game manager, then the nodes are freed.
// Names and layouts are placeholders.
#include <string.h>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::global::CStrId;

struct UnkBoundObject_b260;
struct UnkRemovedValue_b60;
void fn_7100debc28(UnkGameManager*, UnkRemovedValue_b60*);
void fn_7100242ff8(void* map);
extern void* g_unk_7101a20c98[];  // vtable

struct UnkBoundObject_b260 {
    u8 unk_0x0[0xc8];
    void* owner;  // 0xc8
    u8 unk_0xd0[0xe5];
    u8 detached;  // 0x1b5
};

struct UnkIdNode_b260 {
    CStrId id;                      // 0x0
    UnkBoundObject_b260* object;    // 0x8
    UnkIdNode_b260* next;           // 0x10
};

struct UnkIdMap_b260 {
    base::UnkAllocator* allocator;  // 0x0
    u8 unk_0x8[4];
    u32 count;                      // 0xc
    void* buckets;                  // 0x10
    u8 unk_0x18[4];
    u32 bucket_count;               // 0x1c
    u8 unk_0x20[0x18];
    UnkIdNode_b260* tail;           // 0x38
    UnkIdNode_b260* head;           // 0x40
};

struct UnkBoundMap_b260 {
    void* vtable;                   // 0x0
    CStrId name;                    // 0x8
    UnkIdMap_b260 map;              // 0x10
};
static_assert(__builtin_offsetof(UnkBoundMap_b260, map.head) == 0x50, "layout");

void fn_7100262dd4(UnkBoundMap_b260* self) {
    self->vtable = reinterpret_cast<char*>(g_unk_7101a20c98) + 0x10;
    for (UnkIdNode_b260* node = self->map.head; node; node = node->next) {
        UnkBoundObject_b260* object = node->object;
        if (object->detached)
            continue;
        if (object->owner == self)
            object->owner = nullptr;
        fn_7100debc28(g_unk_7101d47310, reinterpret_cast<UnkRemovedValue_b60*>(object));
    }
    if (self->map.count) {
        UnkIdNode_b260* node = self->map.head;
        while (node) {
            UnkIdNode_b260* next = node->next;
            node->id.Reset();
            self->map.allocator->vf_0x48(node);
            node = next;
        }
        memset(self->map.buckets, 0, self->map.bucket_count * u64(8));
        self->map.count = 0;
        self->map.tail = nullptr;
        self->map.head = nullptr;
    }
    fn_7100242ff8(&self->map);
    self->name.Reset();
}
