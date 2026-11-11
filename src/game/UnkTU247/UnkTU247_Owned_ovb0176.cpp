// Unit 247 (no reflected class): an object that owns a pooled object (made like
// src/base/UnkUnit204/UnkPooledCreate_b54.cpp's 0x29d0-byte one) and a list of string ids. Names and
// layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/UnkTU207.h"
#include "game/UnkTU218.h"
#include "game/UnkTU225.h"
#include "game/UnkTU226.h"
#include "types.h"

using base::global::CStrId;

extern "C" [[noreturn]] void __assert_fail(const char* expr, const char* file, int line, const char* func);

extern void* g_unk_716e34c2e0;

void fn_710016a8dc(void* self, void* arg);
struct UnkKeyOwner;
void* fn_710019964c(void* self);

struct UnkPoolNode_ovb0176 {
    u64 key;
    u64 value;
    UnkPoolNode_ovb0176* next;
    UnkPoolNode_ovb0176* prev;
    UnkPoolNode_ovb0176* chain;

    explicit UnkPoolNode_ovb0176(u64 k) : key(k), value(k), next(nullptr), prev(nullptr), chain(nullptr) {}

    UnkPoolNode_ovb0176*& Next() { return next; }
    UnkPoolNode_ovb0176*& Prev() { return prev; }
};

struct UnkPool_ovb0176 {
    u64 field_0x0;
    base::UnkAllocator* allocator;  // 0x8
    u32 flags;                      // 0x10
    u32 count;                      // 0x14
    UnkPoolNode_ovb0176** buckets;    // 0x18
    u32 bucket_count;               // 0x20
    u32 bucket_capacity;            // 0x24
    u8 pad_0x28[0x18];
    UnkPoolNode_ovb0176* tail;  // 0x40
    UnkPoolNode_ovb0176* head;  // 0x48

    void Insert(u64 key) {
        UnkPoolNode_ovb0176** bucket = nullptr;
        if (bucket_count != 0) {
            u32 i = (u32)key % bucket_capacity;
            if (bucket_count <= i)
                __assert_fail("i < m_iSize", "..\\Base/global/RntVector.h", 0x15b, "operator[]");
            bucket = &buckets[i];
        }
        UnkPoolNode_ovb0176* old = *bucket;
        auto* node = static_cast<UnkPoolNode_ovb0176*>(allocator->vf_0x40(sizeof(UnkPoolNode_ovb0176), 8, flags));
        if (node)
            new (node) UnkPoolNode_ovb0176(key);
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



struct UnkOwnedHolder_ovb0176 {
    u8 pad_0x0[0x78];
    void* object;  // 0x78
    u8 pad_0x80[0x58];
    CStrId* ids;  // 0xd8
    u32 id_count;  // 0xe0
};
static_assert(__builtin_offsetof(UnkOwnedHolder_ovb0176, id_count) == 0xe0, "layout");

void fn_71001bbc88(UnkOwnedHolder_ovb0176* self) {
    fn_710016ae30(reinterpret_cast<u8*>(self) + 0x80);
    for (u64 i = 0; i < self->id_count; ++i)
        self->ids[i].Reset();
    self->id_count = 0;
    if (self->object) {
        fn_710016a8dc(reinterpret_cast<u8*>(self) + 0x80, nullptr);
        fn_710019e100(static_cast<UnkKeyOwner*>(g_unk_716e34c2e0), reinterpret_cast<u64>(self->object));
        self->object = nullptr;
    }
}

void fn_71001bbd6c(UnkOwnedHolder_ovb0176* self, void* owner) {
    if (self->object) {
        fn_710016a8dc(reinterpret_cast<u8*>(self) + 0x80, nullptr);
        fn_710019e100(static_cast<UnkKeyOwner*>(g_unk_716e34c2e0), reinterpret_cast<u64>(self->object));
        self->object = nullptr;
    }
    if (owner) {
        auto* pool = static_cast<UnkPool_ovb0176*>(g_unk_716e34c2e0);
        void* obj = pool->Create<0x29d0, fn_7100198aac>(owner, 0x1a);
        self->object = obj;
        fn_710016a8dc(reinterpret_cast<u8*>(self) + 0x80, **reinterpret_cast<void***>(reinterpret_cast<u8*>(obj) + 0x48));
    }
}
