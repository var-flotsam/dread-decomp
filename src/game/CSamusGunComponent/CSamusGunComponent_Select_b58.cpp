// CSamusGunComponent unit (unit 1315 of tools/tu_map.py): fn_710085a2f4, when the owner's (+0x30)
// sub-object at +0x220 is a CSamusAnimationComponent (fn_7100713bb0), clears +0x384, sends it the
// primary (fn_710085961c) or secondary (fn_7100859798) weapon key under kind 0x5d523009b8f43e5f with
// mode 0x80 (otherwise as UnkSpitSend, src/game_s2/UnkSpitSend.h) and gives the owner the key
// "onprimaryweaponselected" or "onsecondaryweaponselected" (fn_7100d7c828); for the secondary weapon
// it then calls fn_7100859938. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CName.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkEnergyGun_b52;
u64 fn_7100859798(UnkEnergyGun_b52* self);

struct UnkSamusGun_b58;
u64 fn_710085961c(UnkSamusGun_b58* self);
void fn_7100859938(UnkSamusGun_b58* self);

struct UnkSamusGunOwner_b58 {
    u8 pad_0x0[0x220];
    UnkReflObject* field_0x220;
};

struct UnkSamusGun_b58 {
    u8 pad_0x0[0x30];
    UnkSamusGunOwner_b58* owner;
    u8 pad_0x38[0x384 - 0x38];
    u32 field_0x384;
};

void fn_710085a2f4(UnkSamusGun_b58* self, bool secondary) {
    UnkReflObject* target = self->owner->field_0x220;
    if (target && fn_710009d50c(target->vf_0x0(), fn_7100713bb0())) {
        self->field_0x384 = 0;
        u64 hash = secondary ? fn_7100859798(reinterpret_cast<UnkEnergyGun_b52*>(self))
                             : fn_710085961c(self);
        base::reflection::UnkReflTypeIdArg mode;
        mode.id = 0x80;
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7101176690(target, hash, 0x5d523009b8f43e5f, &mode, &g_unk_710164b958, &g_unk_7172769740,
                      &mask);
        u64 key = secondary ? base::global::CName("onsecondaryweaponselected").hash
                            : base::global::CName("onprimaryweaponselected").hash;
        fn_7100d7c828(reinterpret_cast<UnkActor*>(self->owner), &key, false);
    }
    if (secondary)
        fn_7100859938(self);
}
