// Copy-constructs a value-or-reference holder: kind 1 holds a value (an id and an entry) in
// place, kind 2 points at them; the copy always holds them in place (kind 1).
// fn_71008aa61c is an identical copy.
#include "base/global/CStrId.h"
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

// Function table of a type-erased value; only the slot used here is named.
struct UnkErasedOps {
    void* field_0x0[8];
    void (*destroy)(struct UnkErasedValue* self);
};

struct UnkErasedValue {
    void* storage;
    UnkErasedOps* ops;
};

struct UnkErasedVector {
    UnkErasedValue* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkErasedVector() : data(nullptr), count(0), capacity(0), step(10), field_0x14(true), flags(0xf) {
        allocator = fn_7100083990();
    }
    ~UnkErasedVector() {
        if (data) {
            for (u64 i = 0; i < count; ++i) {
                UnkErasedValue& v = data[i];
                if (v.ops) {
                    v.ops->destroy(&v);
                    v.ops = nullptr;
                }
            }
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

UnkErasedVector* fn_710023b29c(UnkErasedVector* self, const UnkErasedVector* other);
void fn_7100ad3588(UnkErasedVector* self);  // clear

struct UnkRefCounted {
    virtual void vf0();
    virtual void vf8();
    virtual void Delete();
    union {
        s32 ref_count;
    };
};

struct UnkCallbackBase {
    virtual void vf0();
    UnkErasedVector callbacks;
    u32 field_0x30;

    UnkCallbackBase() : field_0x30(0) {}
};

struct UnkCallbackSet;
void fn_710031d208(UnkRefCounted** self, UnkRefCounted* const* other);

struct UnkCallbackSet : UnkCallbackBase {
    virtual void vf8();
    UnkRefCounted* owner;

    __attribute__((always_inline)) UnkCallbackSet& operator=(UnkCallbackSet&& other) {
        fn_710031d208(&owner, &other.owner);
        field_0x30 = other.field_0x30;
        fn_710023b29c(&callbacks, &other.callbacks);
        other.field_0x30 = 0;
        fn_7100ad3588(&other.callbacks);
        if (UnkRefCounted* p = other.owner) {
            if (--p->ref_count == 0)
                p->Delete();
            other.owner = nullptr;
        }
        return *this;
    }

    __attribute__((always_inline)) ~UnkCallbackSet() {
        if (UnkRefCounted* p = owner) {
            if (--p->ref_count == 0)
                p->Delete();
        }
    }

    UnkCallbackSet(const UnkCallbackSet& other) : owner(nullptr) {
        UnkRefCounted* p = other.owner;
        if (p != owner) {
            if (p)
                ++p->ref_count;
            if (UnkRefCounted* old = owner) {
                if (--old->ref_count == 0)
                    old->Delete();
            }
            owner = p;
        }
        field_0x30 = other.field_0x30;
        fn_710023b29c(&callbacks, &other.callbacks);
    }
};

struct UnkEntry {
    UnkCallbackSet set;
    UnkId id;

    UnkEntry(const UnkEntry& other) = default;
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

void fn_71008947b8(UnkHolder* self, const UnkHolder* other) {
    CopyHolder(self, other);
}

void fn_71008aa61c(UnkHolder* self, const UnkHolder* other) {
    CopyHolder(self, other);
}



