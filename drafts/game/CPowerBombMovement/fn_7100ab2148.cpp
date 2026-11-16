// CPowerBombMovement's unit (tools/tu_map.py unit 1612): adds a hit record to the 64-record list unless its
// collision component is already listed (the "skip" result); a collision component the record names is also
// added to the list's component pointers. Results: 1 added, 2 full, 3 added and now full.
// Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityShinespark.h"
#include "game/UnkTU365.h"
#include "types.h"

#include <algorithm>
#include <new>

using base::reflection::UnkReflObject;

extern "C" int __cxa_guard_acquire(u64* guard);
extern "C" void __cxa_guard_release(u64* guard);

struct UnkVisitResult_b0040 {
    s32 value;
    UnkVisitResult_b0040(s32 v) : value(v) {}
    UnkVisitResult_b0040(const UnkVisitResult_b0040& other) = default;
    ~UnkVisitResult_b0040() {}
};

static inline UnkVisitResult_b0040 Skip_lab2148() {
    if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(&g_unk_71726a4780), __ATOMIC_ACQUIRE) & 1), 0)) {
        if (__cxa_guard_acquire(&g_unk_71726a4780)) {
            g_unk_71726a4778.value = 0;
            __cxa_guard_release(&g_unk_71726a4780);
        }
    }
    return g_unk_71726a4778;
}

void* fn_7100ab1fa8(void* body);

struct UnkPbCollision_lab2148 : UnkReflObject {
    u8 pad_0x8[0x28];
    void* body;  // 0x30
};

struct UnkPbHolder_lab2148 {
    u8 pad_0x0[0x10];
    UnkReflObject* object;  // 0x10
};

struct UnkPbOwner_lab2148 {
    u8 pad_0x0[0x10];
    UnkPbHolder_lab2148* holder;  // 0x10
};

#pragma pack(push, 4)
struct UnkPbHit_lab2148 {
    UnkPbOwner_lab2148* owner;
    u64 unk_0x8[8];
    UnkPbHit_lab2148(const UnkPbHit_lab2148& other) { fn_7100331c7c(this, &other); }
};


struct UnkPbHitList_lab2148 {
    void* vtable;
    u32 count;                      // 0x8
    UnkPbHit_lab2148 items[64];     // 0xc
    u32 pad_0x120c;
    u32 componentCount;             // 0x1210
    UnkReflObject* components[64];  // 0x1214
};
#pragma pack(pop)
static_assert(sizeof(UnkPbHit_lab2148) == 0x48, "hit");
static_assert(__builtin_offsetof(UnkPbHitList_lab2148, components) == 0x1214, "list");

static inline bool IsCollision(UnkReflObject* object) {
    return fn_710009d50c(object->vf_0x0(), fn_710085fc44());
}

UnkVisitResult_b0040 fn_7100ab2148(UnkPbHitList_lab2148* self, const UnkPbHit_lab2148& hit) {
    UnkReflObject* object = nullptr;
    bool tracked = false;
    if (hit.owner && hit.owner->holder) {
        object = hit.owner->holder->object;
        if (object && IsCollision(object)) {
            void* body = static_cast<UnkPbCollision_lab2148*>(object)->body;
            tracked = body && fn_7100ab1fa8(body);
        }
    }
    if (!tracked) {
        UnkReflObject** begin = self->components;
        UnkReflObject** end = begin + self->componentCount;
        if (std::find(begin, end, object) != end)
            return Skip_lab2148();
    }

    UnkReflObject* component = hit.owner->holder->object;
    if (component && IsCollision(component)) {
        self->components[self->componentCount] = hit.owner->holder->object;
        ++self->componentCount;
    }

    u32 count = self->count;
    s32 free = 64 - (s32)count;
    if (free < 1)
        return 2;
    new (&self->items[count]) UnkPbHit_lab2148(hit);
    ++self->count;
    return free == 1 ? 3 : 1;
}
