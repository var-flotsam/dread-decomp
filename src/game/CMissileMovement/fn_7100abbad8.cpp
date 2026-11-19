// CMissileMovement unit (tools/tu_map.py unit 1618): adds a hit record to the 32-record list, then sorts
// the list nearest first with the list's comparator (std::sort, fn_7100abbd40).
// Results: 1 added, 2 full, 3 added and now full. Names and layouts are placeholders.
#include "game/CAbilityShinespark.h"
#include "types.h"

#include <new>

struct UnkVisitResult_b0040 {
    s32 value;
    UnkVisitResult_b0040(s32 v) : value(v) {}
    UnkVisitResult_b0040(const UnkVisitResult_b0040& other) = default;
    ~UnkVisitResult_b0040() {}
};

struct UnkEntry_7100abbd40;
struct UnkCmp_7100abbd40;
void fn_7100abbd40(UnkEntry_7100abbd40*, UnkEntry_7100abbd40*, UnkCmp_7100abbd40&);

#pragma pack(push, 4)
struct UnkMissileHit_labbad8 {
    u64 unk_0x0[9];
    UnkMissileHit_labbad8(const UnkMissileHit_labbad8& other) { fn_7100331c7c(this, &other); }
};
#pragma pack(pop)
static_assert(sizeof(UnkMissileHit_labbad8) == 0x48, "hit");

// The comparator: the point the hits are sorted by distance from.
struct UnkMissileHitOrder_labbad8 {
    f32 x;
    f32 y;
    f32 z;
};

// A fixed-capacity list: a count, then room for N elements.
template <typename T, int N>
struct UnkMissileFixedList_labbad8 {
    u32 count;
    u8 storage[N * sizeof(T)];

    T* begin() { return reinterpret_cast<T*>(storage); }
    T* end() { return begin() + count; }

    // Copies the element in when there is room: 1 added, 2 full, 3 added and now full.
    s32 Add(const T& value) {
        s32 free = N - count;
        if (free > 0) {
            new (&begin()[count]) T(value);
            ++count;
            return free == 1 ? 3 : 1;
        }
        return 2;
    }
};

struct UnkMissileHitList_labbad8 {
    u8 pad_0x0[0x8];
    UnkMissileFixedList_labbad8<UnkMissileHit_labbad8, 32> hits;  // 0x8
    UnkMissileHitOrder_labbad8 order;                             // 0x90c
};
static_assert(__builtin_offsetof(UnkMissileHitList_labbad8, order) == 0x90c, "order");

// NON_MATCHING(functional): Conditions differ from the original; behaves the same on every input tools/equiv_test.py --any-kind tried, with full coverage. Owes a match.
UnkVisitResult_b0040 fn_7100abbad8(UnkMissileHitList_labbad8* self, const UnkMissileHit_labbad8& hit) {
    UnkVisitResult_b0040 result = self->hits.Add(hit);
    UnkMissileHitOrder_labbad8 order = self->order;
    fn_7100abbd40(reinterpret_cast<UnkEntry_7100abbd40*>(self->hits.begin()),
                  reinterpret_cast<UnkEntry_7100abbd40*>(self->hits.end()),
                  reinterpret_cast<UnkCmp_7100abbd40&>(order));
    return result;
}
