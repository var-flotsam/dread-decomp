// Unit 604 of tools/tu_map.py (no name hint): destructors of pooled node lists (the node's destructor body
// inlined into its owners) and a lookup that clears a slot's reference. Names and layouts are placeholders.
#include "base/UnkBlockFreeList.h"
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "game/UnkTU604.h"
#include "types.h"

using base::UnkAllocator;

extern char g_unk_7101a326a0[];

struct UnkPoolNodeList_b256;

template <>
struct base::UnkListOwnerTraits<UnkPoolNodeList_b256> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a32660); }
    static char* base_vtable() { return reinterpret_cast<char*>(g_unk_7101a32680); }
    static char* member_vtable() { return g_unk_7101a1a4c8; }
};

// The destructor of one pooled list (the deleting form's body without the free).
__attribute__((always_inline)) static inline void DestroyNodeList(char* self) {
    using Traits = base::UnkListOwnerTraits<UnkPoolNodeList_b256>;
    *reinterpret_cast<char**>(self) = Traits::vtable() + 0x10;
    *reinterpret_cast<bool*>(self + 0x6c) = true;
    base::refl_FreeListNodes<UnkPoolNodeList_b256, 8>(self);
    *reinterpret_cast<u32*>(self + 8) = 0;
    *reinterpret_cast<char**>(self + 0x10) = nullptr;
    *reinterpret_cast<char**>(self + 0x18) = nullptr;
    *reinterpret_cast<char**>(self + 0x20) = nullptr;
    *reinterpret_cast<char**>(self + 0x30) = Traits::member_vtable() + 0x10;
    if (char* p = *reinterpret_cast<char**>(self + 0x60)) {
        fn_71000837f4(reinterpret_cast<UnkBlockFreeList*>(self + 0x40));
        (*reinterpret_cast<UnkAllocator**>(self + 0x38))->vf_0x48(p);
        *reinterpret_cast<char**>(self + 0x38) = nullptr;
    }
    *reinterpret_cast<char**>(self) = Traits::base_vtable() + 0x10;
    base::refl_FreeListNodes<UnkPoolNodeList_b256, 8>(self);
    *reinterpret_cast<u32*>(self + 8) = 0;
    *reinterpret_cast<char**>(self + 0x10) = nullptr;
    *reinterpret_cast<char**>(self + 0x18) = nullptr;
}

void fn_71003fadf4(char* owner, char* list) {
    if (!list)
        return;
    DestroyNodeList(list);
    (*reinterpret_cast<UnkAllocator**>(owner + 0x20))->vf_0x48(list);
}

void fn_71003facac(char* self) {
    *reinterpret_cast<char**>(self) = g_unk_7101a326a0 + 0x10;
    char* n = *reinterpret_cast<char**>(self + 0x10);
    while (n) {
        char* next = *reinterpret_cast<char**>(n + 0x78);
        DestroyNodeList(n);
        (*reinterpret_cast<UnkAllocator**>(self + 0x20))->vf_0x48(n);
        n = next;
    }
    __builtin_operator_delete(self);
}

extern char g_unk_7101a32640[];

void fn_71003f8b80(char* self) {
    *reinterpret_cast<char**>(self) = g_unk_7101a32640 + 0x10;
    *reinterpret_cast<char**>(self + 8) = g_unk_7101a326a0 + 0x10;
    char* n = *reinterpret_cast<char**>(self + 0x18);
    while (n) {
        char* next = *reinterpret_cast<char**>(n + 0x78);
        DestroyNodeList(n);
        (*reinterpret_cast<UnkAllocator**>(self + 0x28))->vf_0x48(n);
        n = next;
    }
    *reinterpret_cast<u32*>(self + 0x10) = 0;
    *reinterpret_cast<char**>(self + 0x18) = nullptr;
    *reinterpret_cast<char**>(self + 0x20) = nullptr;
}

struct UnkListRef_b256 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    union {
        s32 ref_count;
    };
    u32 pad_0xc;
    void* owner;  // 0x10
};

struct UnkListEntry_b256 {
    void* value;
    UnkListEntry_b256* next;
};

struct UnkListGroup_b256 {
    u8 pad_0x0[8];
    u32 count;                  // 0x8
    UnkListEntry_b256* head;    // 0x10
    u8 pad_0x18[0x58];
    void* key;                  // 0x70
    UnkListGroup_b256* next;    // 0x78
};

struct UnkListGroups_b256 {
    u8 pad_0x0[0x10];
    u32 count;                // 0x10
    UnkListGroup_b256* head;  // 0x18
};

struct UnkListSlot_b256 {
    UnkListRef_b256* ref;  // 0x0
    u8 pad_0x8[4];
    u8 flag;  // 0xc
};

char* fn_71003f92ac(void* value);

static inline UnkListGroup_b256* NthGroup(UnkListGroup_b256* n, u32 i) {
    for (u32 k = 0; k < i; ++k)
        n = n->next;
    return n;
}

static inline UnkListEntry_b256* NthEntry(UnkListEntry_b256* e, u32 i) {
    for (u32 k = 0; k < i; ++k)
        e = e->next;
    return e;
}

void fn_71003f9b58(UnkListGroups_b256* self, void* key, void* target) {
    u32 total = self->count;
    if (!total)
        return;
    UnkListGroup_b256* head = self->head;
    u32 found = 0;
    for (;; ++found) {
        if (NthGroup(head, found)->key == key)
            break;
        if (found + 1 == total)
            return;
    }
    UnkListGroup_b256* group = head;
    for (u32 k = 0; k != found; ++k) {
        if (k == total - 1)
            return;
        group = group->next;
    }
    if (!group)
        return;
    u32 n = group->count;
    if (!n)
        return;
    for (u32 j = 0; j != n; ++j) {
        UnkListEntry_b256* e = NthEntry(group->head, j);
        UnkListSlot_b256* slot = reinterpret_cast<UnkListSlot_b256*>(fn_71003f92ac(e->value) + 0x146d0);
        UnkListRef_b256* ref = slot->ref;
        void* owner = ref ? ref->owner : nullptr;
        if (owner == target) {
            slot->flag = 1;
            if (ref) {
                if (--ref->ref_count == 0)
                    ref->vf_0x10();
                slot->ref = nullptr;
            }
            return;
        }
    }
}
