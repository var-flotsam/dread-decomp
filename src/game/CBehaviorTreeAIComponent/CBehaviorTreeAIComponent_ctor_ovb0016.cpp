// CBehaviorTreeAIComponent unit (tools/tu_map.py, the name from reflection): the constructor body of
// fn_710036a3e4 (src/game/CBehaviorTreeAIComponent/CBehaviorTreeAIComponent_Ctor_A2_b76.cpp) without the
// class-entry pass: the base constructor (fn_71010f813c), the vtable, and the 7 bytes at +0x146ac and the 16
// at +0x146b8 cleared. Names and layouts are placeholders.
#include "game/CBehaviorTreeAIComponent.h"
#include "game/UnkTU2702.h"
#include "types.h"

struct UnkBtAI_A2_b76;

struct UnkBtAI_ovb0016 {
    void* vtable;
    u8 pad_0x8[0x146ac - 0x8];
    u8 block_0x146ac[7];
    u8 pad_0x146b3[0x146b8 - 0x146b3];
    u8 block_0x146b8[0x10];
};

void fn_710036932c(UnkBtAI_ovb0016* self) {
    fn_71010f813c(reinterpret_cast<UnkBtAI_A2_b76*>(self));
    self->vtable = reinterpret_cast<char*>(g_unk_7101a24230) + 0x10;
    __builtin_memset(self->block_0x146ac, 0, sizeof(self->block_0x146ac));
    __builtin_memset(self->block_0x146b8, 0, sizeof(self->block_0x146b8));
}
