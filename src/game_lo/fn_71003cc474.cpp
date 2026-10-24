// Destructor of an unidentified class: releases its string ids, file-path ids and a
// reference-counted object, destroys a member at 0x158, then the subobject at 8.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

extern void* g_unk_7101a2e1e8[];  // vtable

// A string id's destructor on the pool `global` points to, inlined (decrement and test in two
// steps).
static inline void ReleaseId(UnkStringData*& data, UnkStringPool*& global) {
    if (UnkStringPool* pool = global) {
        if (data && data != &pool->empty) {
            --data->ref_count;
            if (data->ref_count == 0) {
                fn_71000803a4(pool, data);
                pool = global;
            }
        }
        data = &pool->empty;
    }
}

struct UnkRefCounted_71003cc474 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();  // destroys the object when the count drops to zero
    s32 ref_count;
};

struct UnkMember_71003cc474 {
    u8 unk_0x0[0x30];
};

struct UnkBase_71003cc474 {
    u8 unk_0x0[0x138];
};
void fn_71008c1558(UnkBase_71003cc474* self);

struct UnkStruct_71003cc474 {
    void** vtable;
    UnkBase_71003cc474 base;  // 0x8
    UnkStringData* field_0x140;  // CStrId
    u8 unk_0x148[8];
    UnkStringData* field_0x150;  // CFilePathStrId
    UnkMember_71003cc474 field_0x158;
    UnkRefCounted_71003cc474* field_0x188;
    u8 unk_0x190[0x20];
    UnkStringData* field_0x1b0;  // CStrId
    u8 unk_0x1b8[0x20];
    UnkStringData* field_0x1d8;  // CFilePathStrId
};

void fn_71003cc474(UnkStruct_71003cc474* self) {
    self->vtable = &g_unk_7101a2e1e8[2];
    ReleaseId(self->field_0x1d8, g_unk_7101d47200);
    ReleaseId(self->field_0x1b0, g_unk_7101d47108);
    if (UnkRefCounted_71003cc474* p = self->field_0x188) {
        if (--p->ref_count == 0)
            p->vf_0x10();
    }
    fn_710007ecd4(reinterpret_cast<base::global::CRntString*>(&self->field_0x158));
    ReleaseId(self->field_0x150, g_unk_7101d47200);
    ReleaseId(self->field_0x140, g_unk_7101d47108);
    fn_71008c1558(&self->base);
}
