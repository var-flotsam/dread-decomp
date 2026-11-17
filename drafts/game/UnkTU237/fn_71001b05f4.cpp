// Unit 237: rebases a block of linked lists after it was copied from oldBase to its current address,
// rewriting every stored pointer so it points into the block at newBase.
// Names and layouts are placeholders.
#include "types.h"

// Moves a pointer that points into a block at `from` to the same place in a block at `to`.
template <typename T>
inline void RelocatePointer(T*& ptr, const void* from, const void* to) {
    if (from != to && ptr != nullptr) {
        ptr = reinterpret_cast<T*>(reinterpret_cast<u8*>(ptr) +
                                   (static_cast<const u8*>(to) - static_cast<const u8*>(from)));
    }
}

struct UnkRelocItem3 {
    u8 pad_0x0[0x8];
    void* field_0x8;
    u8 pad_0x10[0x8];
    void* field_0x18;
    u8 pad_0x20[0x8];
    void* field_0x28;

    void Relocate(const void* from, const void* to) {
        RelocatePointer(field_0x8, from, to);
        RelocatePointer(field_0x18, from, to);
        RelocatePointer(field_0x28, from, to);
    }
};

struct UnkRelocItem1 {
    u8 pad_0x0[0x8];
    void* field_0x8;

    void Relocate(const void* from, const void* to) { RelocatePointer(field_0x8, from, to); }
};

struct UnkRelocItem0 {
    void* field_0x0;

    void Relocate(const void* from, const void* to) { RelocatePointer(field_0x0, from, to); }
};

template <typename Item>
struct UnkRelocNode {
    Item* item;
    UnkRelocNode* next;
};

struct UnkRelocHeader {
    u8 pad_0x0[0x8];
    UnkRelocNode<UnkRelocItem3>* list_0x8;
    UnkRelocNode<UnkRelocItem1>* list_0x10;
    void* field_0x18;
    UnkRelocNode<UnkRelocItem0>* list_0x20;
    UnkRelocNode<UnkRelocItem0>* list_0x28;
    UnkRelocNode<UnkRelocItem0>* list_0x30;
    UnkRelocNode<UnkRelocItem0>* list_0x38;
};

struct UnkRelocOwner {
    u8 pad_0x0[0x10];
    UnkRelocHeader* header;
};

// Rebases one node of a list whose links still hold oldBase addresses: the node and its item are reached at
// their current address (base), the node's links are rewritten for newBase, and the next node is returned
// as stored (an oldBase address).
template <typename Item>
inline UnkRelocNode<Item>* RelocateNode(UnkRelocNode<Item>* node, const void* oldBase, const void* base,
                                        const void* newBase) {
    RelocatePointer(node, oldBase, base);
    RelocatePointer(node->item, oldBase, base);
    Item* item = node->item;
    RelocatePointer(node->item, base, newBase);
    UnkRelocNode<Item>* next = node->next;
    RelocatePointer(node->next, oldBase, newBase);
    item->Relocate(oldBase, newBase);
    return next;
}

bool fn_71001b05f4(UnkRelocOwner* self, const void* oldBase, const void* newBase) {
    const UnkRelocHeader* base = self->header;
    if (base == nullptr) {
        return false;
    }

    for (auto* node = self->header->list_0x8; node != nullptr;) {
        node = RelocateNode(node, oldBase, base, newBase);
    }
    RelocatePointer(self->header->list_0x8, oldBase, newBase);
    for (auto* node = self->header->list_0x10; node != nullptr;) {
        node = RelocateNode(node, oldBase, base, newBase);
    }
    RelocatePointer(self->header->list_0x10, oldBase, newBase);
    RelocatePointer(self->header->field_0x18, oldBase, newBase);
    for (auto* node = self->header->list_0x20; node != nullptr;) {
        node = RelocateNode(node, oldBase, base, newBase);
    }
    RelocatePointer(self->header->list_0x20, oldBase, newBase);
    for (auto* node = self->header->list_0x28; node != nullptr;) {
        node = RelocateNode(node, oldBase, base, newBase);
    }
    RelocatePointer(self->header->list_0x28, oldBase, newBase);
    for (auto* node = self->header->list_0x30; node != nullptr;) {
        node = RelocateNode(node, oldBase, base, newBase);
    }
    RelocatePointer(self->header->list_0x30, oldBase, newBase);
    for (auto* node = self->header->list_0x38; node != nullptr;) {
        node = RelocateNode(node, oldBase, base, newBase);
    }
    RelocatePointer(self->header->list_0x38, oldBase, newBase);
    return true;
}
