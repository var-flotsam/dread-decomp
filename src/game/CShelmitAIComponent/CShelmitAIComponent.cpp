// CShelmitAIComponent: its per-frame update (fn_71006ba718). Runs the behaviour-tree update
// (fn_7100369c44) and two steps of its own, then, unless the rate (+0xdc of the block at +0x146c8) is within
// 0.001 of zero, advances the block's value at +0xd4 by the rate times the frame time, clamped to the
// range at +0x634/+0x638 of the block's object (reaching a bound stops the rate), and applies it
// (fn_71006badf4). Ends with fn_71006ba90c.
#include "types.h"
#include "game/UnkBehaviorTree.h"

struct UnkShelmitRange {
    u8 pad_0x0[0x634];
    f32 min;
    f32 max;
};

struct UnkShelmitBlock {
    UnkShelmitRange* field_0x0;
    u8 pad_0x8[0xcc];
    f32 field_0xd4;
    u8 pad_0xd8[4];
    f32 rate;
};

struct UnkShelmitAI {
    u8 pad_0x0[0x146c8];
    UnkShelmitBlock field_0x146c8;
};

// Declared as its other caller declares it (src/game/CGobblerAIComponent/fn_71005820c4.cpp); it also takes dt.
struct UnkGobblerComponent;
void fn_71006ba194(UnkShelmitAI* self);
void fn_71006ba7ec(UnkShelmitAI* self, f32 dt);
void fn_71006badf4(UnkShelmitAI* self);
void fn_71006ba90c(UnkShelmitAI* self);

void fn_71006ba718(UnkShelmitAI* self, f32 dt) {
    fn_7100369c44(reinterpret_cast<UnkSelf_7100369c44*>(self), dt);
    fn_71006ba194(self);
    fn_71006ba7ec(self, dt);
    f32 rate = self->field_0x146c8.rate;
    if (!(rate + 0.001f >= 0.0f && rate - 0.001f <= 0.0f)) {
        UnkShelmitBlock* block = &self->field_0x146c8;
        if (UnkShelmitRange* range = block->field_0x0) {
            f32 v = block->field_0xd4 + rate * dt;
            f32 lo = range->min;
            f32 hi = range->max;
            if (!(v >= lo && v <= hi)) {
                v = v <= lo ? lo : (v >= hi ? hi : v);
                block->rate = 0.0f;
            }
            block->field_0xd4 = v;
            fn_71006badf4(self);
        }
    }
    fn_71006ba90c(self);
}
