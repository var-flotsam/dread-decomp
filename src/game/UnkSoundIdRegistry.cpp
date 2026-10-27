// The unit of the object that registers sounds under ids and starts them (fn_7100f00af4,
// fn_7100f00f4c, fn_7100f011c4 in src/game_hi/ have the same layouts as local structs): a chained
// hash map of registrations by id at +0, lists of (id, playing instance) nodes at +0x50, +0x80
// and +0xb0, and two items whose colour it sets. Names and layouts are placeholders inferred from
// the code.
#include <string.h>

#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::UnkAllocator;

// A playing instance: reference-counted (atomically, at +8), destroyed through its vtable.
struct UnkSoundIdInstance {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;
    u8 pad_0xc[0x20];
    u32 state;  // 0x2c: 0xa and 0xc once it has finished
};

struct UnkSoundIdRef {
    UnkSoundIdInstance* ptr;

    UnkSoundIdRef(const UnkSoundIdRef& other) : ptr(other.ptr) {
        if (ptr)
            __atomic_fetch_add(&ptr->ref_count, 1, __ATOMIC_SEQ_CST);
    }
    ~UnkSoundIdRef() {
        if (UnkSoundIdInstance* p = ptr) {
            __atomic_fetch_sub(&p->ref_count, 1, __ATOMIC_SEQ_CST);
            if (__atomic_load_n(&p->ref_count, __ATOMIC_SEQ_CST) == 0)
                p->vf_0x10();
        }
    }
};

struct UnkSoundIdListNode {
    u32 key;
    UnkSoundIdRef ref;
    UnkSoundIdListNode* next;
};

struct UnkSoundIdList {
    u32 count;
    UnkSoundIdListNode* head;
    UnkSoundIdListNode* tail;
    UnkAllocator* allocator;
    u32 flags;

    UnkSoundIdListNode* Find(u32 key) {
        for (UnkSoundIdListNode* n = head; n; n = n->next)
            if (n->key == key)
                return n;
        return nullptr;
    }
    // Unlinks and frees `node`; returns the node that followed it (null when it isn't listed).
    UnkSoundIdListNode* Erase(UnkSoundIdListNode* node) {
        UnkSoundIdListNode* cur = nullptr;
        for (UnkSoundIdListNode* it = head; it;) {
            UnkSoundIdListNode* prev = cur;
            cur = it;
            it = it->next;
            if (cur == node) {
                if (prev) {
                    prev->next = it;
                    if (tail == node)
                        tail = prev;
                } else {
                    head = it;
                }
                if (--count == 0) {
                    head = nullptr;
                    tail = nullptr;
                }
                node->~UnkSoundIdListNode();
                allocator->vf_0x48(node);
                return it;
            }
        }
        return nullptr;
    }
};

// A registration (the map's node): the sound's file path and name, flags, the link of the map's
// list of nodes (+0x20) and of its bucket (+0x30).
struct UnkSoundIdEntry {
    u32 key;
    base::UnkStrIdMemberOf<g_unk_7101d47200> path;  // 0x8
    base::UnkStrIdMemberOf<g_unk_7101d47108> name;  // 0x10
    u8 field_0x18;
    bool field_0x19;
    u8 field_0x1a;
    // A union member (no type-based alias info): the original reloads the map's buckets after
    // the store.
    union {
        bool field_0x1b;
    };
    u8 pad_0x1c[0x4];
    UnkSoundIdEntry* list_next;  // 0x20
    u8 pad_0x28[0x8];
    UnkSoundIdEntry* next;       // 0x30
};

// What a coloured item's model holds at +0x78 (fn_710019c564 sets four floats on it).
struct UnkSoundIdItemModel {
    u8 pad_0x0[0x78];
    void* field_0x78;
};
struct UnkSoundIdItem {
    u8 pad_0x0[0x178];
    UnkSoundIdItemModel* model;
};
void fn_710019c564(void* target, f32 a, f32 b, f32 c, f32 d);

static inline void SetColor(UnkSoundIdItem* item, f32 rgb, f32 alpha) {
    fn_710019c564(item->model->field_0x78, rgb, rgb, rgb, alpha);
}

struct UnkSoundIdRegistry {
    UnkAllocator* allocator;  // the map's nodes'
    u32 flags;
    u32 count;
    base::UnkVector<UnkSoundIdEntry*> buckets;  // 0x10
    UnkSoundIdEntry* field_0x38;
    UnkSoundIdEntry* first;  // 0x40: the list of all nodes
    u8 pad_0x48[0x8];
    UnkSoundIdList field_0x50;
    u8 pad_0x78[0x8];
    UnkSoundIdList field_0x80;
    u8 pad_0xa8[0x8];
    UnkSoundIdList field_0xb0;
    u8 pad_0xd8[0x30];
    UnkSoundIdItem* field_0x108;
    UnkSoundIdItem* field_0x110;
    f32 field_0x118;
    f32 field_0x11c;

    UnkSoundIdEntry* Get(u32 key) {
        UnkSoundIdEntry* n = buckets.data[key % buckets.capacity];
        while (n->key != key)
            n = n->next;
        return n;
    }
    // Whether the instance playing under `id` (the one listed at +0x80) is still running.
    bool IsPlaying(u32 id) {
        UnkSoundIdListNode* n = field_0x80.Find(id);
        if (!n)
            return false;
        UnkSoundIdRef ref = n->ref;
        u32 state = ref.ptr->state;
        return state != 0xc && state != 0xa;
    }
};

// Removes every registration: each node's ids are released and the node freed, then the buckets
// are emptied.
void fn_7100f004dc(UnkSoundIdRegistry* self) {
    if (self->count) {
        for (UnkSoundIdEntry* n = self->first; n;) {
            UnkSoundIdEntry* next = n->list_next;
            n->~UnkSoundIdEntry();
            self->allocator->vf_0x48(n);
            n = next;
        }
        memset(self->buckets.data, 0, self->buckets.capacity * 8ul);
        self->count = 0;
        self->field_0x38 = nullptr;
        self->first = nullptr;
    }
}

// NON_MATCHING(minor): block layout and register allocation. The original keeps the destroying call
// of a released instance in IsPlaying out of line (the finished-state test duplicated after it) and,
// in Erase's walk, carries the previous node in x9 and the current one in x23 (ours: x10 and x9, the
// moves after the compare); the same loads, stores and calls in the same order.
// Drops the finished instances from the playing list and sets both items' colour (black and
// opaque when nothing is playing), then clears the flag at +0x1b of every id listed at +0x50.
void fn_7100f00780(UnkSoundIdRegistry* self) {
    if (self->field_0x80.count != 0) {
        for (UnkSoundIdListNode* n = self->field_0x80.head; n;) {
            if (self->IsPlaying(n->key))
                n = n->next;
            else
                n = self->field_0x80.Erase(n);
        }
        SetColor(self->field_0x108, self->field_0x11c, self->field_0x118);
        SetColor(self->field_0x110, self->field_0x11c, self->field_0x118);
    } else {
        SetColor(self->field_0x108, 0.0f, 1.0f);
        SetColor(self->field_0x110, 0.0f, 1.0f);
    }
    for (UnkSoundIdListNode* n = self->field_0x50.head; n; n = n->next)
        self->Get(n->key)->field_0x1b = false;
}
