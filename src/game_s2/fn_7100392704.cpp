// Advances a timed state: reads the elapsed time of the record at +0x30 (fn_7100394e2c), adds the frame
// time, hands the new value back (fn_71003954fc) and returns the current state (+0x58) while it is
// below the limit at +0x5c; once it reaches the limit, resets the record when the flag at +0x38 is set
// and returns 5.
#include "types.h"

struct UnkSelf {
    u8 pad_0x0[0x30];
    u8 field_0x30[8];
    u8 field_0x38;
    u8 pad_0x39[0x1f];
    u32 field_0x58;
    f32 field_0x5c;
};

void fn_7100394e2c(UnkSelf* self, void* record, f32* out);
void fn_71003954fc(UnkSelf* self, void* record, f32 value);

u32 fn_7100392704(UnkSelf* self, f32 delta) {
    f32 elapsed = 0.0f;
    fn_7100394e2c(self, self->field_0x30, &elapsed);
    elapsed = elapsed + delta;
    fn_71003954fc(self, self->field_0x30, elapsed);
    if (elapsed >= self->field_0x5c) {
        if (self->field_0x38)
            fn_71003954fc(self, self->field_0x30, 0.0f);
        return 5;
    }
    return self->field_0x58;
}
