// CAutclastChargeAttack (tools/tu_map.py): the reset. The shared attack reset (fn_71003609b8,
// game/UnkAttack.h), then the attack unregisters its listener (+0x38) from the object behind the weak
// handle at +0x50 (fn_71010cb2fc) and drops the handle, and drops the reference each of its entries
// (+0x90, count +0x98, 0x28 bytes each) holds at +0x20. Names and layouts are placeholders.
#include "game/CEnemyMovement.h"
#include "game/UnkAttack.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkChargeEntry_b49 {
    u8 pad_0x0[0x20];
    UnkRefObject* ref;
};

struct UnkChargeAttack_b49 {
    void** vtable;
    void* actor;
    u8 pad_0x10[0x28];
    u8 link[0x18];
    UnkWeakHandle* handle;
    u8 pad_0x58[0x38];
    UnkChargeEntry_b49* entries;
    u32 count;
};

void fn_71003b73d4(UnkChargeAttack_b49* self) {
    fn_71003609b8(reinterpret_cast<UnkAttackBase*>(self));
    if (UnkWeakHandle* handle = self->handle) {
        if (handle->target) {
            fn_71010cb2fc(handle->target, self->link);
            if (!self->handle)
                goto entries;
        }
        ResetRef(&self->handle);
    }
entries:
    for (UnkChargeEntry_b49 *it = self->entries, *end = it + self->count; it != end; ++it)
        ResetRef(&it->ref);
}
