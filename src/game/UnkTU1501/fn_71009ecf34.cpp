// CSamusMovement's unit (unit 1501): the movement's input sample, an x scale from the run info and a y copied
// from it, damped while slow motion or the speed booster is on, scaled while the morph-ball style flag (+0x7e4)
// is set, and zeroed under a few conditions of the aim component. Names and layouts are placeholders.
#include "base/reflection/ReflectedAttributes.h"
#include "game/CAbilitySpeedBooster.h"
#include "game/UnkGameManager.h"
#include "game/UnkSamusInputMovement.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2689.h"
#include "game/UnkTU437.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkPlayer;
struct UnkAbility;
struct UnkSamusRunInfo;

f32 fn_7100796590(UnkSamusRunInfo* info);

static inline f32 TunableAt(const base::reflection::UnkTunable& tunable, u32 offset) {
    return *reinterpret_cast<const f32*>(reinterpret_cast<const char*>(&tunable) + offset);
}

void fn_71009ecf34(UnkSamusInputMovement* self, UnkSamusInputSample* out, bool stopWhenIdle) {
    out->y = self->info->field_0xac;
    out->x = fn_7100796590(self->info);
    if (UnkAbility* ability = fn_7100df5928(reinterpret_cast<UnkPlayer*>(g_unk_7101d47310))) {
        if (fn_710033cf20(ability))
            out->x = TunableAt(g_unk_71726b0fc0, 0x30) * out->x;
    }
    UnkGameManager* manager = g_unk_7101d47310;
    const u8* bytes = reinterpret_cast<const u8*>(manager);
    if ((manager->field_0xc8 != 0 || manager->field_0x10c) && bytes[0xc4] && !bytes[0xc6]) {
        out->y.value = TunableAt(g_unk_7172a03d80, 0x2c) * out->y.value;
        out->x = fn_71010d6858() * out->x;
    }
    if (self->field_0x7e4) {
        out->x = TunableAt(g_unk_7172a03d80, 0x34) * out->x;
        out->y.value = TunableAt(g_unk_7172a03d80, 0x30) * out->y.value;
    }
    UnkSamusAim* aim = self->actor->aim;
    if (aim && aim->field_0x360 && !fn_710034f8dc(reinterpret_cast<UnkAimComponent_34f8dc*>(aim)))
        out->x = 0.0f;
    if (self->field_0x7c == 0.0f && stopWhenIdle)
        out->x = 0.0f;
}
