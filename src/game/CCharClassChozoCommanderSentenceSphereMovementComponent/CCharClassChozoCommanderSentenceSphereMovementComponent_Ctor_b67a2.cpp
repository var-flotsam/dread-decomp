// CCharClassChozoCommanderSentenceSphereMovementComponent unit (tools/tu_map.py, the name from reflection): the
// in-place constructor its reflection descriptor points to (fn_0x28). The 0x150-byte object is value-initialised
// (cleared first), then built on the projectile movement base (fn_7100ac14f8), given its vtable and the default
// tunables at +0x134..+0x14c (90, pi/2, 30, 90, 200, 1, 4), then runs the class entries (fn_7100023854). Defined
// under the `void()` symbol the generated reflection file declares (the b64a4 alias pattern). Names and layouts are
// placeholders.
#include "base/core/CBaseObject.h"
#include "types.h"

struct UnkNode;

// As src/game/CCharClassProjectileMovement/fn_7100ac14f8.cpp defines it.
struct UnkComponent_0x138;
void fn_7100ac14f8(UnkComponent_0x138* self);

extern void* g_unk_7101a37420[];

struct UnkSentenceSphereMovement_b67a2 {
    void* vtable;
    u8 pad_0x8[0x134 - 0x8];
    f32 field_0x134;
    f32 field_0x138;
    f32 field_0x13c;
    f32 field_0x140;
    f32 field_0x144;
    f32 field_0x148;
    f32 field_0x14c;
};
static_assert(sizeof(UnkSentenceSphereMovement_b67a2) == 0x150, "layout");

void fn_710045928c_b67a2(UnkSentenceSphereMovement_b67a2* self) asm("_Z13fn_710045928cv");
void fn_710045928c_b67a2(UnkSentenceSphereMovement_b67a2* self) {
    __builtin_memset(self, 0, sizeof(*self));
    fn_7100ac14f8(reinterpret_cast<UnkComponent_0x138*>(self));
    self->vtable = reinterpret_cast<char*>(g_unk_7101a37420) + 0x10;
    self->field_0x134 = 90.0f;
    self->field_0x138 = 1.57079637f;
    self->field_0x13c = 30.0f;
    self->field_0x140 = 90.0f;
    self->field_0x144 = 200.0f;
    self->field_0x148 = 1.0f;
    self->field_0x14c = 4.0f;
    fn_7100023854(reinterpret_cast<UnkNode*>(self));
}
