// Unit 271 of tools/tu_map.py (base::reflection names): two lookups in chained hash maps keyed by a
// 64-bit id (bucket = (high ^ low) % capacity over a base::global::CRntVector of buckets, next node at
// +0x20): one hands the value found to fn_71001f9e8c with the owner, the other clears the value's
// word at +0xc0. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/UnkTU253.h"
#include "types.h"

extern "C" [[noreturn]] void __assert_fail(const char* expr, const char* file, int line, const char* func);

struct UnkMapNode_A1b71 {
    u64 key;
    void* value;
    u8 pad_0x10[0x10];
    UnkMapNode_A1b71* next;  // 0x20
};

struct UnkMap_A1b71 {
    UnkMapNode_A1b71** buckets;
    u32 count;
    u32 capacity;

    __attribute__((always_inline)) UnkMapNode_A1b71* Find(u64 key) {
        if (count == 0)
            return nullptr;
        u32 i = ((u32)(key >> 32) ^ (u32)key) % capacity;
        if (count <= i)
            __assert_fail("i < m_iSize", "..\\Base/global/RntVector.h", 0x15b, "operator[]");
        UnkMapNode_A1b71** bucket = &buckets[i];
        if (!bucket)
            return nullptr;
        for (UnkMapNode_A1b71* n = *bucket; n; n = n->next) {
            if (n->key == key)
                return n;
        }
        return nullptr;
    }
};

struct UnkMapOwnerA_A1b71 {
    u8 pad_0x0[0x118];
    UnkMap_A1b71 map;  // 0x118
};

bool fn_71001f9e8c(void* value, UnkMapOwnerA_A1b71* owner);

bool fn_71001ef364(UnkMapOwnerA_A1b71* self, u64 key) {
    if (UnkMapNode_A1b71* node = self->map.Find(key))
        return fn_71001f9e8c(node->value, self);
    return false;
}

struct UnkMapKey_A1b71 {
    u8 pad_0x0[0x30];
    u64 id;  // 0x30
};

struct UnkMapValueB_A1b71 {
    u8 pad_0x0[0xc0];
    void* field_0xc0;
};

struct UnkMapOwnerB_A1b71 {
    u8 pad_0x0[0xd0];
    UnkMap_A1b71 map;  // 0xd0
};

void fn_71001f023c(UnkMapOwnerB_A1b71* self, UnkMapKey_A1b71* const* key) {
    if (UnkMapNode_A1b71* node = self->map.Find((*key)->id))
        static_cast<UnkMapValueB_A1b71*>(node->value)->field_0xc0 = nullptr;
}

bool fn_7100199d88(void* table, const u64* key, u32* id);  // as src/game/UnkSlotBlend.cpp declares it

struct UnkMapUser_A1b71 {
    u8 pad_0x0[0x78];
    void* table;  // 0x78
};

struct UnkMapOwnerD_A1b71 {
    u8 pad_0x0[0x88];
    UnkMap_A1b71 users;  // 0x88
    u8 pad_0x98[0x38];
    UnkMap_A1b71 map;  // 0xd0
};

// A key found in the map at +0xd0 and a user found in the map at +0x88: the user's table looks up
// the third key's id.
void fn_71001f0074(UnkMapOwnerD_A1b71* self, UnkMapKey_A1b71* const* key, UnkMapKey_A1b71* const* user_key,
                   UnkMapKey_A1b71* const* other) {
    u64 k = (*key)->id;
    u64 uid = (*user_key)->id;
    if (self->map.Find(k)) {
        if (UnkMapNode_A1b71* node = self->users.Find(uid)) {
            void* table = static_cast<UnkMapUser_A1b71*>(node->value)->table;
            u32 id = 0;
            u64 ok = (*other)->id;
            fn_7100199d88(table, &ok, &id);
        }
    }
}

// With a user, a key found in the map at +0xd0 makes the user's table look up the second key's id.
void fn_71001f0180(UnkMapOwnerB_A1b71* self, UnkMapKey_A1b71* const* key, UnkMapUser_A1b71* user,
                   UnkMapKey_A1b71* const* other) {
    if (!user)
        return;
    if (self->map.Find((*key)->id)) {
        void* table = user->table;
        u32 id = 0;
        u64 k = (*other)->id;
        fn_7100199d88(table, &k, &id);
    }
}

// The node of the map at +0x108 of the first owner, with its list links (the layout of
// src/base/reflection/UnkUnit271_PairMapInsert_b61.cpp).
struct UnkListNode_A1b71 {
    u64 key;
    void* value;
    UnkListNode_A1b71* next;   // 0x10: the list of all nodes
    UnkListNode_A1b71* prev;   // 0x18
    UnkListNode_A1b71* chain;  // 0x20: the next node in the bucket

    UnkListNode_A1b71*& Next() { return next; }
    UnkListNode_A1b71*& Prev() { return prev; }
};

struct UnkListMap_A1b71 {
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;
    UnkListNode_A1b71** buckets;  // 0x10 (base::global::CRntVector)
    u32 bucket_count;             // 0x18
    u32 bucket_capacity;          // 0x1c
    u8 pad_0x20[0x18];
    UnkListNode_A1b71* tail;  // 0x38
    UnkListNode_A1b71* head;  // 0x40
};

struct UnkMapOwnerC_A1b71 {
    u8 pad_0x0[0x108];
    UnkListMap_A1b71 map;  // 0x108
};

struct UnkMapEntry_A1b71 {
    u8 pad_0x0[0x8];
    UnkMapKey_A1b71* key;  // 0x8
};

// Adds `entry` under its key's id: at the head of its bucket chain and the tail of the list.
void fn_71001ef038(UnkMapOwnerC_A1b71* self, UnkMapEntry_A1b71* entry) {
    u64 key = entry->key->id;
    UnkListMap_A1b71* m = &self->map;
    UnkListNode_A1b71** slot = nullptr;
    if (m->bucket_count) {
        u32 i = ((u32)(key >> 32) ^ (u32)key) % m->bucket_capacity;
        if (m->bucket_count <= i)
            __assert_fail("i < m_iSize", "..\\Base/global/RntVector.h", 0x15b, "operator[]");
        slot = &m->buckets[i];
    }
    UnkListNode_A1b71* chain = *slot;
    auto* n = static_cast<UnkListNode_A1b71*>(m->allocator->vf_0x40(0x28, 8, m->flags));
    if (n) {
        n->key = key;
        n->value = entry;
        n->chain = nullptr;
        n->next = nullptr;
        n->prev = nullptr;
    }
    n->chain = chain;
    *slot = n;
    if (!m->head) {
        m->head = n;
    } else if (m->tail) {
        n->Prev() = m->tail;
        m->tail->Next() = n;
    }
    m->tail = n;
    ++m->count;
}

struct UnkResetOwner;  // src/base/UnkUnit247/UnkUnit247_ResetEntries_b40.cpp

// What the value's vf_0x88 returns: a kind, and for kind 2 the entries fn_71001c3270 resets.
struct UnkMapState_A1b71 {
    s32 kind;
    u8 pad_0x4[0xc];
    UnkResetOwner* entries;  // 0x10
};

class UnkMapValueE_A1b71 {
public:
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual UnkMapState_A1b71* vf_0x88();

    u8 pad_0x8[0xf4 - 0x8];
    u8 enabled;  // 0xf4
    u8 pad_0xf5[0x7];
    bool can_toggle;  // 0xfc
};

struct UnkMapOwnerE_A1b71 {
    u8 pad_0x0[0x40];
    UnkMap_A1b71 map;  // 0x40
};

// Sets the enabled flag of the object stored under `*key` (when it can be toggled), first resetting
// its state's entries when asked and the state is of kind 2. False when there is no such object.
bool fn_71001efa3c(UnkMapOwnerE_A1b71* self, bool enabled, const u64* key, bool reset) {
    UnkMapNode_A1b71* node = self->map.Find(*key);
    if (!node)
        return false;
    UnkMapValueE_A1b71* value = static_cast<UnkMapValueE_A1b71*>(node->value);
    if (!value)
        return false;
    UnkMapState_A1b71* state = value->vf_0x88();
    if (reset && state->kind == 2)
        fn_71001c3270(state->entries);
    if ((value->enabled != 0) != enabled && value->can_toggle) {
        bool e = enabled;
        asm("" : "+r"(e));
        value->enabled = e;
    }
    return true;
}
