// Copies of a copy constructor through three vtables: the root's (0x7101a1a138, two words
// at 8), the middle class's (0x7101aa8110) with a string id at 0x18, two words and a word from
// 0x20, then the class's (0x7101aa6cd8) with four string ids from 0x38 and a byte at 0x58; its
// final vtable is 0x7101aa6d08.
// Also another class on the same root with three groups of words and string ids.
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

extern void* g_unk_7101a1a138[];
extern void* g_unk_7101aa8110[];
extern void* g_unk_7101aa6cd8[];
extern void* g_unk_7101aa6d08[];
extern void* g_unk_7101aa7b70[];

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
struct UnkIdMiddle : UnkRoot {
    CStrId id;
    UnkWords2 words_0x20;
    u32 field_0x30;

    __attribute__((always_inline)) UnkIdMiddle(const UnkIdMiddle& other)
        : UnkRoot(other), id((SetVtable(g_unk_7101aa8110), other.id)), words_0x20(other.words_0x20),
          field_0x30(other.field_0x30) {}
};

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
