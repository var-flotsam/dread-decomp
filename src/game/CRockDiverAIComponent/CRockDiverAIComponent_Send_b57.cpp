// The CRockDiverAIComponent unit. fn_7100663b90 runs the CAIComponent step fn_710110cca4; unless the flag
// at +0x15d0 is set, and when the owner (+0x30) has both its sub-object at +0x220 and the component at
// +0x1f0: in a track of the class of fn_7100d2ee28 the sub-object gets fn_710117ab64 with 1 and the
// hidden g_unk_71726fd808 and the component fn_710024fa0c with false; otherwise the sub-object is sent
// hash 0x7531238cb72381d8 under the usual track kind with mode 0 (UnkSpitSend,
// src/game_s2/UnkSpitSend.h). Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

void fn_710110cca4(void* self, void* arg);
struct UnkStruct_710024fa0c;
void fn_710024fa0c(UnkStruct_710024fa0c*, bool);
void fn_710117ab64(UnkAnimationComponent* self, u32 value, const void* arg);
extern char g_unk_71726fd808[] __attribute__((visibility("hidden")));

struct UnkRockDiverOwner_b57a4 {
    u8 pad_0x0[0x1f0];
    UnkStruct_710024fa0c* field_0x1f0;
    u8 pad_0x1f8[0x28];
    UnkAnimationComponent* field_0x220;
};

struct UnkRockDiverAI_b57a4 {
    u8 pad_0x0[0x30];
    UnkRockDiverOwner_b57a4* owner;
    u8 pad_0x38[0x15d0 - 0x38];
    bool field_0x15d0;
};

// `arg` (x1) reaches the base step untouched in the original (tools/forwarded_args.py).
void fn_7100663b90(UnkRockDiverAI_b57a4* self, void* arg) {
    fn_710110cca4(self, arg);
    if (self->field_0x15d0)
        return;
    UnkAnimationComponent* target = self->owner->field_0x220;
    UnkStruct_710024fa0c* comp = self->owner->field_0x1f0;
    if (!target || !comp)
        return;
    if (fn_710117b348(target, fn_7100d2ee28())) {
        fn_710117ab64(target, 1, g_unk_71726fd808);
        fn_710024fa0c(comp, false);
    } else {
        UnkSpitSend(target, 0x7531238cb72381d8, 0xf4061ff25a2163d6);
    }
}
