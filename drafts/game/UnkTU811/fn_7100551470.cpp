// Unit 811 (CEmmyAIComponent hint): whether the E.M.M.I. may proceed: not while the owner's movement is in
// mode 4, and not while the movement is tracked, the timer at +0x29418 runs or the flag at +0x29f26 is
// clear. Names and layouts are placeholders.
#include "game/CCharacterMovement.h"
#include "game/CEnemyMovement.h"
#include "game/UnkActor.h"
#include "game/UnkTU39.h"
#include "types.h"

struct UnkEmmyAI_551470 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x29418 - 0x38];
    u8 timer[0xb0e];
    u8 flag;
};

bool fn_7100551470(UnkEmmyAI_551470* self) {
    bool tracked = false;
    u32 mode = 0;
    if (self->owner->field_0x218) {
        mode = fn_71010cc3a4(self->owner->field_0x218);
        if (self->owner->field_0x218)
            tracked = fn_7100979ef0(self->owner->field_0x218);
    }
    if (mode == 4)
        return false;
    bool timerRunning = fn_710002fb70(self->timer);
    bool flagClear = !timerRunning && self->flag == 0;
    return !(tracked || flagClear);
}
