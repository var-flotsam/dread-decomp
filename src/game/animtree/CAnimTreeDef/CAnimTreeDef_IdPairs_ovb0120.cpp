// animtree::CAnimTreeDef's unit (tools/tu_map.py unit 401): an array (base::UnkVector's layout) of pairs
// of string ids, reserved out of line (fn_71002da674): its copy constructor (fn_71002da558) and copy
// assignment (fn_71002da7a4), each pair appended with both ids retained; the copy (fn_71002d9808) and move
// (fn_71002d9994) assignments of a record holding a word and a pair of ids, both out of line in mode 2;
// and a definition's destructor (fn_71002d8010: the pairs, two lists of owned objects and two string
// ids, the last in the base with vtable g_unk_7101a21718). Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/animtree/CAnimTreeDef.h"
#include "game_s2/UnkOwnedList.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

struct UnkIdPair10Array;
void fn_71002db388(UnkIdPair10Array* self, u32 n);  // truncate

struct UnkAnimTreeIdPairs_b49a1;

struct UnkIdPairRaw_ovb0120 {
    UnkStringData* first;
    UnkStringData* second;
};

struct UnkIdPairArray_ovb0120 {
    UnkIdPairRaw_ovb0120* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    void Reserve(u32 n) { fn_71002da674(reinterpret_cast<UnkAnimTreeIdPairs_b49a1*>(this), n); }
    void PushBack(const UnkIdPairRaw_ovb0120& v) {
        u32 n = count;
        u32 c = capacity;
        if (c <= n && step != 0)
            c = (n / step + 1) * step;
        if ((s32)n >= (s32)c)
            for (;;) {
            }
        Reserve(c);
        UnkIdPairRaw_ovb0120* slot = &data[count];
        UnkStringData* a = v.first;
        __builtin_memcpy(&slot->first, &a, 8);
        UnkStringData* empty = &g_unk_7101d47108->empty;
        if (empty != a)
            ++a->ref_count;
        UnkStringData** second = &slot->second;
        UnkStringData* b = v.second;
        *second = b;
        if (empty != b)
            ++b->ref_count;
        ++count;
    }
    void Append(const UnkIdPairArray_ovb0120& other) {
        for (const UnkIdPairRaw_ovb0120 *p = other.data, *e = p + other.count; p != e; ++p)
            PushBack(*p);
    }
};

void fn_71002da558(UnkIdPairArray_ovb0120* self, const UnkIdPairArray_ovb0120* other) {
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->field_0x14 = other->field_0x14;
    self->flags = other->flags;
    self->allocator = other->allocator;
    self->Reserve(other->capacity);
    self->Append(*other);
}

UnkIdPairArray_ovb0120* fn_71002da7a4(UnkIdPairArray_ovb0120* self, const UnkIdPairArray_ovb0120* other) {
    if (self != other) {
        fn_71002db388(reinterpret_cast<UnkIdPair10Array*>(self), 0);
        self->Reserve(other->capacity);
        self->Append(*other);
    }
    return self;
}

struct UnkIdPair_ovb0120 {
    CStrId first;
    CStrId second;
};

// Mode 2 keeps the value and the pair out of line, reached through the pointers in their slots.
struct UnkAnimTreeVariant_ovb0120 {
    union {
        u32 value;
        u32* value_ptr;
    };
    union {
        UnkIdPair_ovb0120 pair;
        UnkIdPair_ovb0120* pair_ptr;
    };
    u32 mode;  // 0x18

    u32& Value() { return mode == 2 ? *value_ptr : value; }
    const u32& Value() const { return mode == 2 ? *value_ptr : value; }
    UnkIdPair_ovb0120& Pair() { return mode == 2 ? *pair_ptr : pair; }
    const UnkIdPair_ovb0120& Pair() const { return mode == 2 ? *pair_ptr : pair; }
};
static_assert(__builtin_offsetof(UnkAnimTreeVariant_ovb0120, mode) == 0x18, "UnkAnimTreeVariant_ovb0120 layout");

void* fn_71002d9808(void* dst, const void* src) {
    UnkAnimTreeVariant_ovb0120* self = static_cast<UnkAnimTreeVariant_ovb0120*>(dst);
    const UnkAnimTreeVariant_ovb0120* other = static_cast<const UnkAnimTreeVariant_ovb0120*>(src);
    if (self != other) {
        self->Value() = other->Value();
        const UnkIdPair_ovb0120& from = other->Pair();
        UnkIdPair_ovb0120& to = self->Pair();
        to.first = from.first;
        to.second = from.second;
    }
    return self;
}

template <bool Indirect>
static inline void MoveFrom(UnkAnimTreeVariant_ovb0120* self, UnkAnimTreeVariant_ovb0120* other) {
    self->Value() = Indirect ? *other->value_ptr : other->value;
    UnkIdPair_ovb0120& from = Indirect ? *other->pair_ptr : other->pair;
    UnkIdPair_ovb0120& to = self->Pair();
    to.first.ReleaseAndTake(from.first);
    to.second.ReleaseAndTake(from.second);
}

void* fn_71002d9994(void* dst, void* src) {
    UnkAnimTreeVariant_ovb0120* self = static_cast<UnkAnimTreeVariant_ovb0120*>(dst);
    UnkAnimTreeVariant_ovb0120* other = static_cast<UnkAnimTreeVariant_ovb0120*>(src);
    switch (other->mode) {
    case 1:
        MoveFrom<false>(self, other);
        break;
    case 2:
        MoveFrom<true>(self, other);
        break;
    }
    return self;
}

extern void* g_unk_7101a21898[];
extern void* g_unk_7101a21718[];

// An array of pairs of string ids (base::UnkVector's layout), truncated out of line.
struct UnkTreeIdPairs_ovb0120 {
    void* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    void Truncate(u32 n) { fn_71002db388(reinterpret_cast<UnkIdPair10Array*>(this), n); }
    __attribute__((always_inline)) ~UnkTreeIdPairs_ovb0120() {
        if (data) {
            Truncate(0);
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

struct UnkTreeDefBase_ovb0120 {
    void* vtable;
    base::UnkStrIdMember name;  // 0x8

    __attribute__((always_inline)) ~UnkTreeDefBase_ovb0120() { vtable = &g_unk_7101a21718[2]; }
};

struct UnkTreeDef_ovb0120 : UnkTreeDefBase_ovb0120 {
    base::UnkStrIdMember id;  // 0x10
    UnkOwnedList first;  // 0x18
    u64 field_0x40;
    UnkOwnedList second;  // 0x48
    u64 field_0x70;
    UnkTreeIdPairs_ovb0120 pairs;  // 0x78

    __attribute__((always_inline)) ~UnkTreeDef_ovb0120() {
        vtable = &g_unk_7101a21898[2];
        pairs.Truncate(0);
        for (u32 i = 0; i < first.count; ++i)
            first.data[i].~UnkOwnedPtr();
        first.count = 0;
    }
};
static_assert(__builtin_offsetof(UnkTreeDef_ovb0120, pairs) == 0x78, "UnkTreeDef_ovb0120 layout");

void fn_71002d8010(UnkTreeDef_ovb0120* self) { self->~UnkTreeDef_ovb0120(); }
