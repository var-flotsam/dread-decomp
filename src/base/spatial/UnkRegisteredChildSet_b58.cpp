// Units 255 and 256 of tools/tu_map.py (no reflected class): two setters that replace an owned object
// entered in the registry g_unk_716e34c2e0 (as src/base/spatial/UnkDecalProjector.cpp's decal): the
// old one is removed from the registry (fn_710019e100); with an owner, a new one (0x140 bytes, built
// by fn_7100195680; 0x138 bytes, built by fn_7100196480) is allocated with flags 0x3b, given its
// owner (fn_710019e95c) and entered in the registry. Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "types.h"

extern "C" [[noreturn]] void __assert_fail(const char* expr, const char* file, int line, const char* func);

struct UnkKeyOwner;
extern void* g_unk_716e34c2e0;
bool fn_710019e100(UnkKeyOwner* self, u64 key);
void fn_710019e95c(void* self, void* owner);
void fn_7100195680(void* self);
void fn_7100196480(void* self);

namespace {

struct UnkRegistryNode_b58 {
    u64 key;
    u64 value;
    UnkRegistryNode_b58* next;
    UnkRegistryNode_b58* prev;
    UnkRegistryNode_b58* chain;

    explicit UnkRegistryNode_b58(u64 k) : key(k), value(k), next(nullptr), prev(nullptr), chain(nullptr) {}

    UnkRegistryNode_b58*& Next() { return next; }
    UnkRegistryNode_b58*& Prev() { return prev; }
};

// As UnkProjectorRegistry (src/base/spatial/UnkDecalProjector.cpp).
struct UnkRegistry_b58 {
    u8 pad_0x0[0x8];
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;
    UnkRegistryNode_b58** buckets;
    u32 bucket_count;
    u32 bucket_capacity;
    u8 pad_0x28[0x18];
    UnkRegistryNode_b58* tail;
    UnkRegistryNode_b58* head;

    void Insert(u64 key) {
        UnkRegistryNode_b58** bucket = nullptr;
        if (bucket_count != 0) {
            u32 i = (u32)key % bucket_capacity;
            if (bucket_count <= i)
                __assert_fail("i < m_iSize", "..\\Base/global/RntVector.h", 0x15b, "operator[]");
            bucket = &buckets[i];
        }
        UnkRegistryNode_b58* old = *bucket;
        auto* node = static_cast<UnkRegistryNode_b58*>(allocator->vf_0x40(sizeof(UnkRegistryNode_b58), 8, flags));
        if (node)
            new (node) UnkRegistryNode_b58(key);
        node->chain = old;
        *bucket = node;
        if (!head)
            head = node;
        else if (tail) {
            node->Prev() = tail;
            tail->Next() = node;
        }
        tail = node;
        ++count;
    }
};

template <u64 Size, void (*Ctor)(void*)>
__attribute__((always_inline)) inline void SetChild(void** slot, void* owner) {
    if (*slot) {
        fn_710019e100(static_cast<UnkKeyOwner*>(g_unk_716e34c2e0), reinterpret_cast<u64>(*slot));
        *slot = nullptr;
    }
    if (!owner)
        return;
    auto* registry = static_cast<UnkRegistry_b58*>(g_unk_716e34c2e0);
    void* obj = fn_71000839a0(Size, 0x3b);
    Ctor(obj);
    if (obj) {
        fn_710019e95c(obj, owner);
        registry->Insert(reinterpret_cast<u64>(obj));
    }
    *slot = obj;
}

}  // namespace

void fn_71001c3894(char* self, void* owner) {
    SetChild<0x140, fn_7100195680>(reinterpret_cast<void**>(self + 0x78), owner);
}

void fn_71001c3b14(char* self, void* owner) {
    SetChild<0x138, fn_7100196480>(reinterpret_cast<void**>(self + 0x80), owner);
}
