// Copies of fn_7100895194 (src/game_s4/fn_71008947b8.cpp) for other types, with their own callees and data (they name their own callees, data and strings; the comments describe the original): fn_71011141e8.
// Copy-constructs a value-or-reference holder: kind 1 holds a value (an id and an entry) in
// place, kind 2 points at them; the copy always holds them in place (kind 1).
// fn_71008aa61c is an identical copy.
#include "base/global/CStrId.h"
#include "base/UnkErasedVector.h"
#include "base/UnkRefHandle.h"
#include "base/UnkVector.h"

using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

// A string id whose move-assignment releases the old string first (decrementing and testing the
// count in two steps), then takes the other's and leaves it the empty string of the pool it
// last loaded.
struct UnkId : CStrId {
    UnkId(const UnkId& other) = default;
    __attribute__((always_inline)) ~UnkId() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
    UnkId& operator=(UnkId&& other) {
        UnkStringPool* pool = g_unk_7101d47108;
        if (pool) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
        data = other.data;
        other.data = &pool->empty;
        return *this;
    }
};

void fn_7100ad3588(UnkErasedVector* self);  // clear

struct UnkCallbackBase {
    virtual void vf0();
    UnkOwnedErasedVector callbacks;
    u32 field_0x30;

    UnkCallbackBase() : field_0x30(0) {}
};

struct UnkCallbackSet : UnkCallbackBase {
    virtual void vf8();
    base::UnkRefHandle owner;

    __attribute__((always_inline)) UnkCallbackSet& operator=(UnkCallbackSet&& other) {
        fn_710031d208(&owner, &other.owner);
        field_0x30 = other.field_0x30;
        fn_710023b29c(&callbacks, &other.callbacks);
        other.field_0x30 = 0;
        fn_7100ad3588(&other.callbacks);
        if (base::UnkRefCounted* p = other.owner.ptr) {
            if (--p->ref_count == 0)
                delete p;
            other.owner.ptr = nullptr;
        }
        return *this;
    }

    __attribute__((always_inline)) ~UnkCallbackSet() {
        if (base::UnkRefCounted* p = owner.ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }

    UnkCallbackSet(UnkCallbackSet&& other);  // fn_71007b1928

    __attribute__((always_inline)) UnkCallbackSet& operator=(const UnkCallbackSet& other) {
        const base::UnkRefHandle* from = &other.owner;  // evaluated first, as `owner = other.owner` would be
        fn_710031d208(&owner, from);
        field_0x30 = other.field_0x30;
        fn_710023b29c(&callbacks, &other.callbacks);
        return *this;
    }

    UnkCallbackSet(const UnkCallbackSet& other) {
        owner.ptr = nullptr;
        owner.Set(other.owner.ptr);
        field_0x30 = other.field_0x30;
        fn_710023b29c(&callbacks, &other.callbacks);
    }
};

struct UnkEntry {
    UnkCallbackSet set;
    UnkId id;

    UnkEntry(const UnkEntry& other) = default;
    __attribute__((always_inline)) UnkEntry& operator=(const UnkEntry& other) {
        set = other.set;
        static_cast<CStrId&>(id) = other.id;
        return *this;
    }
    __attribute__((always_inline)) ~UnkEntry() {}
    __attribute__((always_inline)) UnkEntry& operator=(UnkEntry&& other) {
        set = static_cast<UnkCallbackSet&&>(other.set);
        id = static_cast<UnkId&&>(other.id);
        return *this;
    }
};

struct UnkPair {
    u32 value;
    UnkEntry entry;
};

struct UnkPairRef {
    u32* value;
    UnkEntry* entry;
};

// Untyped storage for either form.
struct UnkHolder {
    alignas(8) u8 storage[0x50];
    u32 kind;

    UnkPair& Pair() { return *reinterpret_cast<UnkPair*>(storage); }
    const UnkPair& Pair() const { return *reinterpret_cast<const UnkPair*>(storage); }
    const UnkPairRef& Ref() const { return *reinterpret_cast<const UnkPairRef*>(storage); }
    u32& Value() { return kind == 2 ? *Ref().value : Pair().value; }
    UnkEntry& Entry() { return kind == 2 ? *Ref().entry : Pair().entry; }
    const u32& Value() const { return kind == 2 ? *Ref().value : Pair().value; }
    const UnkEntry& Entry() const { return kind == 2 ? *Ref().entry : Pair().entry; }
};

static inline __attribute__((always_inline)) void CopyHolder(UnkHolder* self, const UnkHolder* other) {
    self->kind = 0;
    if (other->kind == 1) {
        self->kind = 1;
        self->Pair().value = other->Pair().value;
        new (&self->Pair().entry) UnkEntry(other->Pair().entry);
    } else if (other->kind == 2) {
        self->kind = 1;
        UnkEntry* entry = other->Ref().entry;
        self->Pair().value = *other->Ref().value;
        new (&self->Pair().entry) UnkEntry(*entry);
    }
}

// An entry as the array's Reserve moves it and its assignment destroys it: CStrId has no move
// constructor, and its destructor would release a second time, so the id is a plain pointer
// here, emptied when moved from.
struct UnkMovedId {
    UnkStringData* data;

    UnkMovedId(UnkMovedId&& other) : data(other.data) { other.data = &g_unk_7101d47108->empty; }
    __attribute__((always_inline)) ~UnkMovedId() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkMovedEntry {
    UnkCallbackSet set;
    UnkMovedId id;
};

struct UnkEntryVector;
// Reserve and PushBack of a copy (one copy each per array type)
void fn_7100895498(UnkEntryVector* v, u32 n);
void fn_71008ab4e0(UnkEntryVector* v, u32 n);
void fn_710089566c(UnkEntryVector* v, const UnkEntry* value);
void fn_71008ab6b4(UnkEntryVector* v, const UnkEntry* value);

struct UnkEntryVector {
    UnkEntry* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) void Reserve(u32 n) {
        if (capacity >= n)
            return;
        UnkEntry* p = static_cast<UnkEntry*>(allocator->vf_0x40((u64)n * sizeof(UnkEntry), 8, flags));
        if (data) {
            UnkMovedEntry* src = reinterpret_cast<UnkMovedEntry*>(data);
            UnkMovedEntry* dst = reinterpret_cast<UnkMovedEntry*>(p);
            for (u32 i = 0; i < count; ++i, ++dst, ++src) {
                new (dst) UnkMovedEntry(static_cast<UnkMovedEntry&&>(*src));
                src->~UnkMovedEntry();
            }
            allocator->vf_0x48(data);
        }
        data = p;
        capacity = n;
    }

    // Reserve is this array type's out-of-line copy of Reserve.
    template <void (*Reserve)(UnkEntryVector*, u32)>
    __attribute__((always_inline)) void Insert(UnkMovedEntry&& value, u32 index) {
        if (index == count) {
            u32 n = count;
            u32 new_capacity = capacity;
            if (new_capacity <= n && step != 0)
                new_capacity = (n / step + 1) * step;
            if ((s32)n >= (s32)new_capacity)  // hangs, as UnkVector::PushBack does
                for (;;) {
                }
            Reserve(this, new_capacity);
            new (&reinterpret_cast<UnkMovedEntry*>(data)[count]) UnkMovedEntry(static_cast<UnkMovedEntry&&>(value));
        } else {
            u32 n = capacity;
            if (n <= count && step != 0)
                n = (count / step + 1) * step;
            Reserve(this, n);
            for (u32 i = count - 1; i >= index; --i) {
                UnkMovedEntry* e = &reinterpret_cast<UnkMovedEntry*>(data)[i];
                new (&e[1]) UnkMovedEntry(static_cast<UnkMovedEntry&&>(*e));
                reinterpret_cast<UnkMovedEntry*>(data)[i].~UnkMovedEntry();
                if (i == 0)
                    break;
            }
            new (&reinterpret_cast<UnkMovedEntry*>(data)[index]) UnkMovedEntry(static_cast<UnkMovedEntry&&>(value));
        }
        ++count;
    }

    template <void (*Reserve)(UnkEntryVector*, u32), void (*PushBack)(UnkEntryVector*, const UnkEntry*)>
    __attribute__((always_inline)) UnkEntryVector* Assign(const UnkEntryVector& other) {
        if (this != &other) {
            for (u64 i = 0; i < count; ++i)
                reinterpret_cast<UnkMovedEntry*>(data)[i].~UnkMovedEntry();
            count = 0;
            Reserve(this, other.capacity);
            for (const UnkEntry *p = other.data, *end = p + other.count; p != end; ++p)
                PushBack(this, p);
        }
        return this;
    }

    template <void (*Reserve)(UnkEntryVector*, u32), void (*PushBack)(UnkEntryVector*, const UnkEntry*)>
    __attribute__((always_inline)) void CopyConstruct(const UnkEntryVector& other) {
        data = nullptr;
        count = 0;
        capacity = 0;
        step = other.step;
        field_0x14 = other.field_0x14;
        flags = other.flags;
        allocator = other.allocator;
        Reserve(this, other.capacity);
        for (const UnkEntry *p = other.data, *end = p + other.count; p != end; ++p)
            PushBack(this, p);
    }
};

void fn_71011143d8(UnkEntryVector* v, u32 n);
void fn_7101114588(UnkEntryVector* v, const UnkEntry* value);
void fn_71011141e8(UnkEntryVector* self, const UnkEntryVector* other) {
    self->CopyConstruct<fn_71011143d8, fn_7101114588>(*other);
}

struct UnkMovedPair {
    u32 value;
    UnkMovedEntry entry;
};

struct UnkMovedPairRef {
    u32* value;
    UnkMovedEntry* entry;
};

// Moves either form into an owned pair.
static inline __attribute__((always_inline)) void MoveHolder(UnkHolder* self, UnkHolder* other) {
    self->kind = 0;
    UnkMovedPair* pair = reinterpret_cast<UnkMovedPair*>(self->storage);
    if (other->kind == 1) {
        self->kind = 1;
        UnkMovedPair* from = reinterpret_cast<UnkMovedPair*>(other->storage);
        pair->value = from->value;
        new (&pair->entry) UnkMovedEntry(static_cast<UnkMovedEntry&&>(from->entry));
    } else if (other->kind == 2) {
        self->kind = 1;
        const UnkMovedPairRef* ref = reinterpret_cast<const UnkMovedPairRef*>(other->storage);
        UnkMovedEntry* entry = ref->entry;
        pair->value = *ref->value;
        new (&pair->entry) UnkMovedEntry(static_cast<UnkMovedEntry&&>(*entry));
    }
}

// Finds or adds the entry for a key (one copy per map type).
UnkEntry* fn_7100892528(void* self, u32 key);
UnkEntry* fn_71008a84e4(void* self, u32 key);

template <UnkEntry* (*Get)(void*, u32)>
static inline __attribute__((always_inline)) void SetEntry(void* self, const u32* key, UnkEntry* value) {
    *Get(self, *key) = static_cast<UnkEntry&&>(*value);
}

static inline __attribute__((always_inline)) UnkEntry* MoveAssign(UnkEntry* self, UnkEntry* other) {
    if (self != other)
        *self = static_cast<UnkEntry&&>(*other);
    return self;
}

static inline __attribute__((always_inline)) UnkHolder* AssignHolder(UnkHolder* self, const UnkHolder* other) {
    if (self != other) {
        self->Value() = other->Value();
        self->Entry() = other->Entry();
    }
    return self;
}

