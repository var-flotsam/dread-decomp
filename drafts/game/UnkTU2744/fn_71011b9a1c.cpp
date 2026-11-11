#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

struct UnkIdHashData_b0209 {
    u8 pad_0x0[0x30];
    u32 hash_a;
    u32 hash_b;
};

struct UnkGuiRefCount_b0209 {
    s32 count;
};

struct UnkGuiRefNode_b0209 {
    CStrId key;
    UnkGuiRefCount_b0209* value;
    UnkGuiRefNode_b0209* next;
    UnkGuiRefNode_b0209* prev;
    UnkGuiRefNode_b0209* chain;  // 0x20
};

struct UnkGuiRefMap_b0209 {
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;
    UnkGuiRefNode_b0209** buckets;  // 0x10
    u32 bucket_count;               // 0x18
    u32 bucket_capacity;            // 0x1c
};

struct UnkGuiRefs_b0209 {
    u8 pad_0x0[8];
    UnkGuiRefMap_b0209* map;  // 0x8
};

// A string id built from a run-time string (fn_7100000850).
struct UnkBuiltId_b0209 {
    union {
        CStrId id;
    };
    explicit UnkBuiltId_b0209(const CRntString& str) { fn_7100000850(&id, str, true); }
    ~UnkBuiltId_b0209() { id.~CStrId(); }
};

void fn_71011b9bc4(UnkGuiRefs_b0209* self, const char* const* name);

void fn_71011b9a1c(UnkGuiRefs_b0209* self, const char* const* name) {
    UnkGuiRefMap_b0209* map = self->map;
    if (!map)
        return;
    const char* text = *name;
    if (!text)
        text = g_unk_710162e2ed;
    UnkGuiRefNode_b0209* found = nullptr;
    {
        CStrId id(text, true);
        if (map->bucket_count) {
            auto* d = reinterpret_cast<UnkIdHashData_b0209*>(id.data);
            u32 hash = d->hash_a ^ d->hash_b;
            u32 index = hash % map->bucket_capacity;
            UnkGuiRefNode_b0209** slot = map->buckets + index;
            if (slot) {
                for (UnkGuiRefNode_b0209* n = *slot; n; n = n->chain) {
                    if (n->key.data == id.data) {
                        found = n;
                        break;
                    }
                }
            }
        }
    }
    if (!found)
        return;
    map = self->map;
    UnkGuiRefCount_b0209* ref;
    {
        UnkBuiltId_b0209 id(*reinterpret_cast<const CRntString*>(name));
        auto* d = reinterpret_cast<UnkIdHashData_b0209*>(id.id.data);
        u32 hash = d->hash_a ^ d->hash_b;
        u32 index = hash % map->bucket_capacity;
        UnkGuiRefNode_b0209* n = map->buckets[index];
        while (n->key.data != id.id.data)
            n = n->chain;
        ref = n->value;
    }
    if (--ref->count == 0)
        fn_71011b9bc4(self, name);
}
