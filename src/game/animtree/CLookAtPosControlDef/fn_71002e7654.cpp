// Copy and move constructors (fn_71002e76f0) of a three-level class: the vtable is rewritten per level; the first two
// levels each hold a string id (copied by taking another reference), the last a block of
// plain data.
#include <new>
#include "types.h"
#include "base/global/CStrId.h"

using base::global::UnkStringData;

// Copies a string id: the same string with one more reference (unless it is the empty one).
static inline void CopyId(UnkStringData*& dst, UnkStringData* src) {
    dst = src;
    if (&g_unk_7101d47108->empty != src)
        ++src->ref_count;
}

extern void* g_unk_7101a21718[];
extern void* g_unk_7101a21af8[];
extern void* g_unk_7101a21fc8[];

struct UnkCopySelf {
    void** vtable;
    UnkStringData* field_0x8;
    UnkStringData* field_0x10;
    u64 field_0x18;
    u64 field_0x20;
    u64 field_0x28;
    u64 field_0x30;
    u32 field_0x38;

    UnkCopySelf(const UnkCopySelf& other)
        : vtable(&g_unk_7101a21718[2]) {
        CopyId(field_0x8, other.field_0x8);
        vtable = &g_unk_7101a21af8[2];
        CopyId(field_0x10, other.field_0x10);
        __builtin_memcpy(&field_0x18, &other.field_0x18, 0x10);
        vtable = &g_unk_7101a21fc8[2];
        u64 a = other.field_0x28;
        u64 b = other.field_0x30;
        u32 tail = other.field_0x38;
        field_0x38 = tail;
        field_0x28 = a;
        field_0x30 = b;
    }

    // The second string id is stolen (its source ends up empty).
    UnkCopySelf(UnkCopySelf&& other) : vtable(&g_unk_7101a21718[2]) {
        CopyId(field_0x8, other.field_0x8);
        vtable = &g_unk_7101a21af8[2];
        field_0x10 = other.field_0x10;
        other.field_0x10 = &g_unk_7101d47108->empty;
        __builtin_memcpy(&field_0x18, &other.field_0x18, 0x10);
        vtable = &g_unk_7101a21fc8[2];
        u64 a = other.field_0x28;
        u64 b = other.field_0x30;
        u32 tail = other.field_0x38;
        field_0x38 = tail;
        field_0x28 = a;
        field_0x30 = b;
    }
};

void fn_71002e7654(UnkCopySelf* self, const UnkCopySelf* other) {
    new (self) UnkCopySelf(*other);
}

void fn_71002e76f0(UnkCopySelf* self, UnkCopySelf* other) {
    new (self) UnkCopySelf(static_cast<UnkCopySelf&&>(*other));
}
