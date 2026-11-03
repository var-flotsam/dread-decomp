// media::CMoviePlayer's unit (tools/tu_map.py unit 323): the copy, move and assignment helpers
// (CType+0x30, +0x38, +0x48 and +0x50) of a reflected handle to a shared object (an atomic count at
// +0x8, vtable slot 2 destroys it). Names are placeholders.
#include "types.h"

struct UnkShared_b52 {
    virtual void vf_0();
    virtual void vf_1();
    virtual void vf_2();
    s32 ref_count;  // 0x8
};

struct UnkSharedHandle_b52 {
    UnkShared_b52* ptr;
};

__attribute__((always_inline)) static inline void CopySharedHandle_b52(void* dst, const void* src) {
    auto* self = static_cast<UnkSharedHandle_b52*>(dst);
    auto* other = static_cast<const UnkSharedHandle_b52*>(src);
    self->ptr = nullptr;
    if (UnkShared_b52* obj = other->ptr) {
        __atomic_fetch_add(&obj->ref_count, 1, __ATOMIC_SEQ_CST);
        if (UnkShared_b52* old = self->ptr) {
            __atomic_fetch_sub(&old->ref_count, 1, __ATOMIC_SEQ_CST);
            if (__atomic_load_n(&old->ref_count, __ATOMIC_SEQ_CST) == 0)
                old->vf_2();
        }
        self->ptr = obj;
    }
}

void fn_710021e258(void* dst, const void* src) { CopySharedHandle_b52(dst, src); }
void fn_710021e2c8(void* dst, const void* src) { CopySharedHandle_b52(dst, src); }

// The assignment helpers (CType+0x48 and +0x50): unless both already hold the same object, take a
// reference of the source's object (if any) and drop the one held; returns the handle.
__attribute__((always_inline)) static inline void* AssignSharedHandle_b52(void* dst, const void* src) {
    auto* self = static_cast<UnkSharedHandle_b52*>(dst);
    auto* other = static_cast<const UnkSharedHandle_b52*>(src);
    if (self != other) {
        UnkShared_b52* obj = other->ptr;
        if (self->ptr != obj) {
            if (obj)
                __atomic_fetch_add(&obj->ref_count, 1, __ATOMIC_SEQ_CST);
            if (UnkShared_b52* old = self->ptr) {
                __atomic_fetch_sub(&old->ref_count, 1, __ATOMIC_SEQ_CST);
                if (__atomic_load_n(&old->ref_count, __ATOMIC_SEQ_CST) == 0)
                    old->vf_2();
            }
            self->ptr = obj;
        }
    }
    return self;
}

void* fn_710021e36c(void* dst, const void* src) { return AssignSharedHandle_b52(dst, src); }
void* fn_710021e3f0(void* dst, const void* src) { return AssignSharedHandle_b52(dst, src); }
