// Destructor of an unidentified class: if its string id at 0x500 is set, unregisters it from a
// lazily created manager, resets it to a shared global id, releases it, then runs the base
// class destructor.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

void* fn_71000839a0(u64 size, u32 flags);

struct UnkManager_7100416e80 {
    u8 unk_0x0[0x70];
};
void fn_7100db37ec(UnkManager_7100416e80* self);
void fn_7100db3f24(UnkManager_7100416e80* self, CStrId* id);

extern UnkManager_7100416e80* unk_7101cd20a0;  // the manager instance (reached through the GOT)
extern CStrId unk_7101cba0c8;                  // a shared id (reached through the GOT)
extern void* unk_7101cd7300[];                 // vtable (reached through the GOT)

struct UnkStruct_7100416e80 {
    void** vtable;
    u8 unk_0x8[0x4f8];
    CStrId field_0x500;
};

void fn_7100413720(UnkStruct_7100416e80* self);

static inline UnkManager_7100416e80* GetManager() {
    if (!unk_7101cd20a0) {
        UnkManager_7100416e80* m = static_cast<UnkManager_7100416e80*>(fn_71000839a0(0x70, 0x3f));
        fn_7100db37ec(m);
        unk_7101cd20a0 = m;
    }
    return unk_7101cd20a0;
}

void fn_7100416e80(UnkStruct_7100416e80* self) {
    self->vtable = &unk_7101cd7300[2];
    if (self->field_0x500.data != &g_unk_7101d47108->empty) {
        fn_7100db3f24(GetManager(), &self->field_0x500);
        self->field_0x500 = unk_7101cba0c8;
    }
    // The id's destructor, inlined (decrement and test in two steps).
    if (base::global::UnkStringPool* pool = g_unk_7101d47108) {
        base::global::UnkStringData*& data = self->field_0x500.data;
        if (data && data != &pool->empty) {
            --data->ref_count;
            if (data->ref_count == 0) {
                fn_71000803a4(pool, data);
                pool = g_unk_7101d47108;
            }
        }
        data = &pool->empty;
    }
    fn_7100413720(self);
}
