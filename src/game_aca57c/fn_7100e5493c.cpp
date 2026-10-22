// Copy constructor of an array of 0x60-byte records with a second pointer array alongside (the
// array type moved by fn_7100e493f4's users): reserves the source's count out of line
// (fn_7100e54ad8), copy-constructs each record (an array copied by fn_71000cbae4 and a u16 array
// copied inline with its settings) and copies the pointer array. Also the PushBack of a plain
// array of the same records, and constructors (and copy constructors) of key/record pairs
// and an iterator's sync, and lookups that insert a default record.
#include <new>

#include "base/UnkCollectionIter.h"
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

    // base::UnkVector's defaults.
    __attribute__((always_inline)) UnkPart48()
        : data(nullptr), count(0), capacity(0), step(10), field_0x14(true), flags(0xf),
          allocator(fn_7100083990()) {}
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

    // base::UnkVector's defaults.
    __attribute__((always_inline)) UnkU16Array()
        : data(nullptr), count(0), capacity(0), step(10), field_0x14(true), flags(0xf),
          allocator(fn_7100083990()) {}
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
    // Floats (a bounds' minimum and maximum, by the defaults), copied as words.
    u32 field_0x50 = 0x7f7fffff;  // FLT_MAX
    u32 field_0x54 = 0x7f7fffff;
    u32 field_0x58 = 0xff7fffff;  // -FLT_MAX
    u32 field_0x5c = 0xff7fffff;

    UnkRecord60() = default;

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

// A plain array of the records (base::UnkVector's layout) with an out-of-line Reserve
// (fn_7100e52b58): appends a copy, growing like PushBack (including its hang).
struct UnkRecord60List {
    UnkRecord60* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};
void fn_7100e52b58(UnkRecord60List* self, u32 capacity);

void fn_7100e52cf4(UnkRecord60List* self, const UnkRecord60* value) {
    u32 n = self->count;
    u32 new_capacity = self->capacity;
    if (new_capacity <= n && self->step != 0)
        new_capacity = (n / self->step + 1) * self->step;
    if ((s32)n >= (s32)new_capacity)
        for (;;) {
        }
    fn_7100e52b58(self, new_capacity);
    new (&self->data[self->count]) UnkRecord60(*value);
    ++self->count;
}

// Constructors of key/record pairs from a key (a word or a 64-bit value) and a record.
template <class Key>
struct UnkRecord60Pair {
    Key key;
    UnkRecord60 value;

    __attribute__((always_inline)) UnkRecord60Pair(const Key& key, const UnkRecord60& value)
        : key(key), value(value) {}
};

void fn_7100e52488(UnkRecord60Pair<u32>* self, const u32* key, const UnkRecord60* value) {
    new (self) UnkRecord60Pair<u32>(*key, *value);
}
void fn_7100e54100(UnkRecord60Pair<u64>* self, const u64* key, const UnkRecord60* value) {
    new (self) UnkRecord60Pair<u64>(*key, *value);
}

void fn_7100e52388(UnkRecord60Pair<u32>* self, const UnkRecord60Pair<u32>* other) {
    new (self) UnkRecord60Pair<u32>(*other);
}
void fn_7100e54000(UnkRecord60Pair<u64>* self, const UnkRecord60Pair<u64>* other) {
    new (self) UnkRecord60Pair<u64>(*other);
}

// An iterator over keyed records re-pointing its current value at its key and record, or
// resetting it when there is none (base::refl_IterSync with the value's reset inlined).
template <class Key>
__attribute__((always_inline)) static inline void Sync(base::UnkCollectionIter<UnkRecord60, Key>* it) {
    using Pair = typename base::UnkIterValue<UnkRecord60, Key>::Value;
    base::UnkIterValue<UnkRecord60, Key>& cur = it->current;
    if (UnkRecord60* element = it->element) {
        if (cur.state == 2) {
            cur.state = 0;
        } else if (cur.state == 1) {
            cur.value.~Pair();
            cur.state = 0;
        }
        cur.state = 2;
        cur.ref.index = &it->index;
        cur.ref.element = element;
    } else {
        if (cur.state == 2) {
            cur.state = 0;
        } else if (cur.state == 1) {
            cur.value.~Pair();
            cur.state = 0;
        }
    }
}

// NON_MATCHING(minor): the state load is hoisted above the element test (the original loads it
// in each branch).
void fn_7100e46b58(base::UnkCollectionIter<UnkRecord60, u32>* it) { Sync(it); }
// NON_MATCHING(minor): as fn_7100e46b58.
void fn_7100e46f70(base::UnkCollectionIter<UnkRecord60, u32>* it) { Sync(it); }

// Lookups that insert a default record when the key is missing (through fn_7100e550dc and
// fn_7100e56ed4, which take the record by reference).
UnkRecord60* fn_7100e550dc(void* self, const u32* key, UnkRecord60* value);
UnkRecord60* fn_7100e56ed4(void* self, const u64* key, UnkRecord60* value);

UnkRecord60* fn_7100e44e70(void* self, const u32* key) {
    UnkRecord60 value = UnkRecord60();
    return fn_7100e550dc(self, key, &value);
}
UnkRecord60* fn_7100e44f58(void* self, const u64* key) {
    UnkRecord60 value = UnkRecord60();
    return fn_7100e56ed4(self, key, &value);
}
