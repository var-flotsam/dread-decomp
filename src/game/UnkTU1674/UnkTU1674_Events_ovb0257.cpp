// Unit 1674 of tools/tu_map.py (no name hint; with src/game/UnkUnit1674/UnkUnit1674_Push.cpp, which pushes items
// onto the lists of its map). A chained map from an event kind to a list of handlers {handle, mask,
// guard}, its nodes also linked in insertion order (head +0x40, tail +0x38). fn_7100b1e0d0 destroys
// every handler and list and empties the map; fn_7100b1e3fc runs the handlers whose mask meets the event's, dropping the ones whose guard
// says they are done. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

struct UnkEvent_b257f;

struct UnkObj_b257f {
    virtual void vf_0x0();
    virtual void vf_0x8();
};

struct UnkHandle_b257f {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(UnkEvent_b257f* event);
};

struct UnkGuard_b257f {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual bool vf_0x10();
};

struct UnkItem_b257f {
    UnkHandle_b257f* handle;  // 0x0
    u32 mask;  // 0x8
    UnkGuard_b257f* guard;  // 0x10
};

struct UnkNode_b257f {
    UnkItem_b257f* item;
    UnkNode_b257f* next;
};

struct UnkList_b257f {
    void** vtable;
    // A union member: no type-based alias info, so the count is reloaded after the links are stored.
    union {
        u32 count;  // 0x8
    };
    UnkNode_b257f* head;  // 0x10
    UnkNode_b257f* tail;  // 0x18
    base::UnkAllocator* allocator;  // 0x20
    u32 flags;  // 0x28
};

struct UnkMapNode_b257f {
    u32 key;
    UnkList_b257f* value;  // 0x8
    UnkMapNode_b257f* next_all;  // 0x10
    UnkMapNode_b257f* prev_all;  // 0x18
    UnkMapNode_b257f* next;  // 0x20: the bucket's chain
};

struct UnkMap_b257f {
    base::UnkAllocator* allocator;
    u32 flags;  // 0x8
    u32 count;  // 0xc
    UnkMapNode_b257f** buckets;  // 0x10
    u32 field_0x18;
    u32 bucket_count;  // 0x1c
    u8 pad_0x20[0x18];
    UnkMapNode_b257f* tail;  // 0x38
    UnkMapNode_b257f* head;  // 0x40

    __attribute__((always_inline)) UnkMapNode_b257f** Bucket(u32 key) {
        if (!field_0x18)
            return nullptr;
        u32 index = key % bucket_count;
        return &buckets[index];
    }
};

struct UnkEventSource_b257f {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkEvent_b257f {
    UnkEventSource_b257f* source;
    u32 key;  // 0x8
    u8 pad_0xc[0xe];
    u8 handled;  // 0x1a
    u8 pad_0x1b[5];
    u64 mask;  // 0x20
};

static inline __attribute__((always_inline)) void DestroyItem_b257f(UnkItem_b257f* item) {
    if (item->handle) {
        item->handle->vf_0x8();
        item->handle = nullptr;
    }
    if (item->guard)
        item->guard->vf_0x8();
    fn_7100083c00(item);
}

void fn_7100b1e0d0(UnkMap_b257f* self) {
    for (UnkMapNode_b257f* node = self->head; node; node = node->next_all) {
        UnkList_b257f* list = node->value;
        for (UnkNode_b257f* it = list->head; it; it = it->next) {
            if (it->item) {
                DestroyItem_b257f(it->item);
                it->item = nullptr;
            }
        }
        for (UnkNode_b257f* it = list->head; it;) {
            UnkNode_b257f* next = it->next;
            list->allocator->vf_0x48(it);
            it = next;
        }
        list->count = 0;
        list->head = nullptr;
        list->tail = nullptr;
        reinterpret_cast<UnkObj_b257f*>(list)->vf_0x8();
    }
    if (self->count) {
        for (UnkMapNode_b257f* node = self->head; node;) {
            UnkMapNode_b257f* next = node->next_all;
            self->allocator->vf_0x48(node);
            node = next;
        }
        __builtin_memset(self->buckets, 0, (u64)self->bucket_count * 8);
        self->count = 0;
        self->tail = nullptr;
        self->head = nullptr;
    }
}

void fn_7100b1e3fc(UnkMap_b257f* self, UnkEvent_b257f* event) {
    if (event->handled)
        return;
    if (!event->source || !event->source->field_0x10)
        return;
    if (!self->field_0x18)
        return;
    UnkMapNode_b257f** slot = self->Bucket(event->key);
    if (!slot)
        return;
    UnkMapNode_b257f* found = *slot;
    while (found && found->key != event->key)
        found = found->next;
    if (!found)
        return;
    UnkList_b257f* list = found->value;
    if (!list)
        return;
    UnkNode_b257f* it = list->head;
    while (it) {
        UnkItem_b257f* item = it->item;
        if (!item->guard || item->guard->vf_0x10()) {
            if (event->mask & item->mask)
                item->handle->vf_0x10(event);
            if (event->handled)
                return;
            if (!event->source || !event->source->field_0x10)
                return;
            it = it->next;
            continue;
        }
        if (item)
            DestroyItem_b257f(item);
        UnkNode_b257f* next = list->head;
        UnkNode_b257f* previous = nullptr;
        for (;;) {
            if (!next)
                return;
            UnkNode_b257f* current = next;
            next = current->next;
            if (current == it)
                break;
            previous = current;
        }
        if (previous) {
            previous->next = next;
            if (list->tail == it)
                list->tail = previous;
        } else {
            list->head = next;
        }
        if (--list->count == 0) {
            list->head = nullptr;
            list->tail = nullptr;
        }
        list->allocator->vf_0x48(it);
        it = next;
    }
}
