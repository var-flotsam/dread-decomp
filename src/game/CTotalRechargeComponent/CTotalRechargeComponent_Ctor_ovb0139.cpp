// CTotalRechargeComponent's unit (tools/tu_map.py): the constructor. After the recharge base constructor (fn_7100a9e244) it sets the vtable and three
// empty string ids (+0x540, +0x548, +0x550; the pool is made first when there is none). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkRechargeBase_ovb0139;
void fn_7100a9e244(UnkRechargeBase_ovb0139* self);
extern void* g_unk_7101a932d0[];

struct UnkRecharge_ovb0139 {
    void** vtable;
    u8 pad_0x8[0x538];
    CStrId id_0x540;
    CStrId id_0x548;
    CStrId id_0x550;
};
static_assert(__builtin_offsetof(UnkRecharge_ovb0139, id_0x550) == 0x550, "layout");

static inline void UnkSetEmptyId(CStrId* id) {
    id->data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    id->data = &g_unk_7101d47108->empty;
}

void fn_7100a96f10(UnkRecharge_ovb0139* self) {
    fn_7100a9e244(reinterpret_cast<UnkRechargeBase_ovb0139*>(self));
    self->id_0x540.data = nullptr;
    self->vtable = &g_unk_7101a932d0[2];
    UnkSetEmptyId(&self->id_0x540);
    UnkSetEmptyId(&self->id_0x548);
    UnkSetEmptyId(&self->id_0x550);
}
