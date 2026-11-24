// Advances a timed state: reads the elapsed time of the record at +0x30 (fn_7100394e2c), adds the frame
// time, hands the new value back (fn_71003954fc) and returns the current state (+0x58) while it is
// below the limit at +0x5c; once it reaches the limit, resets the record when the flag at +0x38 is set
// and returns 5.
#include "game/behaviortree/CBehavior.h"
#include "types.h"

struct UnkSelf {
    u8 pad_0x0[0x30];
    u8 sElapsedTime[8];
    u8 bResetElapsedTimeOnSuccess;
    u8 pad_0x39[0x1f];
    u32 eReturnStatusFailed;
    f32 field_0x5c;
};

u32 fn_7100392704(UnkSelf* self, f32 delta) {
    f32 elapsed = 0.0f;
    fn_7100394e2c(self, self->sElapsedTime, &elapsed);
    elapsed = elapsed + delta;
    fn_71003954fc(self, self->sElapsedTime, elapsed);
    if (elapsed >= self->field_0x5c) {
        if (self->bResetElapsedTimeOnSuccess)
            fn_71003954fc(self, self->sElapsedTime, 0.0f);
        return 5;
    }
    return self->eReturnStatusFailed;
}
