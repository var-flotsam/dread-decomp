// Units 204, 254 and 259 of tools/tu_map.py: three identical creators of a pooled object (0x58,
// 0x188 and 0x29d0 bytes, each with its own constructor): the object is allocated with the given
// flags and constructed, given its owner (fn_710019e95c) when there is one, and added to the pool's
// chained hash set of pointers (the layout of src/base/spatial/UnkModelInstanceSet.cpp's set, here
// at +8). Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "game/UnkTU218.h"
#include "game/UnkTU223.h"
#include "game/UnkTU226.h"
#include "types.h"

extern "C" [[noreturn]] void __assert_fail(const char* expr, const char* file, int line, const char* func);

void fn_71001a9f84(void* self);

struct UnkPoolNode_b54a1 {
    u64 key;
    u64 value;
    UnkPoolNode_b54a1* next;
    UnkPoolNode_b54a1* prev;
    UnkPoolNode_b54a1* chain;

    explicit UnkPoolNode_b54a1(u64 k) : key(k), value(k), next(nullptr), prev(nullptr), chain(nullptr) {}

    UnkPoolNode_b54a1*& Next() { return next; }
    UnkPoolNode_b54a1*& Prev() { return prev; }
};

struct UnkPool_b54a1 {
    u64 field_0x0;
    base::UnkAllocator* allocator;  // 0x8
    u32 flags;                      // 0x10
    u32 count;                      // 0x14
    UnkPoolNode_b54a1** buckets;    // 0x18
    u32 bucket_count;               // 0x20
    u32 bucket_capacity;            // 0x24
    u8 pad_0x28[0x18];
    UnkPoolNode_b54a1* tail;  // 0x40
    UnkPoolNode_b54a1* head;  // 0x48

    void Insert(u64 key) {
        UnkPoolNode_b54a1** bucket = nullptr;
        if (bucket_count != 0) {
            u32 i = (u32)key % bucket_capacity;
            if (bucket_count <= i)
                __assert_fail("i < m_iSize", "..\\Base/global/RntVector.h", 0x15b, "operator[]");
            bucket = &buckets[i];
        }
        UnkPoolNode_b54a1* old = *bucket;
        auto* node = static_cast<UnkPoolNode_b54a1*>(allocator->vf_0x40(sizeof(UnkPoolNode_b54a1), 8, flags));
        if (node)
            new (node) UnkPoolNode_b54a1(key);
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

    template <u64 Size, void (*Ctor)(void*)>
    void* Create(void* owner, u32 flags) {
        void* obj = fn_71000839a0(Size, flags);
        Ctor(obj);
        if (obj) {
            if (owner)
                fn_710019e95c(obj, owner);
            Insert(reinterpret_cast<u64>(obj));
        }
        return obj;
    }
};

void* fn_7100169e28(UnkPool_b54a1* self, void* owner, u32 flags) {
    return self->Create<0x58, fn_71001a9f84>(owner, flags);
}
void* fn_71001c35b4(UnkPool_b54a1* self, void* owner, u32 flags) {
    return self->Create<0x188, fn_710019bd2c>(owner, flags);
}
void* fn_71001cb410(UnkPool_b54a1* self, void* owner, u32 flags) {
    return self->Create<0x29d0, fn_7100198aac>(owner, flags);
}
