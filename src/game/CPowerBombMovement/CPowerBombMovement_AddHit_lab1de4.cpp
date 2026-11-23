// CPowerBombMovement's unit (tools/tu_map.py unit 1612): adds a hit record to the 128-record list unless its
// collision component is already listed (the "skip" result); a collision component the record names is also
// added to the list's component pointers. Results: 1 added, 2 full, 3 added and now full.
// Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityShinespark.h"
#include "game/UnkTU365.h"
#include "game/UnkVisitResult_b0040.h"
#include "types.h"

#include <algorithm>
#include <new>

using base::reflection::UnkReflObject;

extern "C" int __cxa_guard_acquire(u64* guard);
extern "C" void __cxa_guard_release(u64* guard);

static inline UnkVisitResult_b0040 Skip_lab1de4() {
    if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(&g_unk_71726a4780), __ATOMIC_ACQUIRE) & 1), 0)) {
        if (__cxa_guard_acquire(&g_unk_71726a4780)) {
            g_unk_71726a4778.value = 0;
            __cxa_guard_release(&g_unk_71726a4780);
        }
    }
    return g_unk_71726a4778;
}

void* fn_7100ab1fa8(void* body);

struct UnkPbCollision_lab1de4 : UnkReflObject {
    u8 pad_0x8[0x28];
    void* body;  // 0x30
};

struct UnkPbHolder_lab1de4 {
    u8 pad_0x0[0x10];
    UnkReflObject* object;  // 0x10
};

struct UnkPbOwner_lab1de4 {
    u8 pad_0x0[0x10];
    UnkPbHolder_lab1de4* holder;  // 0x10
};

#pragma pack(push, 4)
struct UnkPbHit_lab1de4 {
    UnkPbOwner_lab1de4* owner;
    u64 unk_0x8[8];
    UnkPbHit_lab1de4(const UnkPbHit_lab1de4& other) { fn_7100331c7c(this, &other); }
};


#pragma pack(pop)
static_assert(sizeof(UnkPbHit_lab1de4) == 0x48, "hit");

// A fixed-capacity list: a count, then room for N elements.
template <typename T, int N>
struct alignas(8) UnkPbFixedList_lab1de4 {
    u32 count;
    u8 storage[N * sizeof(T)];

    T* begin() { return reinterpret_cast<T*>(storage); }
    T* end() { return begin() + count; }
};

struct UnkPbHitList_lab1de4 {
    void* vtable;
    UnkPbFixedList_lab1de4<UnkPbHit_lab1de4, 128> hits;           // 0x8
    UnkPbFixedList_lab1de4<UnkReflObject*, 128> components;  // 0x2410
};
static_assert(__builtin_offsetof(UnkPbHitList_lab1de4, components) == 0x2410, "list");

static inline bool IsCollision(UnkReflObject* object) {
    return fn_710009d50c(object->vf_0x0(), fn_710085fc44());
}

UnkVisitResult_b0040 fn_7100ab1de4(UnkPbHitList_lab1de4* self, const UnkPbHit_lab1de4& hit) {
    UnkReflObject* object = nullptr;
    bool tracked = false;
    if (hit.owner && hit.owner->holder) {
        object = hit.owner->holder->object;
        if (object && IsCollision(object)) {
            void* body = static_cast<UnkPbCollision_lab1de4*>(object)->body;
            tracked = body && fn_7100ab1fa8(body);
        }
    }
    if (!tracked && std::find(self->components.begin(), self->components.end(), object) != self->components.end())
        return Skip_lab1de4();

    UnkReflObject* component = hit.owner->holder->object;
    if (component && IsCollision(component)) {
        self->components.begin()[self->components.count] = hit.owner->holder->object;
        ++self->components.count;
    }

    u32 count = self->hits.count;
    s32 free = 128 - count;
    if (free > 0) {
        new (&self->hits.begin()[count]) UnkPbHit_lab1de4(hit);
        ++self->hits.count;
        return free == 1 ? 3 : 1;
    }
    return 2;
}