// Begin and end of a reflected array of callback sets: an iterator at the first element or
// one past the last, its cached value re-pointed by the iterator's sync function.
#include "base/UnkVector.h"

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

struct UnkCallbackSet : UnkCallbackBase {
    virtual void vf8();
    UnkRefCounted* owner;

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

    __attribute__((always_inline)) ~UnkCallbackSet() {
        if (UnkRefCounted* p = owner) {
            if (--p->ref_count == 0)
                p->Delete();
        }
    }
};

struct UnkCallbackSetElem {
    UnkCallbackSet set;
    u32 field_0x40;
};

// The cached (index, element) value: empty (0), owned (1) or referring to the iterator's (2).
struct UnkIterValue {
    struct Value {
        u32 index;
        UnkCallbackSetElem element;
    };
    alignas(8) u8 storage[sizeof(Value)];
    u32 state;

    UnkIterValue() : state(0) {}
    ~UnkIterValue() {
        if (state == 2) {
        } else if (state == 1) {
            reinterpret_cast<Value*>(storage)->~Value();
        } else {
            return;
        }
        state = 0;
    }
};

// Sync is the iterator's sync function: re-points `current` at the iterator's index and
// element, or resets it (one per copy of the collection functions).
template <void (*Sync)(void*)>
struct UnkIter {
    u32 index;
    UnkCallbackSetElem* element;
    UnkIterValue current;

    UnkIter(u32 index, UnkCallbackSetElem* element);
    __attribute__((always_inline)) UnkIter& operator=(const UnkIter& other) {
        if (this != &other) {
            index = other.index;
            element = other.element;
            Sync(this);
        }
        return *this;
    }
};

template <void (*Sync)(void*)>
inline UnkIter<Sync>::UnkIter(u32 index, UnkCallbackSetElem* element) : index(index), element(element) {
    Sync(this);
}

void fn_710090b1c8(void* self);
void fn_7100a39fec(void* self);
void fn_7100a93bc4(void* self);

struct UnkCallbackSetVector {
    UnkCallbackSetElem* data;
    u32 count;
    u32 capacity;
    u32 step;
};

template <void (*Sync)(void*)>
static inline __attribute__((always_inline)) void Begin(const UnkCallbackSetVector* v, UnkIter<Sync>* out) {
    *out = UnkIter<Sync>(0, v->data);
}

// Three copies (one per array type), each with its own sync function. End's braces matter: they
// evaluate the count before the data, as the original does.
void fn_710090dbb4(const UnkCallbackSetVector* v, UnkIter<fn_710090b1c8>* out) {
    Begin(v, out);
}

void fn_710090dcf0(const UnkCallbackSetVector* v, UnkIter<fn_710090b1c8>* out) {
    *out = {v->count, v->data + v->count};
}

void fn_7100a3c488(const UnkCallbackSetVector* v, UnkIter<fn_7100a39fec>* out) {
    Begin(v, out);
}

void fn_7100a3c5c4(const UnkCallbackSetVector* v, UnkIter<fn_7100a39fec>* out) {
    *out = {v->count, v->data + v->count};
}

void fn_7100a96260(const UnkCallbackSetVector* v, UnkIter<fn_7100a93bc4>* out) {
    Begin(v, out);
}

void fn_7100a9639c(const UnkCallbackSetVector* v, UnkIter<fn_7100a93bc4>* out) {
    *out = {v->count, v->data + v->count};
}
