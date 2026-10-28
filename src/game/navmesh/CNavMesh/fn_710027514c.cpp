// Copies of fn_71008ce35c, fn_71008ce190 (src/game_s4/fn_71008ce35c.cpp), which the binary
// repeats at other addresses: fn_710027514c, fn_710027497c.
// Constructor and destructor of five groups of three arrays: each group first unlinks the objects of its first
// array (clearing their back-pointers) and empties all three, then frees them. fn_7100a9dbac and
// fn_7100a9dd78 are identical copies.
#include "base/UnkVector.h"

struct UnkLinked {
    void* owner;
};

template <class T>
struct UnkArray {
    T* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) UnkArray() : data(nullptr), count(0), capacity(0), step(10), field_0x14(true), flags(0xf) {
        allocator = fn_7100083990();
    }

    T* begin() { return data; }
    T* end() { return data + count; }

    ~UnkArray() {
        if (data) {
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

struct UnkLinkGroup {
    UnkArray<UnkLinked*> linked;
    UnkArray<u64> field_0x28;
    UnkArray<u64> field_0x50;
    bool field_0x78;

    __attribute__((always_inline)) UnkLinkGroup() : field_0x78(false) {}
    ~UnkLinkGroup() {
        for (UnkLinked* p : linked)
            p->owner = nullptr;
        linked.count = 0;
        field_0x28.count = 0;
        field_0x50.count = 0;
    }
};

struct UnkLinkGroups {
    UnkLinkGroup groups[5];
};

static inline __attribute__((always_inline)) void Destroy(UnkLinkGroups* self) {
#pragma clang loop unroll(full)
    for (int i = 5; i-- > 0;)
        self->groups[i].~UnkLinkGroup();
}

void fn_710027514c(UnkLinkGroups* self) {
    Destroy(self);
}

void fn_7100a9dd78(UnkLinkGroups* self);

static inline __attribute__((always_inline)) void Construct(UnkLinkGroups* self) {
#pragma clang loop unroll(full)
    for (int i = 0; i < 5; i++)
        new (&self->groups[i]) UnkLinkGroup;
}

void fn_710027497c(UnkLinkGroups* self) {
    Construct(self);
}

void fn_7100a9dbac(UnkLinkGroups* self);

