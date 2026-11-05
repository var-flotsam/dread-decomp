// CChozoWarriorXAIComponent's unit: the two limits along the owner's facing (the sign of its transform's m[2])
// from fn_71010fc31c / fn_71010fc284 (the CChozoWarriorAIComponent offsets), widened by 190 each way; the
// argument order flips when the owner faces left. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "types.h"

struct UnkChozoWarriorAI_b66a2;

void fn_71010fc284(UnkChozoWarriorAI_b66a2* self, f32* out);
void fn_71010fc31c(UnkChozoWarriorAI_b66a2* self, f32* out);

struct UnkChozoWarriorXOwner_A2_b69 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // +0x38
};

struct UnkChozoWarriorXAI_A2_b69 {
    u8 pad_0x0[0x30];
    UnkChozoWarriorXOwner_A2_b69* owner;  // +0x30
};

bool fn_710048f5cc(UnkChozoWarriorXAI_A2_b69* self, f32* a, f32* b) {
    const UnkMatrix44& m = fn_710002614c(&self->owner->transform);
    bool left = m.m[2] < 0.0f;
    f32* hi = left ? a : b;
    f32* lo = left ? b : a;
    UnkChozoWarriorAI_b66a2* ai = reinterpret_cast<UnkChozoWarriorAI_b66a2*>(self);
    fn_71010fc31c(ai, lo);
    fn_71010fc284(ai, hi);
    *lo += 190.0f;
    *hi += -190.0f;
    return true;
}
