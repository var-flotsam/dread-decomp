// Copy constructor of an array of 0x60-byte records with a second pointer array alongside (the
// array type moved by fn_7100e493f4's users): reserves the source's count out of line
// (fn_7100e54ad8), copy-constructs each record (an array copied by fn_71000cbae4 and a u16 array
// copied inline with its settings) and copies the pointer array. Also the PushBack of a plain
// array of the same records, and constructors (and copy constructors) of key/record pairs
// and an iterator's sync, and lookups that insert a default record.
#include <new>

#include "base/UnkCollectionIter.h"
#include "base/UnkFreedVectorPair.h"
#include "base/global/CRntVector.h"
#include "game/UnkPart48.h"
#include "game/UnkTU2563.h"
#include "game/UnkTU83.h"
#include "types.h"

inline UnkPart48::UnkPart48(const UnkPart48& other) { fn_71000cbae4(this, &other); }

// base::global::CRntVector<u16>'s layout.
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

    // base::global::CRntVector's defaults.
    __attribute__((always_inline)) UnkU16Array()
        : data(nullptr), count(0), capacity(0), step(10), field_0x14(true), flags(0xf),
          allocator(GetDefaultAllocator()) {}
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

struct UnkRecord60_7100e5493c {
    UnkPart48 field_0x0;
    UnkU16Array field_0x28;
    // Floats (a bounds' minimum and maximum, by the defaults), copied as words.
    u32 field_0x50 = 0x7f7fffff;  // FLT_MAX
    u32 field_0x54 = 0x7f7fffff;
    u32 field_0x58 = 0xff7fffff;  // -FLT_MAX
    u32 field_0x5c = 0xff7fffff;

    UnkRecord60_7100e5493c() = default;

    __attribute__((always_inline)) UnkRecord60_7100e5493c(const UnkRecord60_7100e5493c& other)
        : field_0x0(other.field_0x0), field_0x28(other.field_0x28), field_0x50(other.field_0x50),
          field_0x54(other.field_0x54), field_0x58(other.field_0x58), field_0x5c(other.field_0x5c) {}
    __attribute__((always_inline)) UnkRecord60_7100e5493c(UnkRecord60_7100e5493c&& other)
        : field_0x0(static_cast<UnkPart48&&>(other.field_0x0)),
          field_0x28(static_cast<UnkU16Array&&>(other.field_0x28)), field_0x50(other.field_0x50),
          field_0x54(other.field_0x54), field_0x58(other.field_0x58), field_0x5c(other.field_0x5c) {}
};
static_assert(sizeof(UnkRecord60_7100e5493c) == 0x60, "UnkRecord60_7100e5493c layout");

struct UnkRecord60Array {
    UnkRecord60_7100e5493c* data;
    void** field_0x8;
    u32 count;
    u32 capacity;
    u32 field_0x18;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) UnkRecord60Array(const UnkRecord60Array& other);
};

// Reserve: moves the records into new storage (destroying the old ones) and copies the pointer
// array.
void fn_7100e54ad8(UnkRecord60Array* self, u32 capacity) {
    if (self->capacity >= capacity)
        return;
    UnkRecord60_7100e5493c* data = static_cast<UnkRecord60_7100e5493c*>(
        self->allocator->vf_0x40((u64)capacity * sizeof(UnkRecord60_7100e5493c), 8, self->flags));
    void** ptrs = static_cast<void**>(self->allocator->vf_0x40((u64)capacity * sizeof(void*), 8, self->flags));
    if (UnkRecord60_7100e5493c* old = self->data) {
        UnkRecord60_7100e5493c* dst = data;
        for (u32 i = 0; i < self->count; ++i, ++old, ++dst) {
            new (dst) UnkRecord60_7100e5493c(static_cast<UnkRecord60_7100e5493c&&>(*old));
            old->~UnkRecord60_7100e5493c();
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
        new (&data[i]) UnkRecord60_7100e5493c(other.data[i]);
    __builtin_memcpy(field_0x8, other.field_0x8, (u64)other.count * sizeof(void*));
    count = other.count;
}

// NON_MATCHING(minor): the loop addresses each record from its u16 array's count (offset 0x30)
// in the original and from its last field (0x5c) in ours, so the zeroing of the u16 array's
// pointer and count is not paired.
void fn_7100e5493c(UnkRecord60Array* self, const UnkRecord60Array& other) {
    new (self) UnkRecord60Array(other);
}

// Appends a copy to a plain array of the records with an out-of-line Reserve (fn_7100e52b58),
// growing like PushBack (including its hang).
void fn_7100e52cf4(base::global::CRntVector<UnkRecord60_7100e5493c>* self, const UnkRecord60_7100e5493c* value) {
    u32 n = self->count;
    u32 new_capacity = self->capacity;
    if (new_capacity <= n && self->step != 0)
        new_capacity = self->GrownCapacity(n);
    if ((s32)n >= (s32)new_capacity)
        for (;;) {
        }
    // The same array of 0x60-byte records as UnkPairArray (include/base/UnkFreedVectorPair.h), typed by its
    // record here.
    fn_7100e52b58(reinterpret_cast<UnkPairArray*>(self), new_capacity);
    new (&self->data[self->count]) UnkRecord60_7100e5493c(*value);
    ++self->count;
}

// Constructors of key/record pairs from a key (a word or a 64-bit value) and a record.
template <class Key>
struct UnkRecord60Pair {
    Key key;
    UnkRecord60_7100e5493c value;

    __attribute__((always_inline)) UnkRecord60Pair(const Key& key, const UnkRecord60_7100e5493c& value)
        : key(key), value(value) {}
};

void fn_7100e52488(UnkRecord60Pair<u32>* self, const u32* key, const UnkRecord60_7100e5493c* value) {
    new (self) UnkRecord60Pair<u32>(*key, *value);
}
void fn_7100e54100(UnkRecord60Pair<u64>* self, const u64* key, const UnkRecord60_7100e5493c* value) {
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
__attribute__((always_inline)) static inline void Sync(base::UnkCollectionIter<UnkRecord60_7100e5493c, Key>* it) {
    using Pair = typename base::UnkIterValue<UnkRecord60_7100e5493c, Key>::Value;
    base::UnkIterValue<UnkRecord60_7100e5493c, Key>& cur = it->current;
    if (UnkRecord60_7100e5493c* element = it->element) {
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
void fn_7100e46b58(base::UnkCollectionIter<UnkRecord60_7100e5493c, u32>* it) { Sync(it); }
// NON_MATCHING(minor): as fn_7100e46b58.
void fn_7100e46f70(base::UnkCollectionIter<UnkRecord60_7100e5493c, u32>* it) { Sync(it); }

// Lookups that insert a default record when the key is missing (through fn_7100e550dc and
// fn_7100e56ed4, which take the record by reference).
UnkRecord60_7100e5493c* fn_7100e550dc(void* self, const u32* key, UnkRecord60_7100e5493c* value);
UnkRecord60_7100e5493c* fn_7100e56ed4(void* self, const u64* key, UnkRecord60_7100e5493c* value);

UnkRecord60_7100e5493c* fn_7100e44e70(void* self, const u32* key) {
    UnkRecord60_7100e5493c value = UnkRecord60_7100e5493c();
    return fn_7100e550dc(self, key, &value);
}
UnkRecord60_7100e5493c* fn_7100e44f58(void* self, const u64* key) {
    UnkRecord60_7100e5493c value = UnkRecord60_7100e5493c();
    return fn_7100e56ed4(self, key, &value);
}
