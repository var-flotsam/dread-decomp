#pragma once

// Unit 354 (navmesh::CFixedDynamicSmartLinkRule's unit): the array of the 0x50-byte values
// described in src/game/navmesh/UnkNavMeshRuleMove.cpp (four words, an array of children of the
// same type at +0x10, a 0x15-byte block at +0x38). UnkNavRuleCopy_A1_b75.cpp defines the array's copy constructor
// (fn_71002726e8) and copy assignment (fn_7100272348); UnkNavRuleSetAt_A1_b75.cpp the "set element at index" functions
// (slots 0x140 and 0x148 of the reflected collection type) in the shape of
// src/base/UnkUnit25/UnkStrIdPathPairSetAt_b62a1.cpp: an index past the end of a growable array
// first reserves (fn_7100272474) and default-constructs the new values, then the value is copied
// or moved. Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "types.h"

void fn_710026ead0(void* self);

namespace navmesh {
struct UnkNavRuleValue_A1_b75;
struct UnkNavRuleValues_A1_b75;
}  // namespace navmesh

// The array's reserve, copy constructor and copy assignment.
struct UnkSmartLinkEntryArray;  // the array as CFixedDynamicSmartLinkRule_PushBack.cpp names it
void fn_7100272474(UnkSmartLinkEntryArray* self, u32 capacity);
void fn_71002726e8(navmesh::UnkNavRuleValues_A1_b75* self, const navmesh::UnkNavRuleValues_A1_b75* other);
// Spelled as fn_7100272bbc.cpp declares it (one signature per symbol).
using UnkAssignedArray = navmesh::UnkNavRuleValues_A1_b75;
UnkAssignedArray* fn_7100272348(UnkAssignedArray* self, const UnkAssignedArray* other);

namespace navmesh {

// base::UnkVector's layout, with the copy operations out of line.
struct UnkNavRuleValues_A1_b75 {
    UnkNavRuleValue_A1_b75* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkNavRuleValues_A1_b75()
        : data(nullptr), count(0), capacity(0), step(10), field_0x14(true), flags(0xf),
          allocator(fn_7100083990()) {}
    UnkNavRuleValues_A1_b75(const UnkNavRuleValues_A1_b75& other) { fn_71002726e8(this, &other); }
    UnkNavRuleValues_A1_b75& operator=(const UnkNavRuleValues_A1_b75& other) {
        fn_7100272348(this, &other);
        return *this;
    }
    inline UnkNavRuleValues_A1_b75& operator=(UnkNavRuleValues_A1_b75&& other);
    inline void Clear();
    inline void PushBackCopy(const UnkNavRuleValue_A1_b75& value);
};

// A word with its own assignment, so the four at +0x0 are copied one by one.
struct UnkNavRuleWord_A1_b75 {
    u32 value = 0;
    UnkNavRuleWord_A1_b75() {}
    UnkNavRuleWord_A1_b75(const UnkNavRuleWord_A1_b75& other) : value(other.value) {}
    UnkNavRuleWord_A1_b75& operator=(const UnkNavRuleWord_A1_b75& other) {
        value = other.value;
        return *this;
    }
};

// The 0x15-byte block at +0x38, copied as one.
struct UnkNavRuleTail_A1_b75 {
    bool field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    bool field_0x14;
    UnkNavRuleTail_A1_b75()
        : field_0x0(false), field_0x4(0), field_0x8(6), field_0xc(2), field_0x10(0), field_0x14(true) {}
    UnkNavRuleTail_A1_b75(const UnkNavRuleTail_A1_b75& other) { __builtin_memcpy(this, &other, 0x15); }
    UnkNavRuleTail_A1_b75& operator=(const UnkNavRuleTail_A1_b75& other) {
        __builtin_memcpy(this, &other, 0x15);
        return *this;
    }
};

struct UnkNavRuleValue_A1_b75 {
    UnkNavRuleWord_A1_b75 field_0x0;
    UnkNavRuleWord_A1_b75 field_0x4;
    UnkNavRuleWord_A1_b75 field_0x8;
    UnkNavRuleWord_A1_b75 field_0xc;
    UnkNavRuleValues_A1_b75 children;  // 0x10
    UnkNavRuleTail_A1_b75 tail;        // 0x38

    UnkNavRuleValue_A1_b75() {}
    UnkNavRuleValue_A1_b75(const UnkNavRuleValue_A1_b75& other) = default;
    __attribute__((always_inline)) ~UnkNavRuleValue_A1_b75() { fn_710026ead0(this); }
    UnkNavRuleValue_A1_b75& operator=(const UnkNavRuleValue_A1_b75& other) {
        field_0x0 = other.field_0x0;
        field_0x4 = other.field_0x4;
        field_0x8 = other.field_0x8;
        field_0xc = other.field_0xc;
        fn_7100272348(&children, &other.children);
        tail = other.tail;
        return *this;
    }
    UnkNavRuleValue_A1_b75& operator=(UnkNavRuleValue_A1_b75&& other) {
        field_0x0 = other.field_0x0;
        field_0x4 = other.field_0x4;
        field_0x8 = other.field_0x8;
        field_0xc = other.field_0xc;
        children = static_cast<UnkNavRuleValues_A1_b75&&>(other.children);
        tail = other.tail;
        return *this;
    }
};
static_assert(sizeof(UnkNavRuleValue_A1_b75) == 0x50, "UnkNavRuleValue_A1_b75 layout");

__attribute__((always_inline)) inline void UnkNavRuleValues_A1_b75::Clear() {
    for (u32 i = 0; i < count; ++i)
        data[i].~UnkNavRuleValue_A1_b75();
    count = 0;
}

// As base::UnkVector's move assignment.
inline UnkNavRuleValues_A1_b75& UnkNavRuleValues_A1_b75::operator=(UnkNavRuleValues_A1_b75&& other) {
    if (this != &other) {
        Clear();
        allocator = other.allocator;
        flags = other.flags;
        step = other.step;
        field_0x14 = other.field_0x14;
        count = other.count;
        capacity = other.capacity;
        data = other.data;
        other.data = nullptr;
        other.count = 0;
        other.capacity = 0;
    }
    return *this;
}

__attribute__((always_inline)) inline void UnkNavRuleValues_A1_b75::PushBackCopy(const UnkNavRuleValue_A1_b75& value) {
    u32 n = count;
    u32 new_capacity = capacity;
    if (new_capacity <= n && step != 0)
        new_capacity = (n / step + 1) * step;
    if ((s32)n >= (s32)new_capacity)
        for (;;) {
        }
    fn_7100272474(reinterpret_cast<UnkSmartLinkEntryArray*>(this), new_capacity);
    new (&data[count]) UnkNavRuleValue_A1_b75(value);
    ++count;
}

__attribute__((always_inline)) static inline void Grow_A1_b75(UnkNavRuleValues_A1_b75* self, u32 index) {
    u32 capacity = self->capacity;
    if (capacity <= index && self->step != 0)
        capacity = (index / self->step + 1) * self->step;
    fn_7100272474(reinterpret_cast<UnkSmartLinkEntryArray*>(self), capacity);
    u32 n = index + 1;
    fn_7100272474(reinterpret_cast<UnkSmartLinkEntryArray*>(self), n);
    for (u32 i = self->count; i < n; ++i)
        new (&self->data[i]) UnkNavRuleValue_A1_b75();
    self->count = n;
}

}  // namespace navmesh

using navmesh::UnkNavRuleValue_A1_b75;
using navmesh::UnkNavRuleValues_A1_b75;
