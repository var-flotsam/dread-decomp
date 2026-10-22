// Copies of fn_7100a3c218 (src/game_s4/fn_710090dbb4.cpp) for other types, with their own callees and data (the names, strings and comments below are the original's): fn_7101114d10.
// A reflected array of callback sets, three copies (one per array type): Begin and End (an iterator
// at the first element or one past the last, its cached value re-pointed by the iterator's sync
// function), Reserve, Truncate, Erase and Insert of a moved element.
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

    UnkCallbackSet(UnkCallbackSet&& other);  // fn_71007b1928

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

    struct Ref {
        u32* index;
        UnkCallbackSetElem* element;
    };

    UnkIterValue() : state(0) {}
    // An owned value either way: a reference's element is moved into a value of its own.
    __attribute__((always_inline)) void MoveFrom(UnkIterValue& other) {
        state = 0;
        Value* value = reinterpret_cast<Value*>(storage);
        if (other.state == 1) {
            state = 1;
            Value* from = reinterpret_cast<Value*>(other.storage);
            value->index = from->index;
            new (&value->element) UnkCallbackSetElem(static_cast<UnkCallbackSetElem&&>(from->element));
        } else if (other.state == 2) {
            state = 1;
            const Ref* ref = reinterpret_cast<const Ref*>(other.storage);
            UnkCallbackSetElem* element = ref->element;
            value->index = *ref->index;
            new (&value->element) UnkCallbackSetElem(static_cast<UnkCallbackSetElem&&>(*element));
        }
    }
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

// Reserve (one copy per array type).
struct UnkCallbackSetVector;
void fn_710090d00c(UnkCallbackSetVector* v, u32 n);
void fn_7100a3b8e0(UnkCallbackSetVector* v, u32 n);
void fn_7100a956b8(UnkCallbackSetVector* v, u32 n);
// PushBack of a copy (one per array type)
void fn_710090d1bc(UnkCallbackSetVector* v, const UnkCallbackSetElem* value);
void fn_7100a3ba90(UnkCallbackSetVector* v, const UnkCallbackSetElem* value);
void fn_7100a95868(UnkCallbackSetVector* v, const UnkCallbackSetElem* value);
struct UnkCallbackSetVector {
    UnkCallbackSetElem* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    template <void (*Reserve)(UnkCallbackSetVector*, u32),
              void (*PushBack)(UnkCallbackSetVector*, const UnkCallbackSetElem*)>
    __attribute__((always_inline)) void CopyConstruct(const UnkCallbackSetVector& other) {
        data = nullptr;
        count = 0;
        capacity = 0;
        step = other.step;
        field_0x14 = other.field_0x14;
        flags = other.flags;
        allocator = other.allocator;
        Reserve(this, other.capacity);
        for (const UnkCallbackSetElem *p = other.data, *end = p + other.count; p != end; ++p)
            PushBack(this, p);
    }

    __attribute__((always_inline)) void Reserve(u32 n) {
        if (capacity >= n)
            return;
        UnkCallbackSetElem* p =
            static_cast<UnkCallbackSetElem*>(allocator->vf_0x40((u64)n * sizeof(UnkCallbackSetElem), 8, flags));
        if (data) {
            UnkCallbackSetElem* src = data;
            UnkCallbackSetElem* dst = p;
            for (u32 i = 0; i < count; ++i, ++dst, ++src) {
                new (dst) UnkCallbackSetElem(static_cast<UnkCallbackSetElem&&>(*src));
                src->~UnkCallbackSetElem();
            }
            allocator->vf_0x48(data);
        }
        data = p;
        capacity = n;
    }

    __attribute__((always_inline)) void Erase(u32 index) {
        if (index >= count)
            return;
        data[index].~UnkCallbackSetElem();
        for (u64 i = index; i < count - 1; ++i) {
            UnkCallbackSetElem* e = &data[i];
            new (e) UnkCallbackSetElem(static_cast<UnkCallbackSetElem&&>(e[1]));
            (&data[i])[1].~UnkCallbackSetElem();  // not data[i + 1], which indexes differently
        }
        --count;
    }

    __attribute__((always_inline)) u32 GrownCapacity() const {
        u32 n = capacity;
        if (n <= count && step != 0)
            n = (count / step + 1) * step;
        return n;
    }

    // Reserve is this array type's out-of-line copy of Reserve.
    template <void (*Reserve)(UnkCallbackSetVector*, u32)>
    __attribute__((always_inline)) void Insert(UnkCallbackSetElem&& value, u32 index) {
        if (index == count) {
            u32 n = count;
            u32 new_capacity = capacity;
            if (new_capacity <= n && step != 0)
                new_capacity = (n / step + 1) * step;
            if ((s32)n >= (s32)new_capacity)  // hangs, as UnkVector::PushBack does
                for (;;) {
                }
            Reserve(this, new_capacity);
            new (&data[count]) UnkCallbackSetElem(static_cast<UnkCallbackSetElem&&>(value));
        } else {
            Reserve(this, GrownCapacity());
            for (u32 i = count - 1; i >= index; --i) {
                UnkCallbackSetElem* e = &data[i];
                new (&e[1]) UnkCallbackSetElem(static_cast<UnkCallbackSetElem&&>(*e));
                data[i].~UnkCallbackSetElem();
                if (i == 0)
                    break;
            }
            new (&data[index]) UnkCallbackSetElem(static_cast<UnkCallbackSetElem&&>(value));
        }
        ++count;
    }

    __attribute__((always_inline)) void Truncate(u32 size) {
        for (u64 i = size; i < count; ++i)
            data[i].~UnkCallbackSetElem();
        count = size;
    }
};

template <void (*Sync)(void*)>
static inline __attribute__((always_inline)) void Begin(const UnkCallbackSetVector* v, UnkIter<Sync>* out) {
    *out = UnkIter<Sync>(0, v->data);
}

// Three copies (one per array type), each with its own sync function. End's braces matter: they
// evaluate the count before the data, as the original does.

void fn_7101114d10(UnkCallbackSetVector* v, u32 index) {
    v->Erase(index);
}

