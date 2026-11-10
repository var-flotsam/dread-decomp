// CGrappleBeamGun's unit (tools/tu_map.py unit 1301): the gun's update (fn_7100833a58). After the base
// update, while the owner's gun (+0x2a8) is ready (fn_710083f7a0) it switches the gun to the id at
// +0x1240 of the game manager's id table (fn_7100839f68) unless the grapple state forbids it: in mode 2
// when the state at +0x50 is 2 or the tunable's mode is 3 or 4; otherwise when a mode-3 link is held,
// +0x1890 or +0x678 is set, fn_7100818978 refuses the target at +0x1888 or fn_71008408b0 accepts.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

// The game manager's id table (as src/game/CGunComponent/CGunComponent_Charge_ovb0091.cpp).
struct UnkGameManagerIdTable {
    u8 pad_0x0[0x1240];
    CStrId field_0x1240;
    u8 pad_0x1248[0x2578 - 0x1248];
    UnkGameManagerIdTable() asm("fn_7100031894");
    ~UnkGameManagerIdTable() asm("fn_71000407f0");
};
inline UnkGameManagerIdTable& GrappleIdTable() {
    static UnkGameManagerIdTable s_table;
    return s_table;
}

struct UnkTunable;
extern UnkTunable g_unk_7172725bc8;

struct UnkGrappleTunableView_ovb0132 {
    u8 pad_0x0[0x2c];
    u32 mode;  // +0x2c
};

struct UnkGun_ovb0091;
void fn_7100839f68(UnkGun_ovb0091* self, const CStrId& id);

struct UnkGrappleState_ovb0132 {
    u8 pad_0x0[0x10];
    s32 state;
};

struct UnkGrappleLink_ovb0132 {
    u8 pad_0x0[0x10];
    void* target;
};

struct UnkGrappleOwnerSub_ovb0132 {
    u8 pad_0x0[0x50];
    UnkGrappleState_ovb0132* field_0x50;
    u8 pad_0x58[0x620];
    bool field_0x678;
    u8 pad_0x679[0x7];
    UnkGrappleLink_ovb0132* field_0x680;
};

struct UnkGrappleGun_ovb0132 {
    u8 pad_0x0[0x138];
    s32 mode;  // +0x138
};

struct UnkGrappleOwner_ovb0132 {
    u8 pad_0x0[0x2a8];
    UnkGrappleGun_ovb0132* gun;  // +0x2a8
    UnkGrappleOwnerSub_ovb0132* field_0x2b0;
};

struct UnkGrappleBeamGun_ovb0132 {
    u8 pad_0x0[0x18];
    UnkGrappleOwner_ovb0132* owner;  // +0x18
    u8 pad_0x20[0x1868];
    UnkGrappleLink_ovb0132* field_0x1888;
    bool field_0x1890;
};
static_assert(__builtin_offsetof(UnkGrappleBeamGun_ovb0132, field_0x1890) == 0x1890, "layout");

void fn_7100857418(UnkGrappleBeamGun_ovb0132* self);
bool fn_710083f7a0(UnkGrappleGun_ovb0132* gun);
bool fn_7100818978(void* target);
bool fn_71008408b0(UnkGrappleGun_ovb0132* gun, UnkGrappleBeamGun_ovb0132* self);

void fn_7100833a58(UnkGrappleBeamGun_ovb0132* self) {
    fn_7100857418(self);
    UnkGrappleOwner_ovb0132* owner = self->owner;
    if (!owner)
        return;
    UnkGrappleGun_ovb0132* gun = owner->gun;
    if (!gun || !fn_710083f7a0(gun))
        return;
    UnkGrappleOwnerSub_ovb0132* sub = self->owner->field_0x2b0;
    if (gun->mode == 2) {
        UnkGrappleState_ovb0132* state = sub->field_0x50;
        if (state && state->state == 2)
            return;
        u32 mode = reinterpret_cast<UnkGrappleTunableView_ovb0132*>(&g_unk_7172725bc8)->mode;
        if (mode - 3 < 2)
            return;
    } else {
        if (!sub)
            return;
        if (gun->mode == 3 && sub->field_0x680 && sub->field_0x680->target)
            return;
        if (self->field_0x1890 || sub->field_0x678)
            return;
        if (!fn_7100818978(self->field_0x1888 ? self->field_0x1888->target : nullptr))
            return;
        if (fn_71008408b0(gun, self))
            return;
    }
    fn_7100839f68(reinterpret_cast<UnkGun_ovb0091*>(self->owner->gun), GrappleIdTable().field_0x1240);
}
