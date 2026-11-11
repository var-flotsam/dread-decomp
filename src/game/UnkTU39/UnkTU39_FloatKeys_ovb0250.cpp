// Unit 39 of tools/tu_map.py (no reliable name): a keyed list of (string id, float, float) nodes (vtable, count,
// head, tail, allocator and tag, then a deferred callback set at +0x30 told when the list's value changes: the
// value of the last node, 0 when empty). fn_7100030ce0 clears it; fn_7100030fc0 and fn_7100031140 set the
// values of a key's node (adding it with -1.0f as the second float when it is not there); fn_71000312c4 removes
// a key's node. Names and layouts are placeholders.
#include <new>

#include "base/UnkCloneHelpers.h"
#include "base/UnkDeferredSet.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/UnkTU39.h"

using base::UnkDeferredCallback;
using base::UnkDeferredSet;
using base::global::CStrId;

struct UnkKeyNode_b0250 {
    CStrId id;
    f32 value;
    union {
        f32 value2;
        u32 value2_bits;
    };
    UnkKeyNode_b0250* next;
};

struct UnkKeyList_b0250 {
    void** vtable;
    u32 count;
    UnkKeyNode_b0250* head;
    UnkKeyNode_b0250* tail;
    base::UnkAllocator* allocator;
    u32 tag;
    UnkDeferredSet<UnkDeferredCallback<f32>> on_change;  // 0x30
};

void fn_7100030ce0(void* list) {
    UnkKeyList_b0250* self = static_cast<UnkKeyList_b0250*>(list);
    f32 old = self->count ? self->tail->value : 0.0f;
    UnkKeyNode_b0250* node = self->head;
    if (node) {
        UnkKeyNode_b0250* next = node->next;
        for (;;) {
            node->id.Reset();
            self->allocator->vf_0x48(node);
            if (!next)
                break;
            node = next;
            next = next->next;
        }
    }
    self->count = 0;
    self->head = nullptr;
    self->tail = nullptr;
    f32 zero = 0.0f;
    if (old != 0.0f)
        fn_710003152c(&self->on_change, &zero);
}

void fn_7100030fc0(void* list, const void* key_ptr, f32 value) {
    UnkKeyList_b0250* self = static_cast<UnkKeyList_b0250*>(list);
    const CStrId* key = static_cast<const CStrId*>(key_ptr);
    f32 old = self->count ? self->tail->value : 0.0f;
    UnkKeyNode_b0250* found = self->head;
    for (; found; found = found->next) {
        if (found->id.data == key->data)
            break;
    }
    if (found) {
        found->value = value;
        found->value2_bits = 0xbf800000;
    } else {
        CStrId tmp(*key);
        void* mem = self->allocator->vf_0x40(sizeof(UnkKeyNode_b0250), 8, self->tag);
        if (mem) {
            UnkKeyNode_b0250* node = static_cast<UnkKeyNode_b0250*>(mem);
            node->id.data = tmp.data;
            tmp.data = &g_unk_7101d47108->empty;
            node->value = value;
            node->value2_bits = 0xbf800000;
            node->next = nullptr;
            if (self->count == 0) {
                self->head = node;
                self->tail = node;
                self->count = 1;
            } else {
                UnkKeyNode_b0250* last = self->tail;
                __builtin_memcpy(&last->next, &node, sizeof(node));
                node->next = nullptr;
                self->tail = node;
                self->count = self->count + 1;
            }
        }
    }
    if (old != value)
        fn_710003152c(&self->on_change, &value);
}

void fn_7100031140(void* list, const void* key_ptr, f32 value2, f32 value) {
    UnkKeyList_b0250* self = static_cast<UnkKeyList_b0250*>(list);
    const CStrId* key = static_cast<const CStrId*>(key_ptr);
    f32 old = self->count ? self->tail->value : 0.0f;
    UnkKeyNode_b0250* found = self->head;
    for (; found; found = found->next) {
        if (found->id.data == key->data)
            break;
    }
    if (found) {
        found->value = value;
        found->value2 = value2;
    } else {
        CStrId tmp(*key);
        void* mem = self->allocator->vf_0x40(sizeof(UnkKeyNode_b0250), 8, self->tag);
        if (mem) {
            UnkKeyNode_b0250* node = static_cast<UnkKeyNode_b0250*>(mem);
            node->id.data = tmp.data;
            tmp.data = &g_unk_7101d47108->empty;
            node->value = value;
            node->value2 = value2;
            node->next = nullptr;
            if (self->count == 0) {
                self->head = node;
                self->tail = node;
                self->count = 1;
            } else {
                UnkKeyNode_b0250* last = self->tail;
                __builtin_memcpy(&last->next, &node, sizeof(node));
                node->next = nullptr;
                self->tail = node;
                self->count = self->count + 1;
            }
        }
    }
    if (old != value)
        fn_710003152c(&self->on_change, &value);
}

void fn_71000312c4(void* list, const void* key_ptr) {
    UnkKeyList_b0250* self = static_cast<UnkKeyList_b0250*>(list);
    const CStrId* key = static_cast<const CStrId*>(key_ptr);
    f32 old = self->count ? self->tail->value : 0.0f;
    UnkKeyNode_b0250** head_slot = &self->head;
    UnkKeyNode_b0250* first = *head_slot;
    UnkKeyNode_b0250* node = first;
    for (; node; node = node->next) {
        if (node->id.data == key->data)
            break;
    }
    if (node) {
        UnkKeyNode_b0250* prev;
        UnkKeyNode_b0250* last = nullptr;
        UnkKeyNode_b0250* walk = first;
        bool removed = false;
        while (walk) {
            prev = last;
            last = walk;
            // Keeps the visited node in its own register, apart from the walking pointer.
            asm("" : "+r"(last));
            walk = walk->next;
            if (last == node) {
                removed = true;
                break;
            }
        }
        if (removed) {
            if (prev) {
                __builtin_memcpy(&prev->next, &walk, sizeof(walk));
                if (self->tail == node)
                    self->tail = prev;
            } else {
                __builtin_memcpy(head_slot, &walk, sizeof(walk));
            }
            if (--self->count == 0) {
                head_slot[0] = nullptr;
                head_slot[1] = nullptr;
            }
            node->id.Reset();
            self->allocator->vf_0x48(node);
        }
    }
    f32 now = self->count ? self->tail->value : 0.0f;
    if (old != now)
        fn_710003152c(&self->on_change, &now);
}
