// CCameraBoundaryCtrl (unit 1169, the name from reflection): the in-place constructor its reflection descriptor
// points to (fn_0x28). After the camera controller base constructor (fn_71010ed044) it sets the vtable, clears the
// fields from +0x1d8, sets the pair at +0x25c to (1, 0.01) and the flag at +0x264, then runs the class entries
// (fn_7100023854). Defined under the `void()` symbol src/reflection/CCameraBoundaryCtrl.cpp declares. Names and
// layouts are placeholders.
#include "types.h"

struct UnkNode;
void fn_7100023854(UnkNode* node);
struct UnkBase_7100763ed4;
void fn_71010ed044(UnkBase_7100763ed4*);
extern void* g_unk_7101a6e6e8[];

struct UnkCameraBoundaryCtrl_A5_b71 {
    void* vtable;
    u8 pad_0x8[0x1d8 - 0x8];
    u64 field_0x1d8[15];
    u8 pad_0x250[0x4];
    f32 field_0x254;
    f32 field_0x258;
    f32 field_0x25c;
    f32 field_0x260;
    u16 field_0x264;
    u8 pad_0x266[0x2];
    u64 field_0x268[5];
    u16 field_0x290;
    u8 pad_0x292[0x6];
    u64 field_0x298;
};
static_assert(__builtin_offsetof(UnkCameraBoundaryCtrl_A5_b71, field_0x298) == 0x298, "layout");

void fn_7100766ee8_A5_b71(UnkCameraBoundaryCtrl_A5_b71* self) asm("_Z13fn_7100766ee8v");
void fn_7100766ee8_A5_b71(UnkCameraBoundaryCtrl_A5_b71* self) {
    fn_71010ed044(reinterpret_cast<UnkBase_7100763ed4*>(self));
    self->vtable = reinterpret_cast<char*>(g_unk_7101a6e6e8) + 0x10;
    self->field_0x254 = 0.0f;
    self->field_0x258 = 0.0f;
    for (int i = 0; i < 15; ++i)
        self->field_0x1d8[i] = 0;
    self->field_0x25c = 1.0f;
    self->field_0x260 = 0.01f;
    self->field_0x264 = 1;
    self->field_0x298 = 0;
    self->field_0x290 = 0;
    for (int i = 4; i >= 0; --i)
        self->field_0x268[i] = 0;
    fn_7100023854(reinterpret_cast<UnkNode*>(self));
}
