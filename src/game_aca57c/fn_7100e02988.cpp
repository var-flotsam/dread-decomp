// Eight setters of an object's weak-handle slots (0x2240 to 0x2278): each stores the weak
// handle of the given target (created on first use; none for a null target) and releases the
// handle it replaces.
#include "types.h"

// A reference-counted object released through vf_0x10. The count is a union member (no
// type-based alias info: the original reloads the slot after the count changes).
struct UnkRefObject {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    union {
        s32 ref_count;
    };
};

// A weak handle to an object (0x18 bytes, vtable g_unk_7101a209e0).
struct UnkWeakHandle : UnkRefObject {
    void* target;
};

struct UnkTarget {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual UnkWeakHandle** vf_0x28();  // the object's weak handle slot
};

struct UnkHandleOwner_0x2280 {
    u8 unk_0x0[0x2240];
    UnkWeakHandle* field_0x2240;
    UnkWeakHandle* field_0x2248;
    UnkWeakHandle* field_0x2250;
    UnkWeakHandle* field_0x2258;
    UnkWeakHandle* field_0x2260;
    UnkWeakHandle* field_0x2268;
    UnkWeakHandle* field_0x2270;
    UnkWeakHandle* field_0x2278;
};

extern void* g_unk_7101a209e0[];
void* fn_71000839a0(u64 size, u32 flags);

template <class T>
static inline void AssignRef(T** slot, T* p) {
    if (*slot != p) {
        if (p)
            ++p->ref_count;
        if (T* old = *slot) {
            if (--old->ref_count == 0)
                old->vf_0x10();
        }
        *slot = p;
    }
}

template <class T>
static inline void ResetRef(T** slot) {
    if (T* p = *slot) {
        if (--p->ref_count == 0)
            p->vf_0x10();
        *slot = nullptr;
    }
}

// The weak handle of `target`, created on first use.
__attribute__((always_inline)) static inline UnkWeakHandle* GetWeakHandle(UnkTarget* target) {
    UnkWeakHandle** slot = target->vf_0x28();
    if (!slot)
        return nullptr;
    UnkWeakHandle* handle = *slot;
    if (!handle) {
        handle = static_cast<UnkWeakHandle*>(fn_71000839a0(0x18, 0x3e));
        *reinterpret_cast<void***>(handle) = &g_unk_7101a209e0[2];
        handle->ref_count = 0;
        handle->target = target;
        AssignRef(slot, handle);
    }
    return handle;
}

#define SETTER(addr, field)                                                                    \
    void fn_##addr(UnkHandleOwner_0x2280* self, UnkTarget* target) {                           \
        if (target)                                                                            \
            AssignRef(&self->field, GetWeakHandle(target));                                    \
        else                                                                                   \
            ResetRef(&self->field);                                                            \
    }

SETTER(7100e02988, field_0x2248)
SETTER(7100e02acc, field_0x2240)
SETTER(7100e02c10, field_0x2258)
SETTER(7100e02d54, field_0x2250)
SETTER(7100e02e98, field_0x2260)
SETTER(7100e02fdc, field_0x2268)
SETTER(7100e03120, field_0x2270)
SETTER(7100e03264, field_0x2278)
