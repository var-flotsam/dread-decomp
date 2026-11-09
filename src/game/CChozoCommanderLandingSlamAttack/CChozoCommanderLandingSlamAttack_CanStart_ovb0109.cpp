// CChozoCommanderLandingSlamAttack (unit 654 of tools/tu_map.py): whether the attack can start. With no
// target (fn_7100d7c990) the base check (fn_7100424b50) decides; otherwise it needs the base check, the
// target's +0x1f8 state (when it has one) to have +0x3a8 set, and the target at least 150 away
// horizontally from the AI component's charge position (+0x15d1c, or +0x15d24 unless the actor's +0x220
// component is in mode 4), as CChozoCommanderAirChargeAttack's check (fn_710042510c,
// src/game/CChozoCommanderAirChargeAttack/CChozoCommanderAirChargeAttack_Charge_ovb0113.cpp) does.
// Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game_s2/UnkParams.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkTakumakuComponent;
bool fn_7101178e6c(UnkTakumakuComponent* component, u32 kind);
struct UnkRequester;
bool fn_7100424b50(UnkRequester* self, void* request, u32 flag);

struct UnkSlamAI_ovb0109 {
    u8 pad_0x0[0x15d1c];
    f32 field_0x15d1c;
    u8 pad_0x15d20[4];
    f32 field_0x15d24;
};

struct UnkSlamActor_ovb0109 {
    u8 pad_0x0[0x220];
    UnkTakumakuComponent* field_0x220;
    u8 pad_0x228[0x240 - 0x228];
    UnkReflObject* ai;  // 0x240
};

struct UnkSlamTargetState_ovb0109 {
    u8 pad_0x0[0x3a8];
    bool field_0x3a8;
};

struct UnkSlamTarget_ovb0109 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0x1f8 - 0x48];
    UnkSlamTargetState_ovb0109* field_0x1f8;
};

struct UnkSlamAttack_ovb0109 {
    u8 pad_0x0[8];
    UnkSlamActor_ovb0109* actor;
};

bool fn_7100437530(UnkSlamAttack_ovb0109* self, void* request, u32 flag) {
    bool ok = fn_7100424b50(reinterpret_cast<UnkRequester*>(self), request, flag);
    auto* target = reinterpret_cast<UnkSlamTarget_ovb0109*>(
        fn_7100d7c990(reinterpret_cast<UnkActor*>(self->actor)));
    bool result = ok & !target;
    if (!target || !ok)
        return result;
    if (target->field_0x1f8 && !target->field_0x1f8->field_0x3a8)
        return false;
    UnkReflObject* o = self->actor->ai;
    auto* ai = reinterpret_cast<UnkSlamAI_ovb0109*>(
        o && fn_710009d50c(o->vf_0x0(), fn_710044e708()) ? o : nullptr);
    f32 x = fn_7101178e6c(self->actor->field_0x220, 4) ? ai->field_0x15d1c : ai->field_0x15d24;
    return __builtin_fabsf(target->x - x) >= 150.0f;
}
