// CCharClassSaveStationUsableComponent's unit (tools/tu_map.py, unit 1595). Names and layouts are placeholders.
#include <new>
#include "base/global/CStrId.h"
#include "base/global/UnkTwoStepStrId.h"
#include "game/CCharClassUsableComponent.h"
#include "types.h"

using base::global::UnkStringData;

struct UnkSaveStationUsable_ovb0130 {
    void** vtable;
    u8 pad_0x8[0x1b0];
    UnkEmptyStrId field_0x1b8;
    UnkEmptyStrId field_0x1c0;
};

// As src/game/CCharClassElevatorUsableComponent/CCharClassElevatorUsableComponent_Ctor_ovb0072.cpp declares it.
struct UnkShipRechargeDef;
extern void* g_unk_7101a922b0[];

// fn_7100a88cbc: the constructor: the base (fn_7100a7b494), the vtable and two empty string ids.
void fn_7100a88cbc(UnkSaveStationUsable_ovb0130* self) {
    fn_7100a7b494(reinterpret_cast<UnkShipRechargeDef*>(self));
    self->vtable = g_unk_7101a922b0 + 2;
    new (&self->field_0x1b8) UnkEmptyStrId();
    new (&self->field_0x1c0) UnkEmptyStrId();
}
