// Destructor of an object with two name strings (0x68, 0x98) and a string id (0xd8): restore the
// vtable, release the id, destroy both strings and run the base destructor (fn_710097b294).
// Near copy of fn_71006c54dc (src/game/CCharClassStartPointComponent/fn_710116baf4.cpp).
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CRntString;
using base::global::UnkStringData;
using base::global::UnkStringPool;

void fn_710097b294(void* self);

extern void* g_unk_7101a4bcf8[];

struct UnkObject_7100589b94 {
    void** vtable;
    u8 unk_0x8[0x60];
    CRntString name_0x68;
    CRntString name_0x98;
    u8 unk_0xc8[0x10];
    UnkStringData* id;
};

void fn_7100589b94(UnkObject_7100589b94* self) {
    self->vtable = &g_unk_7101a4bcf8[2];
    if (UnkStringPool* pool = g_unk_7101d47108) {
        if (self->id && self->id != &pool->empty) {
            --self->id->ref_count;
            if (self->id->ref_count == 0) {
                fn_71000803a4(pool, self->id);
                pool = g_unk_7101d47108;
            }
        }
        self->id = &pool->empty;
    }
    fn_710007ecd4(&self->name_0x98);
    fn_710007ecd4(&self->name_0x68);
    fn_710097b294(self);
}
