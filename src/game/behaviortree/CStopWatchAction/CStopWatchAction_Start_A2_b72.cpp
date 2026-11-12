// behaviortree::CStopWatchAction's unit (tools/tu_map.py): the start: after the base start
// (fn_7100394bd0), when the elapsed time of the record at +0x30 (fn_7100394e2c) is zero within 0.001, the
// limit (+0x5c) becomes the midpoint of the minimum (+0x3c, blackboard override +0x48) and maximum (+0x40,
// override +0x50). Names and layouts are placeholders.
#include "game/behaviortree/CBehavior.h"
#include "types.h"

struct UnkStruct_0x68;
// Signature as src/game/behaviortree/CStopWatchAction/fn_7100392704.cpp declares it.
struct UnkSelf;

struct UnkStopWatch_A2_b72 {
    u8 pad_0x0[0x30];
    u8 field_0x30[8];
    u8 pad_0x38[0x4];
    f32 field_0x3c;
    f32 field_0x40;
    u8 pad_0x44[0x4];
    u8 field_0x48[8];
    u8 field_0x50[8];
    u8 pad_0x58[0x4];
    f32 field_0x5c;
};

void fn_7100392648(UnkStopWatch_A2_b72* self) {
    UnkSelf* s = reinterpret_cast<UnkSelf*>(self);
    fn_7100394bd0(reinterpret_cast<UnkStruct_0x68*>(self));
    f32 elapsed = 0.0f;
    fn_7100394e2c(s, self->field_0x30, &elapsed);
    if (elapsed + 0.001f >= 0.0f && elapsed + -0.001f <= 0.0f) {
        f32 lo = self->field_0x3c;
        fn_7100394e2c(s, self->field_0x48, &lo);
        f32 hi = self->field_0x40;
        fn_7100394e2c(s, self->field_0x50, &hi);
        self->field_0x5c = lo + (hi - lo) * 0.5f;
    }
}
