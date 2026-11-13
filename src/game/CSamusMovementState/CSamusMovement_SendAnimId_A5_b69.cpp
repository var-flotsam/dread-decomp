// CSamusMovement's unit: after fn_71010dbcbc, sends id 2 to the actor's +0x220 controller (when there is
// one) through fn_7101178564 while the flag at +0x7e4 is set, else through fn_71011785a8, then runs
// fn_71009e4598. Names and layouts are placeholders.
#include "game/CAnimationComponent.h"
#include "game/UnkTU1501.h"
#include "types.h"
#include "game/UnkShakernautId.h"

struct UnkShakernautCtrl_b53;

struct UnkSamusMovementUpdate;

struct UnkSamusMovementActor_A5_b69 {
    u8 unk_0x0[0x220];
    UnkShakernautCtrl_b53* ctrl;
};

struct UnkSamusMovement_A5_b69 {
    u8 unk_0x0[0x30];
    UnkSamusMovementActor_A5_b69* actor;
    u8 unk_0x38[0x7e4 - 0x38];
    bool field_0x7e4;
};

void fn_71010dbcbc(UnkSamusMovement_A5_b69* self);

void fn_71009e7cd4(UnkSamusMovement_A5_b69* self) {
    // The two ids keep separate stack slots: a declaration jumped over gets no lifetime markers.
    goto start;
    UnkShakernautId_b53 on;
    UnkShakernautId_b53 off;
start:
    fn_71010dbcbc(self);
    if (UnkShakernautCtrl_b53* ctrl = self->actor->ctrl) {
        if (self->field_0x7e4) {
            on.value = 2;
            fn_7101178564(ctrl, &on, true, true);
        } else {
            off.value = 2;
            fn_71011785a8(ctrl, &off, true, true);
        }
        fn_71009e4598(reinterpret_cast<UnkSamusMovementUpdate*>(self));
    }
}
