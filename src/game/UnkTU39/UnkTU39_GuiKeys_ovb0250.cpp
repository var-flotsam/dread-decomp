// Unit 39 of tools/tu_map.py (no reliable name): the keyed list the GUI root keeps at +0xca8 (game/UnkGuiRoot.h's
// UnkGuiKeyList; nodes of {string id, float, next}). fn_710002fd10 adds a key, or sets the float of the node it
// has to -1.0f; fn_710002fe64 sets the float of a key's node (adding the node when it is missing). Both tell the
// callback set at +0x30 (with true) when the list was empty. Names and layouts are placeholders.
#include <new>

#include "base/UnkCloneHelpers.h"
#include "base/UnkDeferredSet.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkTU39.h"

using base::UnkDeferredCallback;
using base::UnkDeferredSet;
using base::global::CStrId;

struct UnkKeyNode_b0250 {
    CStrId id;
    union {
        f32 value;
        u32 value_bits;
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
    UnkDeferredSet<UnkDeferredCallback<bool>> on_change;  // 0x30
};

void fn_710002fd10(UnkGuiKeyList* list, CStrId* key) {
    UnkKeyList_b0250* self = reinterpret_cast<UnkKeyList_b0250*>(list);
    u32 old_count = self->count;
    UnkKeyNode_b0250* found = self->head;
    for (; found; found = found->next) {
        if (found->id.data == key->data)
            break;
    }
    if (found) {
        found->value_bits = 0xbf800000;
    } else {
        CStrId tmp(*key);
        void* mem = self->allocator->vf_0x40(sizeof(UnkKeyNode_b0250), 8, self->tag);
        if (mem) {
            UnkKeyNode_b0250* node = static_cast<UnkKeyNode_b0250*>(mem);
            node->id.data = tmp.data;
            tmp.data = &g_unk_7101d47108->empty;
            node->value_bits = 0xbf800000;
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
    if (old_count == 0) {
        bool arg = true;
        fn_71000303c0(&self->on_change, &arg);
    }
}

void fn_710002fe64(UnkGuiKeyList* list, CStrId* key, f32 value) {
    UnkKeyList_b0250* self = reinterpret_cast<UnkKeyList_b0250*>(list);
    u32 old_count = self->count;
    UnkKeyNode_b0250* found = self->head;
    for (; found; found = found->next) {
        if (found->id.data == key->data)
            break;
    }
    if (found) {
        found->value = value;
    } else {
        CStrId tmp(*key);
        void* mem = self->allocator->vf_0x40(sizeof(UnkKeyNode_b0250), 8, self->tag);
        if (mem) {
            UnkKeyNode_b0250* node = static_cast<UnkKeyNode_b0250*>(mem);
            node->id.data = tmp.data;
            tmp.data = &g_unk_7101d47108->empty;
            node->value = value;
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
    if (old_count == 0) {
        bool arg = true;
        fn_71000303c0(&self->on_change, &arg);
    }
}
