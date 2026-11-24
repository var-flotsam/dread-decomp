// Constructor of a component: runs the base constructor, then constructs seventeen members in place
// (each by its own constructor) and sets a few defaults (four pairs of tuning floats and two flags).
//
// The two flag bytes at +0x4fc and +0x4fd (one u16 member of the base) are addressed by offset.
#include "game/CDamageSourceFactor.h"
#include "game/UnkBossAIComponent.h"
#include "types.h"

extern void* g_unk_7101a369a8[];

// The members after the base, constructed in place by their own constructors.
struct UnkComponent_44e048 : UnkBossAIComponent {
    u8 field_0x528[0x100];  // 0x528
    u8 field_0x628[0x148];  // 0x628
    u8 field_0x770[0x100];  // 0x770
    u8 field_0x870[0x118];  // 0x870
    u8 field_0x988[0x100];  // 0x988
    u8 field_0xa88[0x110];  // 0xa88
    u8 field_0xb98[0x108];  // 0xb98
    u8 field_0xca0[0xf0];  // 0xca0
    u8 field_0xd90[0x118];  // 0xd90
    u8 field_0xea8[0xf0];  // 0xea8
    u8 field_0xf98[0x108];  // 0xf98
    u8 field_0x10a0[0x108];  // 0x10a0
    u8 field_0x11a8[0x110];  // 0x11a8
    u8 field_0x12b8[0x108];  // 0x12b8
    u8 field_0x13c0[0xf0];  // 0x13c0
    u8 field_0x14b0[0xf0];  // 0x14b0
    f32 field_0x15a0;
    f32 field_0x15a4;
    f32 field_0x15a8;
    f32 field_0x15ac;
    f32 field_0x15b0;
    f32 field_0x15b4;
    f32 field_0x15b8;
    f32 field_0x15bc;
    u8 field_0x15c0[0x70];  // 0x15c0
    u8 field_0x1630[0x70];  // 0x1630: the same kind as +0x15c0 (fn_71008c2da8)
};

void fn_7100422d7c(void* member);
void fn_7100420fa4(void* member);
void fn_71004228f8(void* member);
void fn_7100422200(void* member);
void fn_710041d07c(void* member);
void fn_71004202c8(void* member);
void fn_7100421740(void* member);
void fn_710041c440(void* member);
void fn_7100423b2c(void* member);
void fn_710041e310(void* member);
void fn_710041fb50(void* member);
void fn_710041e990(void* member);
void fn_7100424690(void* member);
void fn_710041f538(void* member);
void fn_710042315c(void* member);
void fn_71004206fc(void* member);

template <class T>
static inline T& At(void* base, u64 offset) {
    return *reinterpret_cast<T*>(reinterpret_cast<char*>(base) + offset);
}

void fn_710044e048(UnkComponent_44e048* self) {
    fn_710041a198(self);
    self->vtable = g_unk_7101a369a8 + 2;
    fn_7100422d7c(self->field_0x528);
    fn_7100420fa4(self->field_0x628);
    fn_71004228f8(self->field_0x770);
    fn_7100422200(self->field_0x870);
    fn_710041d07c(self->field_0x988);
    fn_71004202c8(self->field_0xa88);
    fn_7100421740(self->field_0xb98);
    fn_710041c440(self->field_0xca0);
    fn_7100423b2c(self->field_0xd90);
    fn_710041e310(self->field_0xea8);
    fn_710041fb50(self->field_0xf98);
    fn_710041e990(self->field_0x10a0);
    fn_7100424690(self->field_0x11a8);
    fn_710041f538(self->field_0x12b8);
    fn_710042315c(self->field_0x13c0);
    fn_71004206fc(self->field_0x14b0);
    self->field_0x15a0 = 1000.0f;
    self->field_0x15a4 = 500.0f;
    self->field_0x15a8 = 1000.0f;
    self->field_0x15ac = 100.0f;
    self->field_0x15b0 = 20.0f;
    self->field_0x15b4 = 0.6f;
    self->field_0x15b8 = 250.0f;
    self->field_0x15bc = 200.0f;
    fn_71008c2da8(self->field_0x15c0);
    fn_71008c2da8(self->field_0x1630);
    self->field_0x409 = 0;
    At<u8>(self, 0x4fc) = 1;
    At<u8>(self, 0x4fd) = 1;
}
