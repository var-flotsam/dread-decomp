// Copies of fn_710081622c (src/game/CCharClassGrappleBeamComponent/
// CCharClassGrappleBeamComponent_Dtor_b49a3.cpp) in CCharClassGoliathXBurstProjectionBombMovement's
// unit: two identical deleting destructors of the object fn_7100589b94 destroys (the string id at
// +0xd8 released, the strings at +0x98 and +0x68 destroyed, the base destructor fn_710097b294, then
// operator delete). Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/CCharClassGoliathXBurstProjectionBombMovement.h"
#include "game/CCharClassMovementComponent.h"
#include "types.h"

using base::global::CRntString;
using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkBurstBombObject_b52 {
    void** vtable;
    u8 unk_0x8[0x60];
    CRntString name_0x68;
    CRntString name_0x98;
    u8 unk_0xc8[0x10];
    UnkStringData* id;
};

__attribute__((always_inline)) static inline void DeleteBurstBomb_b52(UnkBurstBombObject_b52* self) {
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
    operator delete(self);
}

void fn_7100589c20(UnkBurstBombObject_b52* self) {
    DeleteBurstBomb_b52(self);
}

void fn_7100589cb8(UnkBurstBombObject_b52* self) {
    DeleteBurstBomb_b52(self);
}
