// Copies of a copy constructor through three vtables: the root's (0x7101a1a138, two words
// at 8), the middle class's (0x7101aa8110) with a string id at 0x18, two words and a word from
// 0x20, then the class's (0x7101aa6cd8) with four string ids from 0x38 and a byte at 0x58; its
// final vtable is 0x7101aa6d08.
// Also other classes on the same root: groups of words and string ids, a few string ids with a
// word before or after, and a class on another middle class. Also default constructors on the
// root that take the class object (from its refl_GetClass), with empty or global string ids.
#include <new>

#include "base/UnkVector.h"
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

struct UnkClass;
struct UnkRoot;
void fn_710006afb8(UnkRoot* self, UnkClass* cls);  // the root's constructor
extern CStrId g_unk_7101d470e0;

struct UnkRoot {
    void** vtable;
    UnkWords2 words;

    __attribute__((always_inline)) explicit UnkRoot(UnkClass* cls) { fn_710006afb8(this, cls); }

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

    UnkWordsIds(const UnkWordsIds& other) = default;
    // The words hold four floats, all 1.0f; the ids are empty and "Constant0Color".
    __attribute__((always_inline)) UnkWordsIds()
        : words{0x3f8000003f800000, 0x3f8000003f800000}, id_a("", true), id_b("Constant0Color", true) {}
};

struct UnkThreeGroups : UnkRoot {
    UnkWordsIds groups[3];
    u64 field_0x78;

    __attribute__((always_inline)) UnkThreeGroups(const UnkThreeGroups& other)
        : UnkRoot(other), groups{(SetVtable(g_unk_7101aa7b70), other.groups[0]), other.groups[1], other.groups[2]},
          field_0x78(other.field_0x78) {}
    __attribute__((always_inline)) explicit UnkThreeGroups(UnkClass* cls)
        : UnkRoot(cls), groups{(SetVtable(g_unk_7101aa7b70), UnkWordsIds()), UnkWordsIds(), UnkWordsIds()},
          field_0x78(0) {}
};

void fn_7100d189a0(UnkThreeGroups* self, const UnkThreeGroups* other) { new (self) UnkThreeGroups(*other); }
void fn_7100d18aa4(UnkThreeGroups* self, const UnkThreeGroups* other) { new (self) UnkThreeGroups(*other); }

// Another class on the same root (vtable 0x7101aa7c88) with four string ids from 0x18.
struct UnkRootFourIds : UnkRoot {
    CStrId ids[4];

    __attribute__((always_inline)) UnkRootFourIds(const UnkRootFourIds& other)
        : UnkRoot(other), ids{(SetVtable(g_unk_7101aa7c88), other.ids[0]), other.ids[1], other.ids[2], other.ids[3]} {}
    __attribute__((always_inline)) explicit UnkRootFourIds(UnkClass* cls)
        : UnkRoot(cls), ids{(SetVtable(g_unk_7101aa7c88), CStrId("", true)), CStrId("", true), CStrId("", true),
                            CStrId("", true)} {}
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
    // Both ids start as copies of the global id g_unk_7101d470e0.
    __attribute__((always_inline)) explicit UnkRootTwoIds(UnkClass* cls)
        : UnkRoot(cls), ids{(SetVtable(Vtable), g_unk_7101d470e0), g_unk_7101d470e0} {}
};

template <void** Vtable>
struct UnkRootTwoIdsWord : UnkRootTwoIds<Vtable> {
    u64 field_0x28;
};

// The same layout built with empty ids and a zero word.
template <void** Vtable>
struct UnkRootTwoEmptyIdsWord : UnkRoot {
    CStrId ids[2];
    u64 field_0x28;

    __attribute__((always_inline)) explicit UnkRootTwoEmptyIdsWord(UnkClass* cls)
        : UnkRoot(cls), ids{(SetVtable(Vtable), CStrId("", true)), CStrId("", true)}, field_0x28(0) {}
};

template <void** Vtable>
struct UnkRootThreeIdsWord : UnkRoot {
    CStrId ids[3];
    u32 field_0x30;

    __attribute__((always_inline)) UnkRootThreeIdsWord(const UnkRootThreeIdsWord& other)
        : UnkRoot(other), ids{(SetVtable(Vtable), other.ids[0]), other.ids[1], other.ids[2]},
          field_0x30(other.field_0x30) {}
    // Empty ids; the word is left unset.
    __attribute__((always_inline)) explicit UnkRootThreeIdsWord(UnkClass* cls)
        : UnkRoot(cls), ids{(SetVtable(Vtable), CStrId("", true)), CStrId("", true), CStrId("", true)} {}
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
    // The word starts as -1.0f (bits 0xbf800000) and the (three) ids empty.
    __attribute__((always_inline)) explicit UnkRootWordIds(UnkClass* cls)
        : UnkRoot(cls), field_0x18((SetVtable(Vtable), 0xbf800000)),
          ids{{CStrId("", true), CStrId("", true), CStrId("", true)}} {}
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

// Constructors: the class is registered (its refl_GetClass) and its object passed to the root.
UnkClass* fn_7100bf1638();  // CCharClassAllowCrouchRepositionTrack::refl_GetClass
extern UnkClass g_unk_7172777d38 __attribute__((visibility("hidden")));

void fn_7100bf1c0c(UnkRootTwoIds<g_unk_7101aa2230>* self) {
    fn_7100bf1638();
    new (self) UnkRootTwoIds<g_unk_7101aa2230>(&g_unk_7172777d38);
}

UnkClass* fn_7100bf1f28();  // CCharClassAllowFallendRepositionTrack::refl_GetClass
UnkClass* fn_7100c291f8();  // CCharClassIgnoreCollisionMaskTrack::refl_GetClass
UnkClass* fn_7100c455b8();  // CCharClassPlayTimelineTrack::refl_GetClass
UnkClass* fn_7100cecf98();  // CCharClassAttackFailActionTrack::refl_GetClass
extern UnkClass g_unk_7172777e98 __attribute__((visibility("hidden")));
extern UnkClass g_unk_7172783918 __attribute__((visibility("hidden")));
extern UnkClass g_unk_717278a9e8 __attribute__((visibility("hidden")));
extern UnkClass g_unk_71727a0f98 __attribute__((visibility("hidden")));

void fn_7100bf24fc(UnkRootTwoIds<g_unk_7101aa2258>* self) {
    fn_7100bf1f28();
    new (self) UnkRootTwoIds<g_unk_7101aa2258>(&g_unk_7172777e98);
}
void fn_7100c29dec(UnkRootTwoEmptyIdsWord<g_unk_7101aa3508>* self) {
    fn_7100c291f8();
    new (self) UnkRootTwoEmptyIdsWord<g_unk_7101aa3508>(&g_unk_7172783918);
}
void fn_7100ced754(UnkRootWordIds<g_unk_7101aa68c8, 3>* self) {
    fn_7100cecf98();
    new (self) UnkRootWordIds<g_unk_7101aa68c8, 3>(&g_unk_71727a0f98);
}
// NON_MATCHING(minor): the vtable store post-increments to the second id instead of the third,
// so the ids' addresses are formed the other way round.
void fn_7100c45f60(UnkRootThreeIdsWord<g_unk_7101aa41f8>* self) {
    fn_7100c455b8();
    new (self) UnkRootThreeIdsWord<g_unk_7101aa41f8>(&g_unk_717278a9e8);
}

// Classes on the root with an empty string id and an empty array (flags 0xf), or an empty string
// id, a flag and a word.
template <void** Vtable>
struct UnkRootIdArray : UnkRoot {
    CStrId id;
    base::UnkVector<u64> values;

    __attribute__((always_inline)) explicit UnkRootIdArray(UnkClass* cls)
        : UnkRoot(cls), id((SetVtable(Vtable), CStrId("", true))), values(0xf) {}
};

template <void** Vtable>
struct UnkRootIdFlag : UnkRoot {
    CStrId id;
    bool field_0x20;
    u32 field_0x24;

    __attribute__((always_inline)) explicit UnkRootIdFlag(UnkClass* cls)
        : UnkRoot(cls), id((SetVtable(Vtable), CStrId("", true))), field_0x20(false), field_0x24(0) {}
};

extern void* g_unk_7101aa3030[];
extern void* g_unk_7101aa3440[];
extern void* g_unk_7101aa3f20[];
extern void* g_unk_7101aa3f70[];
extern void* g_unk_7101aa41a8[];
UnkClass* fn_7100c1d4b8();  // CCharClassForceBumpFXTrack::refl_GetClass
UnkClass* fn_7100c26e68();  // CCharClassIgnoreBumpFXTrack::refl_GetClass
UnkClass* fn_7100c3e258();  // CCharClassMeleeFailActionTrack::refl_GetClass
UnkClass* fn_7100c3f0d8();  // CCharClassMeleeSuccessBigAIActionTrack::refl_GetClass
UnkClass* fn_7100c44688();  // CCharClassOverrideSamusMeleeActionTrack::refl_GetClass
extern UnkClass g_unk_7172780e78 __attribute__((visibility("hidden")));
extern UnkClass g_unk_7172783238 __attribute__((visibility("hidden")));
extern UnkClass g_unk_7172789118 __attribute__((visibility("hidden")));
extern UnkClass g_unk_71727893d8 __attribute__((visibility("hidden")));
extern UnkClass g_unk_717278a718 __attribute__((visibility("hidden")));

void fn_7100c1def4(UnkRootIdArray<g_unk_7101aa3030>* self) {
    fn_7100c1d4b8();
    new (self) UnkRootIdArray<g_unk_7101aa3030>(&g_unk_7172780e78);
}
void fn_7100c278a4(UnkRootIdArray<g_unk_7101aa3440>* self) {
    fn_7100c26e68();
    new (self) UnkRootIdArray<g_unk_7101aa3440>(&g_unk_7172783238);
}
void fn_7100c3e884(UnkRootIdFlag<g_unk_7101aa3f20>* self) {
    fn_7100c3e258();
    new (self) UnkRootIdFlag<g_unk_7101aa3f20>(&g_unk_7172789118);
}
void fn_7100c3f704(UnkRootIdFlag<g_unk_7101aa3f70>* self) {
    fn_7100c3f0d8();
    new (self) UnkRootIdFlag<g_unk_7101aa3f70>(&g_unk_71727893d8);
}
void fn_7100c44cb4(UnkRootIdFlag<g_unk_7101aa41a8>* self) {
    fn_7100c44688();
    new (self) UnkRootIdFlag<g_unk_7101aa41a8>(&g_unk_717278a718);
}


UnkClass* fn_7100d1b068();  // CCharClassModifyBumpedActionTrack::refl_GetClass
extern UnkClass g_unk_71727abcd8 __attribute__((visibility("hidden")));

// NON_MATCHING(minor): as fn_7100c45f60 (the post-increment goes to the second id, not the fourth).
void fn_7100d1b8c4(UnkRootFourIds* self) {
    fn_7100d1b068();
    new (self) UnkRootFourIds(&g_unk_71727abcd8);
}

// More root classes: an empty string id and two cleared flags; two empty ids, two set flags and
// an empty id; and two pairs of an empty id and an empty array (flags 5).
template <void** Vtable>
struct UnkRootIdTwoFlags : UnkRoot {
    CStrId id;
    bool field_0x20;
    bool field_0x21;

    __attribute__((always_inline)) explicit UnkRootIdTwoFlags(UnkClass* cls)
        : UnkRoot(cls), id((SetVtable(Vtable), CStrId("", true))), field_0x20(false), field_0x21(false) {}
};

template <void** Vtable>
struct UnkRootIdsFlagsId : UnkRoot {
    CStrId ids[2];
    bool field_0x28;
    bool field_0x29;
    CStrId id_0x30;

    __attribute__((always_inline)) explicit UnkRootIdsFlagsId(UnkClass* cls)
        : UnkRoot(cls), ids{(SetVtable(Vtable), CStrId("", true)), CStrId("", true)}, field_0x28(true),
          field_0x29(true), id_0x30("", true) {}
};

struct UnkIdArrayPair {
    CStrId id;
    base::UnkVector<u64> values;

    __attribute__((always_inline)) UnkIdArrayPair() : id("", true), values(5) {}
};

template <void** Vtable>
struct UnkRootTwoIdArrays : UnkRoot {
    UnkIdArrayPair first;
    UnkIdArrayPair second;

    __attribute__((always_inline)) explicit UnkRootTwoIdArrays(UnkClass* cls)
        : UnkRoot(cls), first((SetVtable(Vtable), UnkIdArrayPair())) {}
};

extern void* g_unk_7101aa7008[];
extern void* g_unk_7101aa7030[];
extern void* g_unk_7101aa6f40[];
extern void* g_unk_7101aa7c38[];
UnkClass* fn_7100cfe868();  // CCharClassGrabbedTrackTrack::refl_GetClass
UnkClass* fn_7100cff148();  // CCharClassGrabberTrackTrack::refl_GetClass
UnkClass* fn_7100cfbd38();  // CCharClassFXControlTrack::refl_GetClass
UnkClass* fn_7100d1a278();  // CCharClassMeleeSuccessActionTrack::refl_GetClass
extern UnkClass g_unk_71727a4ed8 __attribute__((visibility("hidden")));
extern UnkClass g_unk_71727a5038 __attribute__((visibility("hidden")));
extern UnkClass g_unk_71727a47f8 __attribute__((visibility("hidden")));
extern UnkClass g_unk_71727aba18 __attribute__((visibility("hidden")));

void fn_7100cfee94(UnkRootIdTwoFlags<g_unk_7101aa7008>* self) {
    fn_7100cfe868();
    new (self) UnkRootIdTwoFlags<g_unk_7101aa7008>(&g_unk_71727a4ed8);
}
void fn_7100cff9a4(UnkRootIdsFlagsId<g_unk_7101aa7030>* self) {
    fn_7100cff148();
    new (self) UnkRootIdsFlagsId<g_unk_7101aa7030>(&g_unk_71727a5038);
}
void fn_7100cfcd68(UnkRootTwoIdArrays<g_unk_7101aa6f40>* self) {
    fn_7100cfbd38();
    new (self) UnkRootTwoIdArrays<g_unk_7101aa6f40>(&g_unk_71727a47f8);
}
void fn_7100d1a8a4(UnkRootIdFlag<g_unk_7101aa7c38>* self) {
    fn_7100d1a278();
    new (self) UnkRootIdFlag<g_unk_7101aa7c38>(&g_unk_71727aba18);
}

// CCharClassMaterialPropertyTransitionTrack's constructor (the three-group class above).
UnkClass* fn_7100d17558();  // CCharClassMaterialPropertyTransitionTrack::refl_GetClass
extern UnkClass g_unk_71727ab338 __attribute__((visibility("hidden")));

void fn_7100d1867c(UnkThreeGroups* self) {
    fn_7100d17558();
    new (self) UnkThreeGroups(&g_unk_71727ab338);
}

// An empty string id and an empty array (flags 0xf), then another empty id and a zero word.
template <void** Vtable>
struct UnkRootIdArrayIdWord : UnkRootIdArray<Vtable> {
    CStrId id_0x48;
    u32 field_0x50;

    __attribute__((always_inline)) explicit UnkRootIdArrayIdWord(UnkClass* cls)
        : UnkRootIdArray<Vtable>(cls), id_0x48("", true), field_0x50(0) {}
};

extern void* g_unk_7101aa2b80[];
extern void* g_unk_7101aa7d00[];
UnkClass* fn_7100c12238();  // CCharClassCooldownXBossDamageShapesTrack::refl_GetClass
UnkClass* fn_7100d217d8();  // CCharClassModifyDamageTrack::refl_GetClass
extern UnkClass g_unk_717277e538 __attribute__((visibility("hidden")));
extern UnkClass g_unk_71727ac0f8 __attribute__((visibility("hidden")));

void fn_7100c12f5c(UnkRootIdArrayIdWord<g_unk_7101aa2b80>* self) {
    fn_7100c12238();
    new (self) UnkRootIdArrayIdWord<g_unk_7101aa2b80>(&g_unk_717277e538);
}
void fn_7100d223f0(UnkRootIdArrayIdWord<g_unk_7101aa7d00>* self) {
    fn_7100d217d8();
    new (self) UnkRootIdArrayIdWord<g_unk_7101aa7d00>(&g_unk_71727ac0f8);
}

// Two ids set to the empty id directly (the pool is created first if there is none yet).
struct UnkDirectEmptyId {
    base::global::UnkStringData* data;

    __attribute__((always_inline)) UnkDirectEmptyId() {
        data = nullptr;
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        data = &g_unk_7101d47108->empty;
    }
};

template <void** Vtable>
struct UnkRootTwoDirectEmptyIds : UnkRoot {
    UnkDirectEmptyId ids[2];

    __attribute__((always_inline)) explicit UnkRootTwoDirectEmptyIds(UnkClass* cls)
        : UnkRoot(cls), ids{(SetVtable(Vtable), UnkDirectEmptyId()), UnkDirectEmptyId()} {}
};

UnkClass* fn_7100c1ee78();  // CCharClassForceRotationOnActionTrack::refl_GetClass
extern UnkClass g_unk_7172781298 __attribute__((visibility("hidden")));

void fn_7100c1f318(UnkRootTwoDirectEmptyIds<g_unk_7101aa30a8>* self) {
    fn_7100c1ee78();
    new (self) UnkRootTwoDirectEmptyIds<g_unk_7101aa30a8>(&g_unk_7172781298);
}
