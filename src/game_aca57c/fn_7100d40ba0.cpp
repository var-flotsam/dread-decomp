// The reflected collection functions Begin, End and Erase of arrays of reference-counted pointers
// (base::refl_Begin/refl_End; the iterator's cached value releases an owned pointer).
#include "base/UnkCollectionIter.h"
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

void fn_7100d40ba0(const base::UnkVector<UnkRefPtr>* v, UnkRefPtrIter* out) { *out = UnkRefPtrIter(0, v->data); }
void fn_7100d40cc8(const base::UnkVector<UnkRefPtr>* v, UnkRefPtrIter* out) { *out = UnkRefPtrIter(v->count, v->data + v->count); }
void fn_7100d84af8(const base::UnkVector<UnkRefPtr>* v, UnkRefPtrIter* out) { *out = UnkRefPtrIter(0, v->data); }
void fn_7100d84c20(const base::UnkVector<UnkRefPtr>* v, UnkRefPtrIter* out) { *out = UnkRefPtrIter(v->count, v->data + v->count); }

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
void fn_7100d3ff6c(base::UnkVector<UnkRefPtr>* v, const UnkRefPtrIter* it) { Erase(v, it); }
// NON_MATCHING(minor): as fn_7100d3ff6c.
void fn_7100d83fd8(base::UnkVector<UnkRefPtr>* v, const UnkRefPtrIter* it) { Erase(v, it); }

// The same for an array of vectors of plain values (an owned vector is freed).
struct UnkPlainVector : base::UnkVector<u32> {
    ~UnkPlainVector() { Free(); }
};

using UnkPlainVectorIter = base::UnkCollectionIter<UnkPlainVector>;

void fn_7100d44810(const base::UnkVector<UnkPlainVector>* v, UnkPlainVectorIter* out) {
    *out = UnkPlainVectorIter(0, v->data);
}
void fn_7100d4492c(const base::UnkVector<UnkPlainVector>* v, UnkPlainVectorIter* out) {
    *out = UnkPlainVectorIter(v->count, v->data + v->count);
}
