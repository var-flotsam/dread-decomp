// Copies of fn_7100d40cc8, fn_7100d40ba0 (src/game_aca57c/fn_7100d40ba0.cpp), which the binary
// repeats at other addresses: fn_71002422e4, fn_71002421bc.
// The reflected collection functions Begin, End and Erase of arrays of reference-counted pointers
// (base::refl_Begin/refl_End; the iterator's cached value releases an owned pointer), and the
// move constructor of an iterator value over a map of such pointers keyed by string ids.
#include "base/UnkCollectionIter.h"
#include "base/global/CStrId.h"
#include "types.h"

struct UnkRefObject {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    union {  // (the original reloads around the count, as through a union)
        s32 ref_count;
    };
};

struct UnkRefPtr {
    UnkRefObject* ptr;

    UnkRefPtr(UnkRefPtr&& other) : ptr(nullptr) { *this = other; }
    UnkRefPtr& operator=(const UnkRefPtr& other) {
        UnkRefObject* p = other.ptr;
        if (p)
            ++p->ref_count;
        if (UnkRefObject* old = ptr) {
            if (--old->ref_count == 0)
                old->vf_0x10();
        }
        ptr = p;
        return *this;
    }
    ~UnkRefPtr() {
        if (UnkRefObject* p = ptr) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

using UnkRefPtrIter = base::UnkCollectionIter<UnkRefPtr>;

void fn_71002421bc(const base::UnkVector<UnkRefPtr>* v, UnkRefPtrIter* out) { *out = UnkRefPtrIter(0, v->data); }

void fn_71002422e4(const base::UnkVector<UnkRefPtr>* v, UnkRefPtrIter* out) { *out = UnkRefPtrIter(v->count, v->data + v->count); }

// Erase (base::refl_Erase, with UnkVector::EraseAt inlined): the pointers after the iterator's
// index move down one by one.
__attribute__((always_inline)) static inline void Erase(base::UnkVector<UnkRefPtr>* v, const UnkRefPtrIter* it) {
    u32 index = it->index;
    if (index < v->count) {
        u64 i = index;
        v->data[i].~UnkRefPtr();
        for (; i < (u32)(v->count - 1); ++i) {
            new (&v->data[i]) UnkRefPtr(static_cast<UnkRefPtr&&>(v->data[i + 1]));
            v->data[i + 1].~UnkRefPtr();
        }
        v->count = v->count - 1;
    }
}

// NON_MATCHING(minor): the original derives the loop index from the index with a shift pair and
// keeps i + 1 out of a callee-saved register (one register more here).
void fn_7100d3ff6c(base::UnkVector<UnkRefPtr>* v, const UnkRefPtrIter* it);
// NON_MATCHING(minor): as fn_7100d3ff6c.

// The same for an array of vectors of plain values (an owned vector is freed).
struct UnkPlainVector : base::UnkVector<u32> {
    ~UnkPlainVector() { Free(); }
};

using UnkPlainVectorIter = base::UnkCollectionIter<UnkPlainVector>;

// A string id key that is taken over when moved (the source is left holding the empty id).
struct UnkMovedStrId {
    base::global::UnkStringData* data;

    __attribute__((always_inline)) UnkMovedStrId(UnkMovedStrId&& other) : data(other.data) {
        other.data = &g_unk_7101d47108->empty;
    }
};

// The move constructor of an iterator value over a keyed map of such pointers.
using UnkKeyedRefPtrValue = base::UnkIterValue<UnkRefPtr, UnkMovedStrId>;

// (base::UnkIterValue's move constructor, inlined.)
__attribute__((always_inline)) static inline void Move(UnkKeyedRefPtrValue* self, UnkKeyedRefPtrValue* other) {
    using Pair = UnkKeyedRefPtrValue::Value;
    self->state = 0;
    if (other->state == 1) {
        self->state = 1;
        new (&self->value) Pair(static_cast<Pair&&>(other->value));
    } else if (other->state == 2) {
        self->state = 1;
        UnkKeyedRefPtrValue::Ref r = other->ref;
        new (&self->value) Pair{static_cast<UnkMovedStrId&&>(*const_cast<UnkMovedStrId*>(r.index)),
                                static_cast<UnkRefPtr&&>(*r.element)};
    }
}

// NON_MATCHING(minor): block layout; the original keeps the release call out of line and gives
// each path its own return.
void fn_7100d410b4(UnkKeyedRefPtrValue* self, UnkKeyedRefPtrValue* other);
// NON_MATCHING(minor): as fn_7100d410b4.

// Equality of two such iterator values: the keys by identity, then the objects' words at 0x10
// (zero for no object).
struct UnkRefObjectId : UnkRefObject {
    u64 field_0x10;
};

__attribute__((always_inline)) static inline u64 IdOf(const UnkRefPtr& p) {
    return p.ptr ? static_cast<UnkRefObjectId*>(p.ptr)->field_0x10 : 0;
}

__attribute__((always_inline)) static inline bool Equal(const UnkKeyedRefPtrValue* a, const UnkKeyedRefPtrValue* b) {
    const UnkMovedStrId* ka = a->state == 2 ? a->ref.index : &a->value.index;
    const UnkMovedStrId* kb = b->state == 2 ? b->ref.index : &b->value.index;
    if (ka->data != kb->data)
        return false;
    const UnkRefPtr* x = a->state == 2 ? a->ref.element : &a->value.element;
    const UnkRefPtr* y = b->state == 2 ? b->ref.element : &b->value.element;
    u64 id = IdOf(*y);
    return IdOf(*x) == id;
}

