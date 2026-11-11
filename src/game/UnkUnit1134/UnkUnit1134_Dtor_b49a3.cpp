// Unit 1134 of tools/tu_map.py (no name hint). Its destructors: the vtable, the member at +0xb0
// (fn_710002f67c), the string id at +0x30 reset to the pool's empty id (released when its count drops
// to zero), then the counted reference at +0x28 dropped; fn_710070d434 also frees the object. Names and
// layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkTU39.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

extern void* g_unk_7101a6c048[];

struct UnkUnit1134Object_b49a3 {
    void** vtable;
    u8 pad_0x8[0x20];
    UnkRefObject* ref;  // 0x28
    UnkStringData* id;  // 0x30, a CStrId
    u8 pad_0x38[0x78];
    u8 field_0xb0[0x8];
};

__attribute__((always_inline)) static inline void Destroy_b49a3(UnkUnit1134Object_b49a3* self) {
    self->vtable = g_unk_7101a6c048 + 2;
    fn_710002f67c(self->field_0xb0);
    if (UnkStringPool* pool = g_unk_7101d47108) {
        UnkStringData** id = &self->id;
        if (*id && *id != &pool->empty) {
            --(*id)->ref_count;
            if ((*id)->ref_count == 0) {
                fn_71000803a4(pool, *id);
                pool = g_unk_7101d47108;
            }
        }
        *id = &pool->empty;
    }
    if (UnkRefObject* p = self->ref) {
        if (--p->ref_count == 0)
            p->vf_0x10();
    }
}

void fn_710070d38c(UnkUnit1134Object_b49a3* self) {
    Destroy_b49a3(self);
}

void fn_710070d434(UnkUnit1134Object_b49a3* self) {
    Destroy_b49a3(self);
    operator delete(self);
}
