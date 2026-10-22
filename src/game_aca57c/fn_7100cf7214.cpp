// Copies of a copy constructor through three vtables: the root's (0x7101a1a138, two words
// at 8), the middle class's (0x7101aa8110) with a string id at 0x18, two words and a word from
// 0x20, then the class's (0x7101aa6cd8) with four string ids from 0x38 and a byte at 0x58; its
// final vtable is 0x7101aa6d08.
// Also other classes on the same root: groups of words and string ids, a few string ids with a
// word before or after, and a class on another middle class.
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

extern void* g_unk_7101a1a138[];
extern void* g_unk_7101aa8110[];
extern void* g_unk_7101aa6cd8[];
extern void* g_unk_7101aa6d08[];
extern void* g_unk_7101aa7b70[];
extern void* g_unk_7101aa7c88[];

struct UnkWords2 {
    u64 field_0x0;
    u64 field_0x8;
};

struct UnkRoot {
    void** vtable;
    UnkWords2 words;

    __attribute__((always_inline)) UnkRoot(const UnkRoot& other) : words((SetVtable(g_unk_7101a1a138), other.words)) {}
    // A plain pointer store, as the compiler emits for a real class's vtable.
    __attribute__((always_inline)) void SetVtable(void** v) { *reinterpret_cast<void***>(this) = v + 2; }
};

// The vtables are set before the members are copied, as the compiler does for a real class.
template <void** Vtable>
struct UnkIdMiddleT : UnkRoot {
    CStrId id;
    UnkWords2 words_0x20;
    u32 field_0x30;

    __attribute__((always_inline)) UnkIdMiddleT(const UnkIdMiddleT& other)
        : UnkRoot(other), id((SetVtable(Vtable), other.id)), words_0x20(other.words_0x20),
          field_0x30(other.field_0x30) {}
};
using UnkIdMiddle = UnkIdMiddleT<g_unk_7101aa8110>;

struct UnkFourIds : UnkIdMiddle {
    CStrId ids[4];
    bool field_0x58;

    __attribute__((always_inline)) UnkFourIds(const UnkFourIds& other)
        : UnkIdMiddle(other), ids{(SetVtable(g_unk_7101aa6cd8), other.ids[0]), other.ids[1], other.ids[2], other.ids[3]},
          field_0x58(other.field_0x58) {}
};

__attribute__((always_inline)) static inline void Copy(UnkFourIds* self, const UnkFourIds* other) {
    new (self) UnkFourIds(*other);
    self->SetVtable(g_unk_7101aa6d08);
}

// The base class's copy (without the final vtable).
void fn_7100cf6b68(UnkFourIds* self, const UnkFourIds* other) { new (self) UnkFourIds(*other); }
void fn_7100cf6c60(UnkFourIds* self, const UnkFourIds* other) { new (self) UnkFourIds(*other); }

void fn_7100cf7214(UnkFourIds* self, const UnkFourIds* other) { Copy(self, other); }
void fn_7100cf731c(UnkFourIds* self, const UnkFourIds* other) { Copy(self, other); }

// Another class on the same root (vtable 0x7101aa7b70): three groups of two words and two string
// ids from 0x18, then a word.
struct UnkWordsIds {
    UnkWords2 words;
    CStrId id_a;
    CStrId id_b;
};

struct UnkThreeGroups : UnkRoot {
    UnkWordsIds groups[3];
    u64 field_0x78;

    __attribute__((always_inline)) UnkThreeGroups(const UnkThreeGroups& other)
        : UnkRoot(other), groups{(SetVtable(g_unk_7101aa7b70), other.groups[0]), other.groups[1], other.groups[2]},
          field_0x78(other.field_0x78) {}
};

void fn_7100d189a0(UnkThreeGroups* self, const UnkThreeGroups* other) { new (self) UnkThreeGroups(*other); }
void fn_7100d18aa4(UnkThreeGroups* self, const UnkThreeGroups* other) { new (self) UnkThreeGroups(*other); }

// Another class on the same root (vtable 0x7101aa7c88) with four string ids from 0x18.
struct UnkRootFourIds : UnkRoot {
    CStrId ids[4];

    __attribute__((always_inline)) UnkRootFourIds(const UnkRootFourIds& other)
        : UnkRoot(other), ids{(SetVtable(g_unk_7101aa7c88), other.ids[0]), other.ids[1], other.ids[2], other.ids[3]} {}
};

void fn_7100d1b954(UnkRootFourIds* self, const UnkRootFourIds* other) { new (self) UnkRootFourIds(*other); }
void fn_7100d1ba00(UnkRootFourIds* self, const UnkRootFourIds* other) { new (self) UnkRootFourIds(*other); }

// Classes on the same root with two or three string ids from 0x18 (a word may follow), or a word
// at 0x18 and then the string ids from 0x20.
template <void** Vtable>
struct UnkRootTwoIds : UnkRoot {
    CStrId ids[2];

    __attribute__((always_inline)) UnkRootTwoIds(const UnkRootTwoIds& other)
        : UnkRoot(other), ids{(SetVtable(Vtable), other.ids[0]), other.ids[1]} {}
};

template <void** Vtable>
struct UnkRootTwoIdsWord : UnkRootTwoIds<Vtable> {
    u64 field_0x28;
};

template <void** Vtable>
struct UnkRootThreeIdsWord : UnkRoot {
    CStrId ids[3];
    u32 field_0x30;

    __attribute__((always_inline)) UnkRootThreeIdsWord(const UnkRootThreeIdsWord& other)
        : UnkRoot(other), ids{(SetVtable(Vtable), other.ids[0]), other.ids[1], other.ids[2]},
          field_0x30(other.field_0x30) {}
};

template <int N>
struct UnkIdArray {
    CStrId ids[N];
};

template <void** Vtable, int N>
struct UnkRootWordIds : UnkRoot {
    u32 field_0x18;
    UnkIdArray<N> ids;

    __attribute__((always_inline)) UnkRootWordIds(const UnkRootWordIds& other)
        : UnkRoot(other), field_0x18((SetVtable(Vtable), other.field_0x18)), ids(other.ids) {}
};

template <class T>
__attribute__((always_inline)) static inline void CopyConstruct(T* self, const T* other) {
    new (self) T(*other);
}

extern void* g_unk_7101aa2230[];
extern void* g_unk_7101aa2258[];
extern void* g_unk_7101aa30a8[];
extern void* g_unk_7101aa3508[];
extern void* g_unk_7101aa41f8[];
extern void* g_unk_7101aa68c8[];
extern void* g_unk_7101aa7080[];

void fn_7100bf1ca0(UnkRootTwoIds<g_unk_7101aa2230>* self, const UnkRootTwoIds<g_unk_7101aa2230>* other) { CopyConstruct(self, other); }
void fn_7100bf1d14(UnkRootTwoIds<g_unk_7101aa2230>* self, const UnkRootTwoIds<g_unk_7101aa2230>* other) { CopyConstruct(self, other); }
void fn_7100bf2590(UnkRootTwoIds<g_unk_7101aa2258>* self, const UnkRootTwoIds<g_unk_7101aa2258>* other) { CopyConstruct(self, other); }
void fn_7100bf2604(UnkRootTwoIds<g_unk_7101aa2258>* self, const UnkRootTwoIds<g_unk_7101aa2258>* other) { CopyConstruct(self, other); }
void fn_7100c1f5bc(UnkRootTwoIds<g_unk_7101aa30a8>* self, const UnkRootTwoIds<g_unk_7101aa30a8>* other) { CopyConstruct(self, other); }
void fn_7100c1f630(UnkRootTwoIds<g_unk_7101aa30a8>* self, const UnkRootTwoIds<g_unk_7101aa30a8>* other) { CopyConstruct(self, other); }
void fn_7100c29e58(UnkRootTwoIdsWord<g_unk_7101aa3508>* self, const UnkRootTwoIdsWord<g_unk_7101aa3508>* other) { CopyConstruct(self, other); }
void fn_7100c29ed4(UnkRootTwoIdsWord<g_unk_7101aa3508>* self, const UnkRootTwoIdsWord<g_unk_7101aa3508>* other) { CopyConstruct(self, other); }
void fn_7100c45fe0(UnkRootThreeIdsWord<g_unk_7101aa41f8>* self, const UnkRootThreeIdsWord<g_unk_7101aa41f8>* other) { CopyConstruct(self, other); }
void fn_7100c46078(UnkRootThreeIdsWord<g_unk_7101aa41f8>* self, const UnkRootThreeIdsWord<g_unk_7101aa41f8>* other) { CopyConstruct(self, other); }
void fn_7100ced7d0(UnkRootWordIds<g_unk_7101aa68c8, 3>* self, const UnkRootWordIds<g_unk_7101aa68c8, 3>* other) { CopyConstruct(self, other); }
void fn_7100ced868(UnkRootWordIds<g_unk_7101aa68c8, 3>* self, const UnkRootWordIds<g_unk_7101aa68c8, 3>* other) { CopyConstruct(self, other); }
void fn_7100d008d0(UnkRootWordIds<g_unk_7101aa7080, 2>* self, const UnkRootWordIds<g_unk_7101aa7080, 2>* other) { CopyConstruct(self, other); }
void fn_7100d0094c(UnkRootWordIds<g_unk_7101aa7080, 2>* self, const UnkRootWordIds<g_unk_7101aa7080, 2>* other) { CopyConstruct(self, other); }

// A class on another middle class (vtable 0x7101aa8090) with three string ids from 0x38 and a
// word; its vtable is 0x7101aa4890.
extern void* g_unk_7101aa8090[];
extern void* g_unk_7101aa4890[];

// (The middle class copies its two words and word as one 0x14-byte block.)
struct UnkWords2Word {
    u64 field_0x0;
    u64 field_0x8;
    u32 field_0x10;
};

struct UnkIdBlockMiddle : UnkRoot {
    CStrId id;
    UnkWords2Word block;

    __attribute__((always_inline)) UnkIdBlockMiddle(const UnkIdBlockMiddle& other)
        : UnkRoot(other), id((SetVtable(g_unk_7101aa8090), other.id)) {
        __builtin_memcpy(&block, &other.block, 0x14);
    }
};

struct UnkThreeIdsOnMiddle : UnkIdBlockMiddle {
    CStrId ids[3];
    u64 field_0x50;

    __attribute__((always_inline)) UnkThreeIdsOnMiddle(const UnkThreeIdsOnMiddle& other)
        : UnkIdBlockMiddle(other), ids{(SetVtable(g_unk_7101aa4890), other.ids[0]), other.ids[1], other.ids[2]},
          field_0x50(other.field_0x50) {}
};

void fn_7100c545f0(UnkThreeIdsOnMiddle* self, const UnkThreeIdsOnMiddle* other) { CopyConstruct(self, other); }
void fn_7100c546cc(UnkThreeIdsOnMiddle* self, const UnkThreeIdsOnMiddle* other) { CopyConstruct(self, other); }
