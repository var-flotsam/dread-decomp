// Unit 271 of tools/tu_map.py (base::reflection names): adds an object to the registry's chained hash
// maps keyed by the CRC-64 of its name, unless already there: to the map of all objects (+0x30), then by
// its kind (vtable slot 0x88) to the map at +0x78 (kind 2) or +0xc0 (kind 0x10). The counterpart of
// fn_71001eed70. Names and layouts are placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

extern "C" [[noreturn]] void __assert_fail(const char* expr, const char* file, int line, const char* func);

struct UnkRegNode_l1ee994 {
    u64 key;
    const void* value;          // 0x8
    UnkRegNode_l1ee994* next;   // 0x10: the list of all nodes
    UnkRegNode_l1ee994* prev;   // 0x18
    UnkRegNode_l1ee994* chain;  // 0x20: the next node in the bucket

    UnkRegNode_l1ee994(u64 k, const void* v) : key(k), value(v), next(nullptr), prev(nullptr), chain(nullptr) {}
};

struct UnkRegMap_l1ee994 {
    base::UnkAllocator* allocator;
    u32 flags;                     // 0x8
    u32 count;                     // 0xc
    UnkRegNode_l1ee994** buckets;  // 0x10 (base::global::CRntVector)
    u32 bucket_count;              // 0x18
    u32 bucket_capacity;           // 0x1c
    u8 pad_0x20[0x18];
    UnkRegNode_l1ee994* tail;  // 0x38
    UnkRegNode_l1ee994* head;  // 0x40

    UnkRegNode_l1ee994** Bucket(const u64& key) {
        if (bucket_count == 0)
            return nullptr;
        u32 i = ((u32)(key >> 32) ^ (u32)key) % bucket_capacity;
        if (bucket_count <= i)
            __assert_fail("i < m_iSize", "..\\Base/global/RntVector.h", 0x15b, "operator[]");
        return &buckets[i];
    }

    bool Contains(const u64& key) {
        UnkRegNode_l1ee994** bucket = Bucket(key);
        if (!bucket)
            return false;
        for (UnkRegNode_l1ee994* n = *bucket; n; n = n->chain) {
            if (n->key == key)
                return true;
        }
        return false;
    }

    void Add(const u64& key, const void* const& value) {
        UnkRegNode_l1ee994** bucket = Bucket(key);
        UnkRegNode_l1ee994* old = *bucket;
        auto* node = static_cast<UnkRegNode_l1ee994*>(allocator->vf_0x40(sizeof(UnkRegNode_l1ee994), 8, flags));
        if (node)
            new (node) UnkRegNode_l1ee994(key, value);
        node->chain = old;
        *bucket = node;
        if (!head)
            head = node;
        else if (tail) {
            node->prev = tail;
            tail->next = node;
        }
        tail = node;
        ++count;
    }
};
static_assert(sizeof(UnkRegMap_l1ee994) == 0x48, "UnkRegMap_l1ee994 layout");

struct UnkRegistry_l1ee994 {
    u8 pad_0x0[0x30];
    UnkRegMap_l1ee994 all;     // 0x30
    UnkRegMap_l1ee994 kind2;   // 0x78
    UnkRegMap_l1ee994 kind16;  // 0xc0
};

struct UnkRegNameData_l1ee994 {
    u8 pad_0x0[0x10];
    const char* text;  // 0x10
    u8 pad_0x18[0x44 - 0x18];
    u32 use_count;  // 0x44
};

struct UnkRegKind_l1ee994 {
    s32 kind;
    u8 pad_0x4[0xc];
    const void* entry;  // 0x10: what the kind's map holds
};

struct UnkRegObject_l1ee994 {
    void** vtable;
    u8 pad_0x8[0x8];
    UnkRegNameData_l1ee994* name;  // 0x10

    const UnkRegKind_l1ee994* vf_0x88() {
        return reinterpret_cast<const UnkRegKind_l1ee994* (*)(UnkRegObject_l1ee994*)>(vtable[0x88 / 8])(this);
    }
};

void fn_71001ee994(UnkRegistry_l1ee994* self, UnkRegObject_l1ee994* obj) {
    ++obj->name->use_count;
    const char* text = obj->name->text;
    text = text ? text : g_unk_710162e2ed;
    u64 hash = fn_7100001570(text, __builtin_strlen(text));
    if (!self->all.Contains(hash))
        self->all.Add(hash, obj);
    switch (obj->vf_0x88()->kind) {
    case 0x10:
        if (!self->kind16.Contains(hash))
            self->kind16.Add(hash, obj->vf_0x88()->entry);
        break;
    case 2:
        if (!self->kind2.Contains(hash))
            self->kind2.Add(hash, obj->vf_0x88()->entry);
        break;
    }
}
