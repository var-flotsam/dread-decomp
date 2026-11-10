// Unit 257 of tools/tu_map.py (no class hint): creates the object this one holds (+0x78) in its pool
// (g_unk_716e34c2e0): a 0x1c0-byte object, constructed with fn_710019c6c0 (src/game_s1/UnkUnit224/
// UnkUnit224_Ctor_A1_b69.cpp) and added to the pool's chained hash set of pointers like
// src/base/UnkUnit204/UnkPooledCreate_b54.cpp does. Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "types.h"

extern "C" [[noreturn]] void __assert_fail(const char* expr, const char* file, int line, const char* func);

struct UnkObject224_b69a1;
void fn_710019c6c0(UnkObject224_b69a1* self);

struct UnkPoolNode_b0202 {
    u64 key;
    u64 value;
    UnkPoolNode_b0202* next;
    UnkPoolNode_b0202* prev;
    UnkPoolNode_b0202* chain;

    explicit UnkPoolNode_b0202(u64 k) : key(k), value(k), next(nullptr), prev(nullptr), chain(nullptr) {}

    UnkPoolNode_b0202*& Next() { return next; }
    UnkPoolNode_b0202*& Prev() { return prev; }
};

struct UnkPool_b0202 {
    u64 field_0x0;
    base::UnkAllocator* allocator;  // 0x8
    u32 flags;                      // 0x10
    u32 count;                      // 0x14
    UnkPoolNode_b0202** buckets;    // 0x18
    u32 bucket_count;               // 0x20
    u32 bucket_capacity;            // 0x24
    u8 pad_0x28[0x18];
    UnkPoolNode_b0202* tail;  // 0x40
    UnkPoolNode_b0202* head;  // 0x48

    void Insert(u64 key) {
        UnkPoolNode_b0202** bucket = nullptr;
        if (bucket_count != 0) {
            u32 i = (u32)key % bucket_capacity;
            if (bucket_count <= i)
                __assert_fail("i < m_iSize", "..\\Base/global/RntVector.h", 0x15b, "operator[]");
            bucket = &buckets[i];
        }
        UnkPoolNode_b0202* old = *bucket;
        auto* node = static_cast<UnkPoolNode_b0202*>(allocator->vf_0x40(sizeof(UnkPoolNode_b0202), 8, flags));
        if (node)
            new (node) UnkPoolNode_b0202(key);
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
extern void* g_unk_716e34c2e0;

struct UnkHolder_b0202 {
    u8 pad_0x0[0x78];
    void* object;  // 0x78
};

void fn_71001c3d80(UnkHolder_b0202* self) {
    UnkPool_b0202* pool = static_cast<UnkPool_b0202*>(g_unk_716e34c2e0);
    void* obj = fn_71000839a0(0x1c0, 0x3b);
    fn_710019c6c0(static_cast<UnkObject224_b69a1*>(obj));
    if (obj)
        pool->Insert(reinterpret_cast<u64>(obj));
    self->object = obj;
}
