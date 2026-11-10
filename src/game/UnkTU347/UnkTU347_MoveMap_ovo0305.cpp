// Unit 347: move-assignment of a named map of bound objects: takes the other's name and settings,
// rebinds each object to this map (re-inserting it by its key), then empties the other map.
// Names and layouts are placeholders.
#include <string.h>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::global::CStrId;

struct UnkBoundObject_b260;

struct UnkBoundObject_b260 {
    u8 unk_0x0[0x30];
    CStrId key;   // 0x30
    u8 unk_0x38[0x90];
    void* owner;  // 0xc8
    u8 unk_0xd0[0xe5];
    u8 detached;  // 0x1b5
};

struct UnkIdNode_b260 {
    CStrId id;                      // 0x0
    UnkBoundObject_b260* object;    // 0x8
    UnkIdNode_b260* next;           // 0x10
    UnkIdNode_b260* prev;           // 0x18
    UnkIdNode_b260* chain;          // 0x20
};

struct UnkIdMap_b260 {
    base::UnkAllocator* allocator;  // 0x0
    u32 flags;                      // 0x8
    u32 count;                      // 0xc
    UnkIdNode_b260** buckets;       // 0x10
    u32 used;                       // 0x18
    u32 bucket_count;               // 0x1c
    u8 unk_0x20[0x18];
    UnkIdNode_b260* tail;           // 0x38
    UnkIdNode_b260* head;           // 0x40
    u64 field_0x48;
};

struct UnkBoundMap_b260 {
    void* vtable;                   // 0x0
    CStrId name;                    // 0x8
    UnkIdMap_b260 map;              // 0x10
};
static_assert(__builtin_offsetof(UnkBoundMap_b260, map.head) == 0x50, "layout");


UnkBoundMap_b260* fn_71002632a8(UnkBoundMap_b260* self, UnkBoundMap_b260* other) {
    self->name.ReleaseAndTake(other->name);
    self->map.field_0x48 = other->map.field_0x48;
    for (UnkIdNode_b260* node = other->map.head; node; node = node->next) {
        UnkBoundObject_b260* object = node->object;
        if (object->owner == other)
            object->owner = nullptr;
        object->owner = self;
        UnkIdNode_b260** slot = nullptr;
        if (self->map.used) {
            u32* words = reinterpret_cast<u32*>(object->key.data);
            u32 lo = words[0x30 / 4], hi = words[0x34 / 4];
            u32 index = (hi ^ lo) % self->map.bucket_count;
            slot = self->map.buckets + index;
        }
        UnkIdNode_b260* chain = *slot;
        UnkIdNode_b260* created = static_cast<UnkIdNode_b260*>(self->map.allocator->vf_0x40(0x28, 8, self->map.flags));
        if (created) {
            created->id.data = object->key.data;
            if (&g_unk_7101d47108->empty != created->id.data)
                ++created->id.data->ref_count;
            created->object = object;
            created->next = nullptr;
            created->prev = nullptr;
            created->chain = nullptr;
        }
        created->chain = chain;
        *slot = created;
        UnkIdNode_b260** link = &self->map.head;
        if (self->map.head) {
            if (self->map.tail) {
                // The tail is read again after the store (the original reloads it).
                UnkIdNode_b260* tail = self->map.tail;
                __builtin_memcpy(&created->prev, &tail, sizeof(tail));
                link = &self->map.tail->next;
                *link = created;
            }
        } else {
            *link = created;
        }
        self->map.tail = created;
        ++self->map.count;
    }
    if (other->map.count) {
        UnkIdNode_b260* node = other->map.head;
        while (node) {
            UnkIdNode_b260* next = node->next;
            node->id.Reset();
            other->map.allocator->vf_0x48(node);
            node = next;
        }
        memset(other->map.buckets, 0, other->map.bucket_count * u64(8));
        other->map.count = 0;
        other->map.tail = nullptr;
        other->map.head = nullptr;
    }
    return self;
}
