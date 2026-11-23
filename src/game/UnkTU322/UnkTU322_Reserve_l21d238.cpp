// Unit 322 (no reliable name): growing the capacity of a vector of refcounted pointers; each element
// is copied into the new array and the old one released. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "types.h"

struct UnkRefObj_l21d238 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;
};

static inline void RefAdd(UnkRefObj_l21d238* o) { __atomic_fetch_add(&o->ref_count, 1, __ATOMIC_SEQ_CST); }
static inline void RefRelease(UnkRefObj_l21d238* o) {
    __atomic_fetch_sub(&o->ref_count, 1, __ATOMIC_SEQ_CST);
    if (__atomic_load_n(&o->ref_count, __ATOMIC_SEQ_CST) == 0)
        o->vf_0x10();
}

struct UnkRef {
    UnkRefObj_l21d238* ptr;
    UnkRef() : ptr(nullptr) {}
    UnkRef(const UnkRef& o) : ptr(o.ptr) {
        if (ptr)
            RefAdd(ptr);
    }
    ~UnkRef() {
        if (ptr)
            RefRelease(ptr);
    }
    UnkRef& operator=(const UnkRef& o) {
        UnkRefObj_l21d238* p = o.ptr;
        if (p)
            RefAdd(p);
        if (ptr)
            RefRelease(ptr);
        ptr = p;
        return *this;
    }
};

void fn_710021d238(base::global::CRntVector<UnkRef>* vec, u32 n) {
    if (vec->capacity < n) {
        auto* p = static_cast<UnkRef*>(vec->allocator->vf_0x40((u64)n * sizeof(UnkRef), 8, vec->flags));
        if (vec->data) {
            UnkRef* src = vec->data;
            UnkRef* dst = p;
            for (u32 i = 0; i < vec->count; ++i, ++dst, ++src) {
                new (dst) UnkRef();
                if (src->ptr) {
                    *dst = *src;
                    src->~UnkRef();
                }
            }
            vec->allocator->vf_0x48(vec->data);
        }
        vec->data = p;
        vec->capacity = n;
    }
}
