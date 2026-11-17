// CSamusAirMovementState::CParams's unit (tools/tu_map.py unit 1455): a target visitor that keeps up to four
// targets in a fixed list at +0x8 (a count, then the records). A target whose two vectors (words 1-3 and
// 7-9) lie within 0.001 of a kept one's is skipped (the guarded "skip" result); otherwise it is added.
// Results: 1 added, 2 full, 3 added and now full. Names and layouts are placeholders.
#include "game/CAbilityShinespark.h"
#include "game/CAimComponent.h"
#include "game/UnkTU365.h"
#include "types.h"

#include <new>

extern "C" int __cxa_guard_acquire(u64* guard);
extern "C" void __cxa_guard_release(u64* guard);

struct UnkVisitResult_b0040 {
    s32 value;
    UnkVisitResult_b0040(s32 v) : value(v) {}
    UnkVisitResult_b0040(const UnkVisitResult_b0040& other) = default;
    ~UnkVisitResult_b0040() {}
};

static inline UnkVisitResult_b0040 Skip_l98de6c() {
    if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(&g_unk_71726a4780), __ATOMIC_ACQUIRE) & 1), 0)) {
        if (__cxa_guard_acquire(&g_unk_71726a4780)) {
            g_unk_71726a4778.value = 0;
            __cxa_guard_release(&g_unk_71726a4780);
        }
    }
    return g_unk_71726a4778;
}

// A fixed-capacity list: a count, then room for N elements.
template <typename T, int N>
struct UnkFixedList_l98de6c {
    u32 count;
    u8 storage[N * sizeof(T)];

    T* begin() { return reinterpret_cast<T*>(storage); }
    T* end() { return begin() + count; }
};

struct UnkTargetSet_l98de6c {
    void* vtable;
    UnkFixedList_l98de6c<UnkAimTarget, 4> targets;  // 0x8
};

static inline f32 Word_l98de6c(const UnkAimTarget& target, int i) {
    return *reinterpret_cast<const f32*>(&target.words[i]);
}

// Whether value lies within 0.001 of center.
static inline bool Near_l98de6c(f32 center, f32 value) {
    return center - 0.001f <= value && center + 0.001f >= value;
}

static inline bool Same_l98de6c(const UnkAimTarget& kept, const UnkAimTarget* target) {
    return Near_l98de6c(Word_l98de6c(kept, 1), Word_l98de6c(*target, 1)) &&
           Near_l98de6c(Word_l98de6c(kept, 2), Word_l98de6c(*target, 2)) &&
           Near_l98de6c(Word_l98de6c(kept, 3), Word_l98de6c(*target, 3)) &&
           Near_l98de6c(Word_l98de6c(kept, 7), Word_l98de6c(*target, 7)) &&
           Near_l98de6c(Word_l98de6c(kept, 8), Word_l98de6c(*target, 8)) &&
           Near_l98de6c(Word_l98de6c(kept, 9), Word_l98de6c(*target, 9));
}

UnkVisitResult_b0040 fn_710098de6c(UnkTargetSet_l98de6c* self, const UnkAimTarget* target) {
    for (UnkAimTarget* kept = self->targets.begin(); kept != self->targets.end(); ++kept) {
        if (Same_l98de6c(*kept, target))
            return Skip_l98de6c();
    }
    u32 count = self->targets.count;
    s32 free = 4 - count;
    if (free > 0) {
        new (&self->targets.begin()[count]) UnkAimTarget(*target);
        ++self->targets.count;
        return free == 1 ? 3 : 1;
    }
    return 2;
}
