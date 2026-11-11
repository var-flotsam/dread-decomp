// CCharClassElevatorUsableComponent's unit (tools/tu_map.py): two constructors of the same object:
// the base constructor fn_7100a7b494 (src/game/CCharClassShipRechargeComponent/), the vtable
// (unk_7101a91170) and four string ids (+0x1b8 to +0x1d0) set to the empty id; the second also calls
// the object's own vtable slot 3, as a tail call. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CCharClassUsableComponent.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

struct UnkShipRechargeDef;
extern char g_unk_7101a91170[];

struct UnkElevatorUsableDef_ovb0072 {
    void* vtable;
    u8 pad_0x8[0x1b0];
    CStrId id_0x1b8;
    CStrId id_0x1c0;
    CStrId id_0x1c8;
    CStrId id_0x1d0;
};

static inline void CopyId_ovb0072(CStrId* id, UnkStringData* data) {
    id->data = data;
    if (&g_unk_7101d47108->empty != data)
        ++data->ref_count;
}

static inline void Construct_ovb0072(UnkElevatorUsableDef_ovb0072* self) {
    fn_7100a7b494(reinterpret_cast<UnkShipRechargeDef*>(self));
    self->vtable = g_unk_7101a91170 + 0x10;
    UnkStringData* empty = g_unk_7101d470e0.data;
    CopyId_ovb0072(&self->id_0x1b8, empty);
    CopyId_ovb0072(&self->id_0x1c0, empty);
    CopyId_ovb0072(&self->id_0x1c8, empty);
    CopyId_ovb0072(&self->id_0x1d0, empty);
}

void fn_7100a7ff04(void* self) { Construct_ovb0072(static_cast<UnkElevatorUsableDef_ovb0072*>(self)); }

void fn_7100a8016c(void* obj) {
    UnkElevatorUsableDef_ovb0072* self = static_cast<UnkElevatorUsableDef_ovb0072*>(obj);
    Construct_ovb0072(self);
    // The vtable is read back as any type could have changed it (after a count update).
    typedef void (*Slot)(void*);
    typedef Slot* __attribute__((may_alias)) VtablePtr;
    VtablePtr vtable = *reinterpret_cast<VtablePtr*>(self);
    vtable[3](self);
}
