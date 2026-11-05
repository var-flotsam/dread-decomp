// CKraidAIComponent's unit (tools/tu_map.py): the area box. While the word at +0x1140 is clear or the
// slot at +0x14e38 is set, the base component's box (fn_71011063a4); otherwise a box around the target actor
// (fn_7100d7c990 of the owner at +0x30): 100 left of it to 1400 right, 100 below to 100 above. Names
// and layouts are placeholders.
#include "game/UnkActor.h"
#include "types.h"

struct UnkKraidVec_b65a4 {
    f32 x, y, z;
};

static inline UnkKraidVec_b65a4 operator+(const UnkKraidVec_b65a4& a, const UnkKraidVec_b65a4& b) {
    UnkKraidVec_b65a4 r = {a.x + b.x, a.y + b.y, a.z + b.z};
    return r;
}

struct UnkKraidBox_b65a4 {
    UnkKraidVec_b65a4 min;
    UnkKraidVec_b65a4 max;
};

struct UnkKraidAI_b65a4 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x1140 - 0x38];
    u32 field_0x1140;
    u8 pad_0x1144[0x14e38 - 0x1144];
    void* field_0x14e38;
};

void fn_71011063a4(UnkKraidAI_b65a4* self, UnkKraidBox_b65a4* out);

void fn_7100600ae4(UnkKraidAI_b65a4* self, UnkKraidBox_b65a4* out) {
    if (self->field_0x1140 == 0 || self->field_0x14e38) {
        fn_71011063a4(self, out);
        return;
    }
    UnkActor* target = fn_7100d7c990(self->owner);
    if (!target)
        return;
    UnkKraidVec_b65a4 pos = {target->field_0x44, target->field_0x54, target->field_0x64};
    UnkKraidVec_b65a4 lo = {-100.0f, -100.0f, 0.0f};
    UnkKraidVec_b65a4 hi = {1400.0f, 100.0f, 0.0f};
    out->min = pos + lo;
    out->max = pos + hi;
}
