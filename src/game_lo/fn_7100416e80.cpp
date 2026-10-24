// Destructor of an unidentified class: if its file-path id at 0x500 is set, unregisters it from
// a lazily created manager, resets it to a shared global id, releases it, then runs the base
// class destructor.
#include "game/UnkPathRegistry.h"
#include "base/global/CFilePathStrId.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

void* fn_71000839a0(u64 size, u32 flags);

void fn_7100db37ec(UnkPathRegistry* self);
void fn_7100db3f24(UnkPathRegistry* self, CFilePathStrId* id);
// The registry and the vtable come from UnkPathRegistry.h, the shared id g_unk_7101d470e8 from
// CFilePathStrId.h; all three are reached through the GOT.

struct UnkStruct_7100416e80 {
    char* vtable;
    u8 unk_0x8[0x4f8];
    CFilePathStrId field_0x500;
};

void fn_7100413720(UnkStruct_7100416e80* self);

static inline UnkPathRegistry* GetManager() {
    if (!g_unk_71726b3240) {
        UnkPathRegistry* m = static_cast<UnkPathRegistry*>(fn_71000839a0(0x70, 0x3f));
        fn_7100db37ec(m);
        g_unk_71726b3240 = m;
    }
    return g_unk_71726b3240;
}

void fn_7100416e80(UnkStruct_7100416e80* self) {
    self->vtable = g_unk_7101a34218 + 0x10;
    if (self->field_0x500.data != &g_unk_7101d47200->empty) {
        fn_7100db3f24(GetManager(), &self->field_0x500);
        // Copy-assignment from the shared id, inlined (CFilePathStrId has none yet): retain the
        // new id, release the old one, as CStrId's on the other pool.
        UnkStringData*& data = self->field_0x500.data;
        if (data != g_unk_7101d470e8.data) {
            UnkStringData* d = g_unk_7101d470e8.data;
            UnkStringPool* pool = g_unk_7101d47200;
            if (d != &pool->empty)
                ++d->ref_count;
            if (data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0)
                    fn_71000803a4(pool, data);
            }
            data = d;
        }
    }
    // The id's destructor, inlined (decrement and test in two steps).
    if (UnkStringPool* pool = g_unk_7101d47200) {
        UnkStringData*& data = self->field_0x500.data;
        if (data && data != &pool->empty) {
            --data->ref_count;
            if (data->ref_count == 0) {
                fn_71000803a4(pool, data);
                pool = g_unk_7101d47200;
            }
        }
        data = &pool->empty;
    }
    fn_7100413720(self);
}
