// The constructor of CCooldownXBossFireBallMovementComponent (vtable 0x7101a3f7f8) over the
// movement base constructor fn_7100ac2c5c: clears 0x5d8..0x5e5 and the pointers at +0x5e8 and
// +0x5f0, then finishes in fn_7100023854. Names and layouts are placeholders.
#include "types.h"

// Declared as src/game/animtree/CLayerNode/fn_71002e4894.cpp does.
struct UnkNode;
void fn_7100023854(UnkNode*);
struct UnkFireBallMovementBase;
void fn_7100ac2c5c(void* self);
extern void* g_unk_7101a3f7f8[];

struct UnkFireBallMovement {
    void** vtable;
    u8 pad_0x8[0x5d0];
    u8 field_0x5d8[0xe];
    u8 pad_0x5e6[0x2];
    void* field_0x5e8;
    void* field_0x5f0;
};

void fn_71004d7d2c(UnkFireBallMovement* self) {
    fn_7100ac2c5c(self);
    self->vtable = g_unk_7101a3f7f8 + 2;
    __builtin_memset(self->field_0x5d8, 0, sizeof(self->field_0x5d8));
    self->field_0x5e8 = nullptr;
    self->field_0x5f0 = nullptr;
    fn_7100023854(reinterpret_cast<UnkNode*>(self));
}
