// The CEmmyProtoAIComponent unit: zeroes three speeds (fn_7101103aa8 on the component, slot 0x98 of
// the owner's +0x298 and +0x218); when the owner's +0x218 is in mode 4 (fn_71010cc3a4), sends a fixed
// request with mode 0 to the owner's sub-object (+0x220) between fn_71009746ac(+0x17a0 of it, 1) and
// (.., 0); descriptors as UnkSpitSend (src/game_s2/UnkSpitSend.h). Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/CEnemyMovement.h"
#include "game/UnkTU2702.h"
#include "types.h"

struct UnkEmmyCaveAIStart_b49;
void fn_71009746ac(void* self, bool flag);

struct UnkEmmyProtoSpeed_b57a3 {
    void** vtable;
    void vf_0x98(f32 v) { reinterpret_cast<void (*)(UnkEmmyProtoSpeed_b57a3*, f32)>(vtable[0x98 / 8])(this, v); }
};

struct UnkEmmyProtoOwner_b57a3 {
    u8 pad_0x0[0x218];
    UnkEmmyProtoSpeed_b57a3* field_0x218;
    void* field_0x220;
    u8 pad_0x228[0x70];
    UnkEmmyProtoSpeed_b57a3* field_0x298;
};

struct UnkEmmyProtoAI_b57a3 {
    u8 pad_0x0[0x30];
    UnkEmmyProtoOwner_b57a3* owner;
};

void fn_71004fccc0(UnkEmmyProtoAI_b57a3* self) {
    fn_7101103aa8(reinterpret_cast<UnkEmmyCaveAIStart_b49*>(self), 0.0f);
    self->owner->field_0x298->vf_0x98(0.0f);
    self->owner->field_0x218->vf_0x98(0.0f);
    UnkEmmyProtoSpeed_b57a3* speed = self->owner->field_0x218;
    if (fn_71010cc3a4(speed) != 4)
        return;
    char* sub = reinterpret_cast<char*>(speed) + 0x17a0;
    fn_71009746ac(sub, true);
    UnkSpitSend(self->owner->field_0x220, 0xdb3dc0b554887a7f, 0xf4061ff25a2163d6);
    fn_71009746ac(sub, false);
}
