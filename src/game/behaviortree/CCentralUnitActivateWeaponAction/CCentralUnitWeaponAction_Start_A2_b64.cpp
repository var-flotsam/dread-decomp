// behaviortree::CCentralUnitActivateWeaponAction (fn_7100401484) and CCentralUnitDeactivateWeaponAction
// (fn_7100401fa4) start: a -1 request id gives 6; otherwise the central unit AI component of the owner at
// 0x18 (when it is one) is handed to the weapon switch (fn_7100406080 / fn_7100406300) with the id, and
// the action gives 5 when that succeeded, else 6. The shape of
// src/game/behaviortree/CCentralUnitAttackAction/fn_7100401a14.cpp. Names are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::CClass;

struct UnkCentralUnitAI_b64a2 {
    virtual const CClass* vf_0x0();
};
struct UnkCentralUnitOwner_b64a2 {
    u8 unk_0x0[0x240];
    UnkCentralUnitAI_b64a2* component;
};
struct UnkCentralUnitWeaponAction_b64a2 {
    u8 unk_0x0[0x18];
    UnkCentralUnitOwner_b64a2* owner;
    u8 unk_0x20[0xc];
    s32 field_0x2c;
};

bool fn_7100406080(UnkCentralUnitAI_b64a2* component, s32 id);
bool fn_7100406300(UnkCentralUnitAI_b64a2* component, s32 id);

template <bool (*Switch)(UnkCentralUnitAI_b64a2*, s32)>
static inline __attribute__((always_inline)) u32 Start(UnkCentralUnitWeaponAction_b64a2* self) {
    if (self->field_0x2c < 0)
        return 6;
    UnkCentralUnitAI_b64a2* component = self->owner->component;
    UnkCentralUnitAI_b64a2* found = nullptr;
    if (component && fn_710009d50c(component->vf_0x0(), fn_71004030f4()))
        found = component;
    return Switch(found, self->field_0x2c) ? 5 : 6;
}

u32 fn_7100401484(UnkCentralUnitWeaponAction_b64a2* self) { return Start<fn_7100406080>(self); }
u32 fn_7100401fa4(UnkCentralUnitWeaponAction_b64a2* self) { return Start<fn_7100406300>(self); }
