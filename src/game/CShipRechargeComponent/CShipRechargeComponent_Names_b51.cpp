// CShipRechargeComponent unit (tools/tu_map.py, the name from reflection): two name getters. While the
// flag at +0x539 is set they copy the name at +0x1e8 / +0x1b8 of the char class (the object at +0x10 when
// it is a CCharClassShipRechargeComponent, fn_7100a8ae5c; the field is read even when the cast gives
// null, as in the original), else they return the default (fn_7100a9f704 / fn_7100a9f740).
// Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::global::CRntString;

struct UnkShipRechargeComponent_b51 {
    u8 unk_0x0[0x10];
    base::reflection::UnkReflObject* charClass;  // 0x10
    u8 unk_0x18[0x539 - 0x18];
    bool active;  // 0x539
};

// The defaults, read from the component (+0x500).
CRntString fn_7100a9f704(UnkShipRechargeComponent_b51* self);
CRntString fn_7100a9f740(UnkShipRechargeComponent_b51* self);

template <u32 Offset, CRntString (*Default)(UnkShipRechargeComponent_b51*)>
static inline CRntString ShipRechargeName(UnkShipRechargeComponent_b51* self) {
    if (!self->active)
        return Default(self);
    base::reflection::UnkReflObject* o = self->charClass;
    char* cls = (o && fn_710009d50c(o->vf_0x0(), fn_7100a8ae5c())) ? reinterpret_cast<char*>(o) : nullptr;
    return *reinterpret_cast<const CRntString*>(cls + Offset);
}

CRntString fn_7100a8b738(UnkShipRechargeComponent_b51* self) { return ShipRechargeName<0x1e8, fn_7100a9f704>(self); }

CRntString fn_7100a8b7bc(UnkShipRechargeComponent_b51* self) { return ShipRechargeName<0x1b8, fn_7100a9f740>(self); }
