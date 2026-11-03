// CGooplotAIComponent (tools/tu_map.py): the constructor. The base AI component's constructor
// (fn_710036932c, as in src/game/CInfesterBallAIComponent/CInfesterBallAIComponent_Ctor_b44.cpp), the
// class's vtable, then its own state from +0x146c8: flags and counters cleared, three members
// constructed (fn_7100594308 at +0x146e0, fn_7100596858 at +0x14768, fn_71005999c0 at +0x14800 with
// 10, 0, 0x13, 0) and the rest cleared. Names and layout are placeholders.
#include "types.h"

extern void* g_unk_7101a4d0c8[];  // the component's vtable

struct UnkComponent;
void fn_710036932c(UnkComponent* self);
void fn_7100594308(void* self);
void fn_7100596858(void* self);
void fn_71005999c0(void* self, u32 a, u32 b, u32 c, u64 d);

typedef u64 UnkU64Align4_b49 __attribute__((aligned(4)));
typedef u16 UnkU16Align1_b49 __attribute__((aligned(1)));

// The state at +0x146c8 (offsets relative to it): the scalar fields, written through a pointer to it.
struct UnkGooplotCtorState_b49 {
    u8 field_0x0;
    u8 pad_0x1[3];
    UnkU64Align4_b49 field_0x4;
    u32 field_0xc;
    u8 pad_0x10;
    UnkU16Align1_b49 field_0x11;
    u8 pad_0x13[0x11d];
    u32 field_0x130;
    u8 pad_0x134[0x74];
    u16 field_0x1a8;
    u8 pad_0x1aa[0x2a];
    UnkU64Align4_b49 field_0x1d4;
    u32 field_0x1dc;
    u8 pad_0x1e0[4];
    u32 field_0x1e4;
};

// The members, addressed from the component.
struct UnkGooplotAICtor_b49 {
    void** vtable;
    u8 pad_0x8[0x146e0 - 0x8];
    u8 field_0x146e0[0x88];
    u8 field_0x14768[0x98];
    u8 field_0x14800[0x72];
    u8 field_0x14872[0x20];
    u16 field_0x14892;
};

void fn_71005998fc(UnkGooplotAICtor_b49* self) {
    UnkGooplotCtorState_b49* state =
        reinterpret_cast<UnkGooplotCtorState_b49*>(reinterpret_cast<u8*>(self) + 0x146c8);
    fn_710036932c(reinterpret_cast<UnkComponent*>(self));
    self->vtable = g_unk_7101a4d0c8 + 2;
    state->field_0x11 = 1;
    state->field_0x0 = 0;
    state->field_0x4 = 0;
    state->field_0xc = 0;
    fn_7100594308(self->field_0x146e0);
    fn_7100596858(self->field_0x14768);
    state->field_0x130 = 0;
    fn_71005999c0(self->field_0x14800, 10, 0, 0x13, 0);
    state->field_0x1a8 = 0x100;
    u8* tail = self->field_0x14872;
    state->field_0x1d4 = 0;
    state->field_0x1dc = 0;
    state->field_0x1e4 = 0;
    *reinterpret_cast<u16*>(tail + 0x20) = 0;
    __builtin_memset(tail, 0, 0x20);
}
