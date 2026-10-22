// Copy constructor of an array of 0x60-byte records with a second pointer array alongside (the
// array type moved by fn_7100e493f4's users): reserves the source's count out of line
// (fn_7100e54ad8), copy-constructs each record (an array copied by fn_71000cbae4 and a u16 array
// copied inline with its settings) and copies the pointer array.
#include <new>

#include "base/UnkVector.h"
#include "types.h"

// base::UnkVector's layout, copied out of line.
struct UnkPart48 {
    union {
        void* data;
    };
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkPart48(const UnkPart48& other);
    __attribute__((always_inline)) UnkPart48(UnkPart48&& other)
        : data(other.data), count(other.count), capacity(other.capacity), step(other.step),
          field_0x14(other.field_0x14), flags(other.flags), allocator(other.allocator) {
        other.data = nullptr;
        other.count = 0;
        other.capacity = 0;
    }
    __attribute__((always_inline)) ~UnkPart48() {
        if (data) {
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};
void fn_71000cbae4(UnkPart48* self, const UnkPart48* other);
inline UnkPart48::UnkPart48(const UnkPart48& other) { fn_71000cbae4(this, &other); }

// base::UnkVector<u16>'s layout.
struct UnkU16Array {
    union {
        u16* data;
    };
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    // Takes the settings, reserves the source's capacity and copies the elements.
    __attribute__((always_inline)) UnkU16Array(const UnkU16Array& other)
        : data(nullptr), count(0), capacity(0), step(other.step), field_0x14(other.field_0x14),
          flags(other.flags), allocator(other.allocator) {
        u32 n = other.capacity;
        if (capacity < n) {
            u16* p = static_cast<u16*>(allocator->vf_0x40((u64)n * sizeof(u16), 8, flags));
            if (data) {
                __builtin_memcpy(p, data, (u64)count * sizeof(u16));
                allocator->vf_0x48(data);
            }
            data = p;
            capacity = n;
        }
        __builtin_memcpy(data, other.data, (u64)other.count * sizeof(u16));
        count = other.count;
    }
    __attribute__((always_inline)) UnkU16Array(UnkU16Array&& other)
        : data(other.data), count(other.count), capacity(other.capacity), step(other.step),
          field_0x14(other.field_0x14), flags(other.flags), allocator(other.allocator) {
        other.data = nullptr;
        other.count = 0;
        other.capacity = 0;
    }
    __attribute__((always_inline)) ~UnkU16Array() {
        if (data) {
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

struct UnkRecord60 {
    UnkPart48 field_0x0;
    UnkU16Array field_0x28;
    u32 field_0x50;
    u32 field_0x54;
    u32 field_0x58;
    u32 field_0x5c;

    __attribute__((always_inline)) UnkRecord60(const UnkRecord60& other)
        : field_0x0(other.field_0x0), field_0x28(other.field_0x28), field_0x50(other.field_0x50),
          field_0x54(other.field_0x54), field_0x58(other.field_0x58), field_0x5c(other.field_0x5c) {}
    __attribute__((always_inline)) UnkRecord60(UnkRecord60&& other)
        : field_0x0(static_cast<UnkPart48&&>(other.field_0x0)),
          field_0x28(static_cast<UnkU16Array&&>(other.field_0x28)), field_0x50(other.field_0x50),
          field_0x54(other.field_0x54), field_0x58(other.field_0x58), field_0x5c(other.field_0x5c) {}
};
static_assert(sizeof(UnkRecord60) == 0x60, "UnkRecord60 layout");

struct UnkRecord60Array {
    UnkRecord60* data;
    void** field_0x8;
    u32 count;
    u32 capacity;
    u32 field_0x18;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) UnkRecord60Array(const UnkRecord60Array& other);
};

__attribute__((noinline)) void fn_7100e54ad8(UnkRecord60Array* self, u32 capacity);

// Reserve: moves the records into new storage (destroying the old ones) and copies the pointer
// array.
void fn_7100e54ad8(UnkRecord60Array* self, u32 capacity) {
    if (self->capacity >= capacity)
        return;
    UnkRecord60* data = static_cast<UnkRecord60*>(
        self->allocator->vf_0x40((u64)capacity * sizeof(UnkRecord60), 8, self->flags));
    void** ptrs = static_cast<void**>(self->allocator->vf_0x40((u64)capacity * sizeof(void*), 8, self->flags));
    if (UnkRecord60* old = self->data) {
        UnkRecord60* dst = data;
        for (u32 i = 0; i < self->count; ++i, ++old, ++dst) {
            new (dst) UnkRecord60(static_cast<UnkRecord60&&>(*old));
            old->~UnkRecord60();
        }
        self->allocator->vf_0x48(self->data);
    }
    if (self->field_0x8) {
        __builtin_memcpy(ptrs, self->field_0x8, (u64)self->count * sizeof(void*));
        self->allocator->vf_0x48(self->field_0x8);
    }
    self->data = data;
    self->field_0x8 = ptrs;
    self->capacity = capacity;
}

inline UnkRecord60Array::UnkRecord60Array(const UnkRecord60Array& other)
    : data(nullptr), field_0x8(nullptr), count(0), capacity(0), field_0x18(other.field_0x18),
      flags(other.flags), allocator(other.allocator) {
    fn_7100e54ad8(this, other.count);
    count = other.count;
    for (u64 i = 0; i < count; ++i)
        new (&data[i]) UnkRecord60(other.data[i]);
    __builtin_memcpy(field_0x8, other.field_0x8, (u64)other.count * sizeof(void*));
    count = other.count;
}

// NON_MATCHING(minor): the loop addresses each record from its u16 array's count (offset 0x30)
// in the original and from its last field (0x5c) in ours, so the zeroing of the u16 array's
// pointer and count is not paired.
void fn_7100e5493c(UnkRecord60Array* self, const UnkRecord60Array& other) {
    new (self) UnkRecord60Array(other);
}
