// CAutsharpAIComponent unit (tools/tu_map.py unit 572): fn_71003ccbec, whether the component may pass from
// segment `from` to segment `to`: yes when they join, otherwise not between flat ground and a slope rising
// the way it faces (flag +0x14708 picks the direction). Names and layouts are placeholders.
#include "base/math/UnkSegment2D.h"
#include "types.h"

bool fn_71001263bc(char* p0, char* p1, f32 f0);

struct UnkAutsharpAI_l3ccbec {
    u8 pad_0x0[0x14708];
    bool facing_left;  // 0x14708
};

static inline UnkVec2Result Normal(const UnkSegment2D* seg) {
    return fn_7100125e78(seg);
}

bool fn_71003ccbec(UnkAutsharpAI_l3ccbec* self, const UnkSegment2D* from, const UnkSegment2D* to) {
    if (fn_71001263bc(reinterpret_cast<char*>(const_cast<UnkSegment2D*>(from)),
                      reinterpret_cast<char*>(const_cast<UnkSegment2D*>(to)), 0.001f))
        return true;
    if (self->facing_left) {
        if (Normal(to).x == 0.0f && Normal(to).y == 1.0f && Normal(from).y > 0.7f && Normal(from).y < 1.0f &&
            Normal(from).x < 0.0f)
            return false;
        if (Normal(to).y > 0.7f && Normal(to).y < 1.0f && Normal(to).x > 0.0f && Normal(from).x == 0.0f &&
            Normal(from).y == 1.0f)
            return false;
    } else {
        if (Normal(to).x == 0.0f && Normal(to).y == 1.0f && Normal(from).y > 0.7f && Normal(from).y < 1.0f &&
            Normal(from).x > 0.0f)
            return false;
        if (Normal(to).y > 0.7f && Normal(to).y < 1.0f && Normal(to).x < 0.0f && Normal(from).x == 0.0f &&
            Normal(from).y == 1.0f)
            return false;
    }
    return true;
}
