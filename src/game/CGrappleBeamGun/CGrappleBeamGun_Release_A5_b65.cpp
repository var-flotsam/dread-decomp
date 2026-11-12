// CGrappleBeamGun's unit: lets go of the grabbed object (+0x1880): clears the gun's flag at +0x1890
// unless the owner's component (+0x208 of +0x18) holds it (+0xf9), turns the object's state off
// (fn_7100232864) and unregisters it from the game manager's points (fn_71011a2a24). Names are
// placeholders.
#include "game/CScenario.h"
#include "game/UnkGameManager.h"
#include "types.h"

struct UnkEntityState;
void fn_7100232864(UnkEntityState*, bool);

struct UnkGrappleHold_A5_b65 {
    u8 pad_0x0[0xf9];
    bool held;
};

struct UnkGrappleOwner_A5_b65 {
    u8 pad_0x0[0x208];
    UnkGrappleHold_A5_b65* hold;
};

struct UnkGrappleRef_A5_b65 {
    u8 pad_0x0[0x10];
    UnkEntityState* obj;
};

struct UnkGrappleBeamGun_A5_b65 {
    u8 pad_0x0[0x18];
    UnkGrappleOwner_A5_b65* owner;
    u8 pad_0x20[0x1880 - 0x20];
    UnkGrappleRef_A5_b65* grabbed;
    u8 pad_0x1888[0x1890 - 0x1888];
    bool field_0x1890;
};

void fn_7100833ec4(UnkGrappleBeamGun_A5_b65* self) {
    UnkGrappleHold_A5_b65* hold = self->owner ? self->owner->hold : nullptr;
    UnkEntityState* obj = self->grabbed ? self->grabbed->obj : nullptr;
    if (hold && !hold->held)
        self->field_0x1890 = false;
    if (obj)
        fn_7100232864(obj, false);
    if (UnkGameManagerSub_0x608* points = g_unk_7101d47310->field_0x608)
        fn_71011a2a24(points, obj);
}
