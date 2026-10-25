// Destructors of a small class family: the base (vtable 0x7101a9c8e0) holds a string at 0x28 and
// a name holder at 0x70 (a string cleaned up out of line by fn_710007ed40 before it is
// destroyed). One derived class (0x7101a9ca40) adds a string id at 0xf8, another (0x7101a9cb48)
// a reference-counted pointer at 0xf0, and a class derived from that (0x7101a9c938) a string id
// at 0xf8.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CRntString;
using base::global::UnkStringData;
using base::global::UnkStringPool;

extern void* g_unk_7101a9c8e0[];
extern void* g_unk_7101a9ca40[];
extern void* g_unk_7101a9cb48[];
extern void* g_unk_7101a9c938[];

// A string id whose destructor reloads the id after the decrement.
struct UnkReleasedStrId {
    UnkStringData* data;

    __attribute__((always_inline)) ~UnkReleasedStrId() {
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

struct UnkRefObject {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;
};

struct UnkRefPtr {
    UnkRefObject* ptr;

    __attribute__((always_inline)) ~UnkRefPtr() {
        if (UnkRefObject* p = ptr) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

struct UnkNameHolder {
    CRntString name;

    __attribute__((always_inline)) ~UnkNameHolder() { fn_710007ed40(&name); }
};

__attribute__((always_inline)) static inline void SetVtable(void* self, void** vtable) {
    *reinterpret_cast<void***>(self) = vtable + 2;
}

// (The root's string is destroyed last.)
struct UnkNamedRoot {
    void** vtable;
    u64 field_0x8[4];
    CRntString name;
    u64 field_0x58[3];
};

struct UnkNamedBase : UnkNamedRoot {
    UnkNameHolder holder;
    u64 field_0xa0[10];

    __attribute__((always_inline)) ~UnkNamedBase() { SetVtable(this, g_unk_7101a9c8e0); }
};

struct UnkNamedWithId : UnkNamedBase {
    u64 field_0xf0;
    UnkReleasedStrId id;

    __attribute__((always_inline)) ~UnkNamedWithId() { SetVtable(this, g_unk_7101a9ca40); }
};

struct UnkNamedWithRef : UnkNamedBase {
    UnkRefPtr ref;

    __attribute__((always_inline)) ~UnkNamedWithRef() { SetVtable(this, g_unk_7101a9cb48); }
};

struct UnkNamedWithRefId : UnkNamedWithRef {
    UnkReleasedStrId id;

    __attribute__((always_inline)) ~UnkNamedWithRefId() { SetVtable(this, g_unk_7101a9c938); }
};

// NON_MATCHING(minor): the base's vtable store post-increments to the string at 0x28 instead of
// the holder at 0x70, so the two addresses are formed the other way round.
void fn_7100b395fc(UnkNamedBase* self) { self->~UnkNamedBase(); }
// NON_MATCHING(minor): as fn_7100b395fc.
void fn_7100b3955c(UnkNamedWithId* self) { self->~UnkNamedWithId(); }
// NON_MATCHING(minor): as fn_7100b395fc.
void fn_7100b47eb8(UnkNamedWithRef* self) { self->~UnkNamedWithRef(); }
// NON_MATCHING(minor): as fn_7100b395fc.
void fn_7100b39640(UnkNamedWithRefId* self) { self->~UnkNamedWithRefId(); }
// NON_MATCHING(minor): as fn_7100b395fc.
void fn_7100b39714(UnkNamedWithRefId* self) { self->~UnkNamedWithRefId(); }

// Deleting destructors of the base (four copies).
// NON_MATCHING(minor): as fn_7100b395fc, and the deleted pointer takes a third register.
void fn_7100b4797c(UnkNamedBase* self) {
    self->~UnkNamedBase();
    operator delete(self);
}
// NON_MATCHING(minor): as fn_7100b4797c.
void fn_7100b479ec(UnkNamedBase* self) {
    self->~UnkNamedBase();
    operator delete(self);
}
// NON_MATCHING(minor): as fn_7100b4797c.
void fn_7100b47a38(UnkNamedBase* self) {
    self->~UnkNamedBase();
    operator delete(self);
}
// NON_MATCHING(minor): as fn_7100b4797c.
void fn_7100b47b2c(UnkNamedBase* self) {
    self->~UnkNamedBase();
    operator delete(self);
}
// NON_MATCHING(minor): as fn_7100b3955c, deleting.
void fn_7100b47a84(UnkNamedWithId* self) {
    self->~UnkNamedWithId();
    operator delete(self);
}
