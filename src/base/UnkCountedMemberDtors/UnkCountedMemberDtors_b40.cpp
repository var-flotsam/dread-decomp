// Destructors of objects with a member at 0x20 (destroyed by fn_7100216934), a counted resource at
// 0x78 or 0x98 (count at +0x54, freed by fn_710001737c) and owned arrays after it, in units 241, 245,
// 247, 250 and 256 of tools/tu_map.py; the same shape as fn_71001bd92c (src/game_s1/fn_71001bd92c.cpp).
// Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkCountedResource.h"
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "types.h"

struct UnkMember_b40 {
    char storage[0x58];

    __attribute__((always_inline)) ~UnkMember_b40() { fn_7100216934(storage); }
};

// The count is read with s32 alias information (see fn_71001bd92c), so the pointer is cast for the free.
struct UnkCountedRef_b40 {
    char* counted;

    __attribute__((always_inline)) ~UnkCountedRef_b40() {
        if (counted && --*reinterpret_cast<s32*>(counted + 0x54) == 0)
            fn_710001737c(reinterpret_cast<UnkCountedResource*>(counted));
    }
};

// An owned array: its elements destroyed, then the storage handed back to its allocator.
template <class T>
struct UnkOwnedVector_b40 : base::UnkVector<T> {
    __attribute__((always_inline)) ~UnkOwnedVector_b40() { this->Free(); }
};

template <int Offset>
struct UnkCountedObject_b40 {
    u8 pad_0x0[0x20];
    UnkMember_b40 member;
    u8 pad_0x78[Offset - 0x78];
    UnkCountedRef_b40 counted;
};

template <>
struct UnkCountedObject_b40<0x78> {
    u8 pad_0x0[0x20];
    UnkMember_b40 member;
    UnkCountedRef_b40 counted;
};

// Unit 241: an array at 0x80.
struct UnkObject241_b40 {
    u8 pad_0x0[0x20];
    UnkMember_b40 member;
    UnkCountedRef_b40 counted;  // 0x78
    UnkOwnedVector_b40<u64> items;  // 0x80
};

// Unit 250: four arrays from 0xa8.
struct UnkObject250_b40 {
    u8 pad_0x0[0x20];
    UnkMember_b40 member;
    UnkCountedRef_b40 counted;  // 0x78
    u8 pad_0x80[0x28];
    UnkOwnedVector_b40<u64> items[4];  // 0xa8
};

void fn_71001b3f44(UnkObject241_b40* self) { self->~UnkObject241_b40(); }
void fn_71001bb678(UnkCountedObject_b40<0x98>* self) { self->~UnkCountedObject_b40<0x98>(); }
void fn_71001bccc8(UnkObject250_b40* self) { self->~UnkObject250_b40(); }
void fn_71001c3a8c(UnkCountedObject_b40<0x78>* self) { self->~UnkCountedObject_b40<0x78>(); }

// Unit 241 too: two more members (destroyed by fn_710019adec and fn_7100196e7c) and a string id at
// 0x2e8 between the counted resource and the end.
void fn_710019adec(void* member);
void fn_7100196e7c(void* member);

struct UnkSubA_b40 {
    char storage[0x130];

    __attribute__((always_inline)) ~UnkSubA_b40() { fn_710019adec(storage); }
};

struct UnkSubB_b40 {
    char storage[0x138];

    __attribute__((always_inline)) ~UnkSubB_b40() { fn_7100196e7c(storage); }
};

struct UnkObject241b_b40 {
    u8 pad_0x0[0x20];
    UnkMember_b40 member;
    UnkCountedRef_b40 counted;  // 0x78
    UnkSubA_b40 sub_a;  // 0x80
    UnkSubB_b40 sub_b;  // 0x1b0
    base::UnkStrIdMember id;  // 0x2e8
};

void fn_71001b4d40(UnkObject241b_b40* self) { self->~UnkObject241b_b40(); }

// Unit 247: a member destroyed by fn_710016a714 at 0x80 and an array of string ids at 0xd8.
void fn_710016a714(char* self);

struct UnkSubC_b40 {
    char storage[0x58];

    __attribute__((always_inline)) ~UnkSubC_b40() { fn_710016a714(storage); }
};

struct UnkObject247_b40 {
    u8 pad_0x0[0x20];
    UnkMember_b40 member;
    u8 pad_0x78[0x8];
    UnkSubC_b40 sub;  // 0x80
    UnkOwnedVector_b40<base::UnkStrIdMember> ids;  // 0xd8
};

void fn_71001bbbb8(UnkObject247_b40* self) { self->~UnkObject247_b40(); }
