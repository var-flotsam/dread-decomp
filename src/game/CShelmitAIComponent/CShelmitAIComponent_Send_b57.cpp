// The CShelmitAIComponent unit. fn_71006bb7bc runs the CAIComponent step fn_710110ed88, then by the
// state at +0x14780: in state 0 it runs fn_71006ba194 and fn_71006ba4fc; in state 1 it sends the
// owner's (+0x30) sub-object at +0x220 hash 0xfb70eec0a98665b3 under the usual track kind with mode 1
// (descriptors as UnkSpitSend, src/game_s2/UnkSpitSend.h). Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/CShelmitAIComponent.h"
#include "types.h"

struct UnkShelmitAI;
void fn_710110ed88(void* self);

struct UnkShelmitOwner_b57a4 {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkShelmitAI_b57a4 {
    u8 pad_0x0[0x30];
    UnkShelmitOwner_b57a4* owner;
    u8 pad_0x38[0x14780 - 0x38];
    u32 field_0x14780;
};

void fn_71006bb7bc(UnkShelmitAI_b57a4* self) {
    fn_710110ed88(self);
    u32 state = self->field_0x14780;
    if (state == 1) {
        if (self->owner) {
            if (void* target = self->owner->field_0x220) {
                base::reflection::UnkReflTypeIdArg mode;
                mode.id = 1;
                base::reflection::UnkReflTypeIdArg mask;
                mask.id = UnkAll_SpitSend().value;
                fn_7101176690(target, 0xfb70eec0a98665b3, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958,
                              &g_unk_7172769740, &mask);
            }
        }
    } else if (state == 0) {
        fn_71006ba194(reinterpret_cast<UnkShelmitAI*>(self));
        fn_71006ba4fc(self);
    }
}
