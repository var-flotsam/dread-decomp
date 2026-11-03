// GUI::CSlotInfoComposition's unit (tools/tu_map.py, the name from reflection): an event handler: the
// container's own handling first (fn_71011d99e4); on an event of kind 2 (+0x20) the composition is
// hidden (fn_71011cdc30), so is its "EnergyTanksComposition" child when it is a
// GUI::CDisplayObjectContainer (then refreshed, fn_71011cd5a0), and the composition updates (vf_0x60).
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGuiDisplayObject.h"
#include "types.h"

using base::global::CStrId;

struct UnkGuiEvent_b49 {
    u8 pad_0x0[0x20];
    u64 kind;  // 0x20
};

void fn_71011d99e4(UnkGuiDisplayObject* self, UnkGuiEvent_b49* event);  // the container's handler
using UnkGuiObj = UnkGuiDisplayObject;
void fn_71011cd5a0(UnkGuiObj* object);

static inline UnkGuiDisplayObject* AsContainer(UnkGuiDisplayObject* obj) {
    return obj && fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(obj->GetClass()),
                                reinterpret_cast<const base::reflection::CType*>(fn_71011d7b84()))
               ? obj
               : nullptr;
}

void fn_7100b79df4(UnkGuiDisplayObject* self, UnkGuiEvent_b49* event) {
    fn_71011d99e4(self, event);
    if (event->kind != 2)
        return;
    fn_71011cdc30(self, false);
    UnkGuiDisplayObject* tanks = AsContainer(fn_71011d94d4(self, CStrId("EnergyTanksComposition", true)));
    if (tanks) {
        fn_71011cdc30(tanks, false);
        fn_71011cd5a0(tanks);
    }
    self->vf_0x60();
}
