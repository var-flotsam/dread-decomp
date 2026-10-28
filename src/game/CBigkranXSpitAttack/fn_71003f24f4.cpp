// CBigkranXSpitAttack: switches to the state `state` (kept at +0x34) when it differs, and sets
// up what that state needs: states 0 and 1 take a distance from the attack's owner object
// (+0x28, at +0xf0); state 2 takes a position from the sub-object that fn_7100d7c990 finds in
// the owner (+0x8) plus two values of the owner object; state 3 forwards a request (under two
// fixed 64-bit ids) to the sub-object at +0x220 of the owner.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkActor.h"
#include "game_s2/UnkSpitAttack.h"

void fn_71003f24f4(UnkSpitAttack* self, u32 state) {
    if (self->field_0x34 == state)
        return;
    self->field_0x34 = state;
    switch (state) {
    case 0:
    case 1:
        self->field_0xe0 = self->field_0x28->field_0xf0;
        break;
    case 2:
        if (self->field_0x8) {
            if (UnkActor* target = fn_7100d7c990(self->field_0x8)) {
                if (UnkSpitData* config = self->field_0x28) {
                    self->field_0xdc = config->field_0xec;
                    self->field_0xe4 = 0;
                    UnkSpitVec3 position = {target->field_0x44, target->field_0x54, target->field_0x64};
                    self->field_0xec = position;
                }
            }
        }
        break;
    case 3:
        if (self->field_0x8) {
            if (void* target = self->field_0x8->field_0x220) {
                UnkSpitSend(target, 0xcd8fc2f707c5dbb3, 0xf4061ff25a2163d6);
            }
        }
        break;
    }
}
