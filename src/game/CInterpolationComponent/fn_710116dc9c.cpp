// CInterpolationComponent unit (tools/tu_map.py unit 2718): runs the base step (fn_71010c4228), then puts
// the interpolation state back to its defaults (four value pairs: counters, a unit speed and a [1, -1]
// range) and drops the pointer at +0x88. Names and layouts are placeholders.
#include "game/CComponent.h"
#include "types.h"

struct UnkCounts_l16dc9c {
    u32 first;
    u32 second;
};

struct UnkStep_l16dc9c {
    u32 count;
    f32 speed;
};

struct UnkRange_l16dc9c {
    f32 start;
    f32 end;
};

struct UnkInterpolationComponent_l16dc9c {
    u8 pad_0x0[0x58];
    u32 field_0x58;
    UnkCounts_l16dc9c field_0x5c;
    UnkStep_l16dc9c field_0x64;
    UnkCounts_l16dc9c field_0x6c;
    UnkRange_l16dc9c field_0x74;
    u8 pad_0x7c[0xc];
    void* field_0x88;
};

static const UnkCounts_l16dc9c kStartCounts = {0, 1};
static const UnkStep_l16dc9c kStartStep = {1, 1.0f};
static const UnkCounts_l16dc9c kStartRepeat = {1, 1};
static const UnkRange_l16dc9c kStartRange = {1.0f, -1.0f};

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_710116dc9c(UnkInterpolationComponent_l16dc9c* self) {
    fn_71010c4228(reinterpret_cast<UnkParent*>(self));
    self->field_0x58 = 0;
    self->field_0x5c = kStartCounts;
    self->field_0x74 = kStartRange;
    self->field_0x6c = kStartRepeat;
    self->field_0x88 = nullptr;
    self->field_0x64 = kStartStep;
}
