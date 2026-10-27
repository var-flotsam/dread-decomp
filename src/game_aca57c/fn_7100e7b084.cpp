// Move constructors of iterator values over keyed strings with their own 0x200-byte storage: an
// owned pair is copied, and a referenced one is copied from its key and string (the string has
// no move, so both copy it). Also a string built from a formatted temporary.
#include <new>

#include "UnkBufferString.h"
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "types.h"

extern void* g_unk_7101a1a5a8[];
using UnkString200 = UnkBufferString<0x200, g_unk_7101a1a5a8>;
static_assert(sizeof(UnkString200) == 0x248, "UnkString200 layout");

struct UnkKeyedString200 {
    u32 key;
    UnkString200 value;
};

// base::UnkIterValue's layout and move constructor, written out.
struct UnkString200IterValue {
    union {
        UnkKeyedString200 value;
        struct {
            const u32* index;
            UnkString200* element;
        } ref;
    };
    s32 state;

    __attribute__((always_inline)) UnkString200IterValue(UnkString200IterValue&& other) {
        state = 0;
        if (other.state == 1) {
            state = 1;
            new (&value) UnkKeyedString200(other.value);
        } else if (other.state == 2) {
            state = 1;
            auto r = other.ref;
            new (&value) UnkKeyedString200{*r.index, *r.element};
        }
    }
};

void fn_7100e7b084(UnkString200IterValue* self, UnkString200IterValue* other) {
    new (self) UnkString200IterValue(static_cast<UnkString200IterValue&&>(*other));
}
void fn_7100e7b410(UnkString200IterValue* self, UnkString200IterValue* other) {
    new (self) UnkString200IterValue(static_cast<UnkString200IterValue&&>(*other));
}

// A text buffer of 0x200 characters, written by fn_71000a1784 from an object and its word at 8,
// and returned as a string with 0x100 bytes of storage.
extern void* g_unk_7101a1b958[];
using UnkString100 = UnkBufferString<0x100, g_unk_7101a194b8>;

struct UnkTextBuffer200 {
    void** vtable;
    u32 length;
    char data[0x200];

    __attribute__((always_inline)) UnkTextBuffer200() {
        length = 0;
        vtable = g_unk_7101a1b958 + 2;
        __builtin_memset(data, 0, sizeof(data));
    }
};

struct UnkFormatSource {
    u64 field_0x0;
    u64 field_0x8;
};
struct UnkFormatArgs {
    u64 field_0x0;
    UnkFormatSource* source;
};
void fn_71000a1784(UnkFormatArgs* args, UnkTextBuffer200* out);

UnkString100 fn_7100adc570(UnkFormatSource* self) {
    UnkTextBuffer200 text;
    UnkFormatArgs args = {self->field_0x8, self};
    fn_71000a1784(&args, &text);
    return UnkString100(text.data);
}

// A record of three floats (all 1 by default), a string with 0x20 bytes of storage and a word:
// its value-initialising constructor and its copy constructor.
extern void* g_unk_7101a1b5a0[];
using UnkString20 = UnkBufferString<0x20, g_unk_7101a1b5a0>;

struct UnkFloat3 {
    f32 x;
    f32 y;
    f32 z;
};

struct UnkScaledString {
    UnkFloat3 field_0x0 = {1.0f, 1.0f, 1.0f};
    UnkString20 field_0x10;
    u32 field_0x78 = 0;
};
static_assert(sizeof(UnkScaledString) == 0x80, "UnkScaledString layout");

void fn_7100ebed24(UnkScaledString* self) { new (self) UnkScaledString(); }
void fn_7100ebed9c(UnkScaledString* self, const UnkScaledString* other) { new (self) UnkScaledString(*other); }
void fn_7100ebee14(UnkScaledString* self, const UnkScaledString* other) { new (self) UnkScaledString(*other); }

// A record of three string ids, a word, a string with 0x40 bytes of storage and plain values:
// its copy constructor, and its move constructor (the ids are taken over, leaving the pool's
// empty string; the string is copied).
using base::global::CStrId;

struct UnkMovedStrId {
    base::global::UnkStringData* data;

    __attribute__((always_inline)) UnkMovedStrId(UnkMovedStrId&& other) : data(other.data) {
        other.data = &g_unk_7101d47108->empty;
    }
};

extern void* g_unk_7101a1b678[];
using UnkString40 = UnkBufferString<0x40, g_unk_7101a1b678>;

// Copied as a block.
struct UnkWord {
    u32 value;

    __attribute__((always_inline)) UnkWord(const UnkWord& other) { __builtin_memcpy(this, &other, 4); }
};

// Copied as a 0x14-byte block.
struct UnkWords5 {
    u64 field_0x0;
    u64 field_0x8;
    u32 field_0x10;

    __attribute__((always_inline)) UnkWords5(const UnkWords5& other) { __builtin_memcpy(this, &other, 0x14); }
};

template <class Id>
struct UnkIdsString40 {
    Id field_0x0;
    u64 field_0x8;
    Id field_0x10;
    Id field_0x18;
    UnkString40 field_0x20;
    UnkWord field_0xa8;
    UnkWord field_0xac;
    UnkWords5 field_0xb0;
};
static_assert(sizeof(UnkIdsString40<CStrId>) == 0xc8, "UnkIdsString40 layout");

void fn_7100e6dda8(UnkIdsString40<CStrId>* self, const UnkIdsString40<CStrId>* other) {
    new (self) UnkIdsString40<CStrId>(*other);
}
void fn_7100e6de94(UnkIdsString40<UnkMovedStrId>* self, UnkIdsString40<UnkMovedStrId>* other) {
    new (self) UnkIdsString40<UnkMovedStrId>(static_cast<UnkIdsString40<UnkMovedStrId>&&>(*other));
}

// An array of the 0x200-storage strings with a word array alongside (keys): its copy constructor
// (the capacity is reserved out of line by fn_7100e7bc3c) and its swap-removing Erase.
struct UnkString200Array {
    UnkString200* data;
    u32* keys;
    u32 count;
    u32 capacity;
    u32 field_0x18;
    u32 flags;
    void* allocator;

    __attribute__((always_inline)) UnkString200Array(const UnkString200Array& other);
};

void fn_7100e7bc3c(UnkString200Array* self, u32 capacity);

inline UnkString200Array::UnkString200Array(const UnkString200Array& other)
    : data(nullptr), keys(nullptr), count(0), capacity(0), field_0x18(other.field_0x18), flags(other.flags),
      allocator(other.allocator) {
    fn_7100e7bc3c(this, other.count);
    count = other.count;
    for (u64 i = 0; i < count; ++i)
        new (&data[i]) UnkString200(other.data[i]);
    __builtin_memcpy(keys, other.keys, (u64)other.count * sizeof(u32));
    count = other.count;
}

void fn_7100e7bab4(UnkString200Array* self, const UnkString200Array* other) { new (self) UnkString200Array(*other); }

UnkString200* fn_7100e7c0f0(UnkString200Array* self, UnkString200* pos) {
    u32 index = pos - self->data;
    self->data[index].~UnkString200();
    u32 last = self->count - 1;
    if (last != index) {
        new (&self->data[index]) UnkString200(self->data[last]);
        self->data[self->count - 1].~UnkString200();
        self->keys[index] = self->keys[self->count - 1];
    }
    --self->count;
    return &self->data[index];
}

// Reserve: copies the strings into new storage (destroying the old ones) and copies the keys.
void fn_7100e7bc3c(UnkString200Array* self, u32 capacity) {
    if (self->capacity >= capacity)
        return;
    base::UnkAllocator* allocator = static_cast<base::UnkAllocator*>(self->allocator);
    UnkString200* data =
        static_cast<UnkString200*>(allocator->vf_0x40((u64)capacity * sizeof(UnkString200), 8, self->flags));
    u32* keys = static_cast<u32*>(
        static_cast<base::UnkAllocator*>(self->allocator)->vf_0x40((u64)capacity * sizeof(u32), 8, self->flags));
    if (UnkString200* old = self->data) {
        UnkString200* dst = data;
        for (u32 i = 0; i < self->count; ++i, ++old, ++dst) {
            new (dst) UnkString200(*old);
            old->~UnkString200();
        }
        static_cast<base::UnkAllocator*>(self->allocator)->vf_0x48(self->data);
    }
    if (self->keys) {
        __builtin_memcpy(keys, self->keys, (u64)self->count * sizeof(u32));
        static_cast<base::UnkAllocator*>(self->allocator)->vf_0x48(self->keys);
    }
    self->data = data;
    self->keys = keys;
    self->capacity = capacity;
}

// Assignment: the strings are destroyed, the storage is freed (fn_7100e7bed0) and reserved again
// unless the capacity already equals the source's count, then the source is copied.
void fn_7100e7bed0(UnkString200Array* self);

UnkString200Array* fn_7100e7bd84(UnkString200Array* self, const UnkString200Array* other) {
    if (self == other)
        return self;
    if (self->data && self->count) {
        for (u64 i = 0; i < self->count; ++i)
            self->data[i].~UnkString200();
    }
    self->count = 0;
    if (other->count != self->capacity) {
        fn_7100e7bed0(self);
        fn_7100e7bc3c(self, other->count);
    }
    self->count = other->count;
    for (u64 i = 0; i < self->count; ++i)
        new (&self->data[i]) UnkString200(other->data[i]);
    __builtin_memcpy(self->keys, other->keys, (u64)other->count * sizeof(u32));
    self->count = other->count;
    return self;
}

// A table of the records keyed by string ids: inserts a copied key with a copied or moved record,
// growing by its step (fn_7100e6f940) when full, and returns the new record.
template <class Value>
struct UnkIdTable {
    Value* values;
    CStrId* keys;
    u32 count;
    u32 capacity;
    u32 step;
};

void fn_7100e6f940(void* self, u32 capacity);

template <class Value, class Ref>
__attribute__((always_inline)) static inline Value* Insert(UnkIdTable<Value>* self, const CStrId* key, Ref value) {
    if (self->count == self->capacity)
        fn_7100e6f940(self, self->step + self->count);
    new (self->keys + self->count) CStrId(*key);
    Value* slot = &self->values[self->count];
    new (slot) Value(static_cast<Ref>(value));
    u32 n = ++self->count;
    return n == 0 ? self->values : self->values + n - 1;
}

// NON_MATCHING(minor): the count is reloaded after the key's reference is taken at the join in
// the original, and in both branches in ours (which then widens it again); registers differ.
UnkIdsString40<CStrId>* fn_7100e7016c(UnkIdTable<UnkIdsString40<CStrId>>* self, const CStrId* key,
                                      const UnkIdsString40<CStrId>* value) {
    return Insert<UnkIdsString40<CStrId>, const UnkIdsString40<CStrId>&>(self, key, *value);
}
// NON_MATCHING(minor): as fn_7100e7016c.
UnkIdsString40<UnkMovedStrId>* fn_7100e702f0(UnkIdTable<UnkIdsString40<UnkMovedStrId>>* self, const CStrId* key,
                                             UnkIdsString40<UnkMovedStrId>* value) {
    return Insert<UnkIdsString40<UnkMovedStrId>, UnkIdsString40<UnkMovedStrId>&&>(
        self, key, static_cast<UnkIdsString40<UnkMovedStrId>&&>(*value));
}
