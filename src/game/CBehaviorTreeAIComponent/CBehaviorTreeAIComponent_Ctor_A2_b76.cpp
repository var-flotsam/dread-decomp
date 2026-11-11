// CBehaviorTreeAIComponent unit (tools/tu_map.py, the name from reflection): the in-place constructor its
// reflection descriptor points to (fn_0x28). After the base constructor (fn_71010f813c) it sets the vtable,
// clears the 7 bytes at +0x146ac and the 16 at +0x146b8, then runs the class entries (fn_7100023854). Defined
// under the `void()` symbol src/reflection/CBehaviorTreeAIComponent.cpp declares (the b64a4 alias pattern, as
// src/game/CBatalloonAIComponent/CBatalloonAIComponent_Ctor_b67a2.cpp). Names and layouts are placeholders.
#include "base/core/CBaseObject.h"
#include "game/CBehaviorTreeAIComponent.h"
#include "game/UnkTU2702.h"
#include "types.h"

struct UnkNode;

struct UnkBtAI_A2_b76;

struct UnkBtAI_A2_b76 {
    void* vtable;
    u8 pad_0x8[0x146ac - 0x8];
    u8 block_0x146ac[7];
    u8 pad_0x146b3[0x146b8 - 0x146b3];
    u8 block_0x146b8[0x10];
};

void fn_710036a3e4_A2_b76(UnkBtAI_A2_b76* self) asm("_Z13fn_710036a3e4v");
void fn_710036a3e4_A2_b76(UnkBtAI_A2_b76* self) {
    fn_71010f813c(self);
    self->vtable = reinterpret_cast<char*>(g_unk_7101a24230) + 0x10;
    __builtin_memset(self->block_0x146ac, 0, sizeof(self->block_0x146ac));
    __builtin_memset(self->block_0x146b8, 0, sizeof(self->block_0x146b8));
    fn_7100023854(reinterpret_cast<UnkNode*>(self));
}
