// From unit 32 of tools/tu_map.py (0x71000255c4-0x7100027774): the destructor of a node of a
// transform hierarchy (0xd0 bytes, embedded by the classes of src/base/UnkTaggedObjectDtors.cpp): the
// parent at +0x0, the first child at +0x8, the previous and next siblings at +0x10 and +0x18, the
// slot at +0xc4 (-1 when detached; the parent counts the negative ones at +0xc8) and a flag at
// +0xcc. It leaves its parent and detaches every child; fn_71000257e0 only leaves the parent (with
// `reset`, after resetting its transform at +0x28 and updating it). Names and layouts are placeholders.
#include "game/UnkTU32.h"
#include "types.h"

struct UnkTransformNode {
    UnkTransformNode* parent;      // +0x0
    UnkTransformNode* first;       // +0x8
    UnkTransformNode* prev;        // +0x10
    UnkTransformNode* next;        // +0x18
    u8 unk_0x20[0xa4];
    s32 slot;                      // +0xc4
    s32 count;                     // +0xc8
    bool attached;                 // +0xcc
};

// Links are written as raw words: the original reloads the node's own links after each one.
static inline void StoreLink(UnkTransformNode** link, UnkTransformNode* value) {
    __builtin_memcpy(link, &value, sizeof(value));
}

static inline void Clear(UnkTransformNode* node) {
    node->parent = nullptr;
    node->attached = false;
    node->prev = nullptr;
    node->next = nullptr;
    node->slot = -1;
}

static inline void Unlink(UnkTransformNode* self) {
    UnkTransformNode* prev = self->prev;
    UnkTransformNode* next = self->next;
    if (prev) {
        StoreLink(&prev->next, next);
        if (UnkTransformNode* after = self->next) {
            UnkTransformNode* before = self->prev;
            StoreLink(&after->prev, before);
        }
    } else {
        if (next)
            StoreLink(&next->prev, prev);
        UnkTransformNode* first = self->next;
        StoreLink(&self->parent->first, first);
    }
}

void fn_710002572c(void* member) {
    UnkTransformNode* self = static_cast<UnkTransformNode*>(member);
    if (self->parent) {
        Unlink(self);
        if (self->slot < 0)
            --self->parent->count;
        Clear(self);
    } else if (!self->first) {
        return;
    }
    for (UnkTransformNode* child = self->first; child;) {
        UnkTransformNode* next = child->next;
        Clear(child);
        child = next;
    }
    self->first = nullptr;
    self->count = 0;
}

// Resets the transform record at +0x28 (base/UnkInitRecord_0x5c.h's layout) and updates the node.
bool fn_7100025bd0(UnkTransformNode* node);

void fn_71000257e0(UnkTransformNode* self, bool reset) {
    if (!self->parent)
        return;
    Unlink(self);
    if (reset) {
        fn_7100025a98(self->unk_0x20 + 0x8);
        fn_7100025bd0(self);
    }
    if (self->slot < 0)
        --self->parent->count;
    Clear(self);
}
