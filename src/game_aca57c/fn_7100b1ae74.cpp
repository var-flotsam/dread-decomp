// Records of a word and a string (base::global::CRntString's layout, copied by fn_710007ea60 and
// moved by fn_710007eab4): moves of iterator values over them (one record type copies its string,
// the other moves it), and base::UnkVector operations on an array of them (copy, assignment,
// PushBack, Insert, GrowAt and a shifting Erase).
#include <new>

#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "types.h"

using base::global::CRntString;

struct UnkString30;
void fn_710007ea60(UnkString30* self, const UnkString30* other);
void fn_710007e6c4(UnkString30* self);

// base::global::CRntString's layout, with a move constructor.
struct UnkString30 {
    // The string, constructed and destroyed by hand (a union member runs neither by itself).
    union {
        CRntString str;
    };

    __attribute__((always_inline)) UnkString30() { fn_710007e6c4(this); }
    __attribute__((always_inline)) UnkString30(const UnkString30& other) { fn_710007ea60(this, &other); }
    __attribute__((always_inline)) UnkString30(UnkString30&& other) { fn_710007eab4(&str, &other.str); }
    __attribute__((always_inline)) ~UnkString30() { fn_710007ecd4(&str); }
};

// Copied as a whole, also when moved.
struct UnkWordString {
    u32 field_0x0;
    UnkString30 name;

    __attribute__((always_inline)) UnkWordString(const UnkWordString& other)
        : field_0x0(other.field_0x0), name(other.name) {}
};

struct UnkWordMovedString {
    u32 field_0x0;
    UnkString30 name;

    __attribute__((always_inline)) UnkWordMovedString() : field_0x0(0) {}
    UnkWordMovedString(const UnkWordMovedString&) = default;
    UnkWordMovedString(UnkWordMovedString&&) = default;
};

// base::UnkIterValue's layout and move constructor, written out.
template <class T>
struct UnkWordIterValue {
    struct Pair {
        u32 index;
        T element;
    };
    union {
        Pair value;
        struct {
            const u32* index;
            T* element;
        } ref;
    };
    s32 state;

    __attribute__((always_inline)) UnkWordIterValue(UnkWordIterValue&& other) {
        state = 0;
        if (other.state == 1) {
            state = 1;
            new (&value) Pair{other.value.index, static_cast<T&&>(other.value.element)};
        } else if (other.state == 2) {
            state = 1;
            auto r = other.ref;
            new (&value) Pair{*r.index, static_cast<T&&>(*r.element)};
        }
    }
};

void fn_7100b1ae74(UnkWordIterValue<UnkWordString>* self, UnkWordIterValue<UnkWordString>* other) {
    new (self) UnkWordIterValue<UnkWordString>(static_cast<UnkWordIterValue<UnkWordString>&&>(*other));
}
void fn_7100b1abb4(UnkWordIterValue<UnkWordMovedString>* self, UnkWordIterValue<UnkWordMovedString>* other) {
    new (self) UnkWordIterValue<UnkWordMovedString>(static_cast<UnkWordIterValue<UnkWordMovedString>&&>(*other));
}

// Erase at an iterator's index: the record is destroyed and each later one is moved down into
// the gap (its word copied, its string moved, then destroyed).
struct UnkWordStringArray {
    UnkWordMovedString* data;
    u32 count;
};

// NON_MATCHING(minor): the original turns the index into a byte offset and divides it back
// (multiplying by the inverse of 0x38), and the loop's compares and registers differ.
void fn_7100b1b030(UnkWordStringArray* self, const u32* index) {
    u32 i = *index;
    if (i < self->count) {
        self->data[i].~UnkWordMovedString();
        for (u64 j = i; j < self->count - 1; ++j) {
            self->data[j].field_0x0 = self->data[j + 1].field_0x0;
            new (&self->data[j].name) UnkString30(static_cast<UnkString30&&>(self->data[j + 1].name));
            self->data[j + 1].~UnkWordMovedString();
        }
        --self->count;
    }
}

// base::UnkVector of the records: PushBack of a copy, and the copy constructor (which reserves
// the source's capacity and pushes each record).
using UnkWordStringVector = base::UnkVector<UnkWordMovedString>;

void fn_7100b1a860(UnkWordStringVector* self, const UnkWordMovedString* value) { self->PushBackCopy(*value); }

void fn_7100b1a74c(UnkWordStringVector* self, const UnkWordStringVector* other) {
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->field_0x14 = other->field_0x14;
    self->flags = other->flags;
    self->allocator = other->allocator;
    self->Reserve(other->capacity);
    for (const UnkWordMovedString *p = other->data, *end = p + other->count; p != end; ++p)
        fn_7100b1a860(self, p);
}

// Copy assignment: the records are destroyed, the source's capacity reserved and each record
// pushed.
UnkWordStringVector* fn_7100b1a98c(UnkWordStringVector* self, const UnkWordStringVector* other) {
    if (self != other) {
        for (u64 i = 0; i < self->count; ++i)
            self->data[i].~UnkWordMovedString();
        self->count = 0;
        self->Reserve(other->capacity);
        for (const UnkWordMovedString *p = other->data, *end = p + other->count; p != end; ++p)
            fn_7100b1a860(self, p);
    }
    return self;
}

// base::UnkVector's MoveTo and Reallocate: the records are moved into new storage and the old ones
// destroyed.
__attribute__((always_inline)) static inline UnkWordMovedString* MoveTo(UnkWordStringVector* self, u32 capacity) {
    UnkWordMovedString* p = static_cast<UnkWordMovedString*>(
        self->allocator->vf_0x40((u64)capacity * sizeof(UnkWordMovedString), 8, self->flags));
    if (self->data) {
        UnkWordMovedString* src = self->data;
        UnkWordMovedString* dst = p;
        for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
            new (dst) UnkWordMovedString(static_cast<UnkWordMovedString&&>(*src));
            src->~UnkWordMovedString();
        }
        self->allocator->vf_0x48(self->data);
    }
    return p;
}

__attribute__((always_inline)) static inline void Reallocate(UnkWordStringVector* self, u32 capacity) {
    UnkWordMovedString* p = MoveTo(self, capacity);
    self->data = p;
    self->capacity = capacity;
}

// Insert of a copy at `index`: appending is PushBack; otherwise the array grows by its step and
// the later records are moved up one by one (as base::UnkVector::Insert).
void fn_7100b1b39c(UnkWordStringVector* self, const UnkWordMovedString* value, u32 index) {
    u32 n = self->count;
    if (n == index) {
        fn_7100b1a860(self, value);
        return;
    }
    u32 cap = self->capacity;
    if (cap <= n && self->step != 0) {
        u32 grown = (n / self->step + 1) * self->step;
        if (cap < grown) {
            Reallocate(self, grown);
            n = self->count;
        }
    }
    for (u32 i = n - 1; i >= index; --i) {
        UnkWordMovedString* q = &self->data[i];
        new (q + 1) UnkWordMovedString(static_cast<UnkWordMovedString&&>(*q));
        self->data[i].~UnkWordMovedString();
        if (i == 0)
            break;
    }
    new (&self->data[index]) UnkWordMovedString(*value);
    ++self->count;
}

// The record at `index`, growing the array (with default records) when it is past the end.
// (base::UnkVector::GrowAt.)
// NON_MATCHING(minor): the default-construction loop's counter and offset swap registers.
UnkWordMovedString* fn_7100b1a568(UnkWordStringVector* self, u32 index) {
    if (self->field_0x14 && self->count <= index) {
        u32 cap = self->capacity;
        if (cap <= index && self->step != 0) {
            u32 grown = (index / self->step + 1) * self->step;
            if (cap < grown) {
                Reallocate(self, grown);
                cap = grown;
            }
        }
        u32 n = index + 1;
        if (cap < n)
            Reallocate(self, n);
        for (u32 i = self->count; i < n; ++i)
            new (&self->data[i]) UnkWordMovedString();
        self->count = n;
    }
    return &self->data[index];
}

// Insert of a moved record at `index` (base::UnkVector::Insert, written out).
// NON_MATCHING(minor): the original computes the slot in each branch and jumps to the shared
// move; ours joins first. The branches are laid out in a different order.
void fn_7100b1b544(UnkWordStringVector* self, UnkWordMovedString* value, u32 index) {
    u32 cap = self->capacity;
    u32 n = self->count;
    UnkWordMovedString* p;
    if (n == index) {
        u32 new_capacity = cap;
        if (cap <= n && self->step != 0)
            new_capacity = (index / self->step + 1) * self->step;
        if ((s32)new_capacity <= (s32)index)
            for (;;) {
            }
        if (cap < new_capacity) {
            p = MoveTo(self, new_capacity);
            index = self->count;
            self->data = p;
            self->capacity = new_capacity;
        } else {
            p = self->data;
        }
    } else {
        if (cap <= n && self->step != 0) {
            u32 grown = (n / self->step + 1) * self->step;
            if (cap < grown) {
                Reallocate(self, grown);
                n = self->count;
            }
        }
        for (u32 i = n - 1; i >= index; --i) {
            UnkWordMovedString* q = &self->data[i];
            new (q + 1) UnkWordMovedString(static_cast<UnkWordMovedString&&>(*q));
            self->data[i].~UnkWordMovedString();
            if (i == 0)
                break;
        }
        p = self->data;
    }
    new (&p[index]) UnkWordMovedString(static_cast<UnkWordMovedString&&>(*value));
    ++self->count;
}
