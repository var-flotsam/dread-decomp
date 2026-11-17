// Unit 2506: removes the entry stored under a string id from the collision manager's list at +0x8
// (count, head, tail, allocator): releases and deletes the entry, then frees its node.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"
#include "base/UnkLinkedList.h"

using base::global::CStrId;

struct UnkManager_7100863a8c;
struct UnkOwner_ld5afa4;
struct UnkOwner_ovb0262a;
void fn_7100d5afa4(UnkOwner_ld5afa4* self);
void fn_7100adffac(UnkOwner_ovb0262a* self);

struct UnkEntry_ld476e4 {
    CStrId id;
};

struct UnkNode_ld476e4 {
    UnkEntry_ld476e4* value;
    UnkNode_ld476e4* next;
};

struct UnkList_ld476e4 : base::UnkLinkedList<UnkNode_ld476e4> {
    void Remove(UnkNode_ld476e4* node);
};

struct UnkManager_ld476e4 {
    u8 pad_0x0[0x8];
    UnkList_ld476e4 entries;  // 0x8
};

static inline UnkNode_ld476e4* Find(UnkNode_ld476e4* node, CStrId id) {
    for (; node; node = node->next) {
        if (node->value && node->value->id.data == id.data)
            break;
    }
    return node;
}

inline void UnkList_ld476e4::Remove(UnkNode_ld476e4* node) {
    UnkNode_ld476e4* prev = nullptr;
    UnkNode_ld476e4* cur = head;
    UnkNode_ld476e4* next;
    UnkNode_ld476e4* last = nullptr;
    for (;;) {
        if (!cur)
            return;
        prev = last;
        next = cur->next;
        if (cur == node)
            break;
        last = cur;
        cur = next;
    }
    if (prev) {
        prev->next = next;
        if (tail == node)
            tail = prev;
    } else {
        head = next;
    }
    if (--count == 0) {
        head = nullptr;
        tail = nullptr;
    }
    allocator->vf_0x48(node);
}

void fn_7100d476e4(UnkManager_7100863a8c* manager, void* obj) {
    UnkManager_ld476e4* self = reinterpret_cast<UnkManager_ld476e4*>(manager);
    CStrId id(*static_cast<const CStrId*>(obj));
    UnkNode_ld476e4* node = Find(self->entries.head, id);
    fn_7100d5afa4(reinterpret_cast<UnkOwner_ld5afa4*>(node->value));
    if (node) {
        if (UnkEntry_ld476e4* entry = node->value) {
            fn_7100adffac(reinterpret_cast<UnkOwner_ovb0262a*>(entry));
            operator delete(entry);
        }
        self->entries.Remove(node);
    }
}
