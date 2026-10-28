// Destructor body of the component built by fn_7100ac2c5c: drops the bound callback, then
// releases the member ids and reference-counted objects and runs the base destructor.
#include "base/UnkCountedResource.h"
#include "base/UnkStrIdPathPair.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// Reference counted through a virtual release (count at +0x8, vf_0x10 deletes).
struct UnkRefObject {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;
};

struct UnkComponent_0x5b8 {
    void** vtable;
    u8 unk_0x8[0x480];
    UnkStringData* field_0x488;  // CFilePathStrId
    UnkStringData* field_0x490;  // CFilePathStrId
    UnkStringData* field_0x498;  // CFilePathStrId
    u8 unk_0x4a0[0x8];
    UnkRefObject* field_0x4a8;
    UnkRefObject* field_0x4b0;
    u8 unk_0x4b8[0x18];
    UnkRefObject* field_0x4d0;
    u8 unk_0x4d8[0x50];
    UnkRefObject* field_0x528;
    UnkRefObject* field_0x530;
    u8 unk_0x538[0x48];
    UnkStringData* field_0x580;  // CStrId
    UnkStringData* field_0x588;  // CFilePathStrId
    UnkCountedResource* field_0x590;
    UnkStringData* field_0x598;  // CStrId
    void* field_0x5a0;
    void* field_0x5a8;
    void* field_0x5b0;
};

extern void* g_unk_7101a96b30[];
void fn_71005c4828(void* callback, void** slot);
void fn_7100ac72a4(UnkComponent_0x5b8* self);

// A string id's destructor on the given pool: decrement, then test the reloaded count.
static inline void ReleaseId(UnkStringData** id, UnkStringPool** global_pool) {
    if (UnkStringPool* pool = *global_pool) {
        if (*id && *id != &pool->empty) {
            --(*id)->ref_count;
            if ((*id)->ref_count == 0) {
                fn_71000803a4(pool, *id);
                pool = *global_pool;
            }
        }
        *id = &pool->empty;
    }
}

static inline void Release(UnkRefObject* p) {
    if (p && --p->ref_count == 0)
        p->vf_0x10();
}

void fn_7100ac2e00(UnkComponent_0x5b8* self) {
    self->vtable = &g_unk_7101a96b30[2];
    if (self->field_0x5a0) {
        fn_71005c4828(self->field_0x5a0, &self->field_0x5a0);
        self->field_0x5a0 = nullptr;
    }
    self->field_0x5b0 = nullptr;
    self->field_0x5a8 = nullptr;
    ReleaseId(&self->field_0x598, &g_unk_7101d47108);
    fn_7100027044(reinterpret_cast<UnkStrIdPathResource*>(&self->field_0x580));
    if (UnkCountedResource* p = self->field_0x590)
        if (--p->ref_count == 0)
            fn_710001737c(p);
    ReleaseId(&self->field_0x588, &g_unk_7101d47200);
    ReleaseId(&self->field_0x580, &g_unk_7101d47108);
    Release(self->field_0x530);
    Release(self->field_0x528);
    Release(self->field_0x4d0);
    Release(self->field_0x4b0);
    Release(self->field_0x4a8);
    ReleaseId(&self->field_0x498, &g_unk_7101d47200);
    ReleaseId(&self->field_0x490, &g_unk_7101d47200);
    ReleaseId(&self->field_0x488, &g_unk_7101d47200);
    fn_7100ac72a4(self);
}
