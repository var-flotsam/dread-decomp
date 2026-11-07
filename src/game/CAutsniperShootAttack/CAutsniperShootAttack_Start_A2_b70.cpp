// CAutsniperShootAttack's unit (the name from reflection): the attack starts when CAttack's test passes and
// the state of the owner's (+0x8) object at +0x218 is not 2; then the character class at +0x220, if any,
// stops layer 6 and plays layer 0xb. Names and layouts are placeholders.
#include "types.h"
#include "game/CAttack.h"

struct UnkSelf;
u32 fn_71010cc3a4(void*);
struct UnkActorTarget;
void fn_7101178e08(UnkActorTarget*, u32, u32, u32, f32);
void fn_710117662c(UnkActorTarget*, u32, u32, u32, u32, f32);

struct UnkAutsniperOwner_A2_b70 {
    u8 pad_0x0[0x218];
    void* field_0x218;
    UnkActorTarget* field_0x220;
};

struct UnkAutsniperAttack_A2_b70 {
    u8 pad_0x0[0x8];
    UnkAutsniperOwner_A2_b70* owner;
};

// `arg` and `mode` (x1, w2) reach the base check untouched in the original (tools/forwarded_args.py).
bool fn_71003d3958(UnkAutsniperAttack_A2_b70* self, void* arg, u32 mode) {
    if (!fn_7100360b84(reinterpret_cast<CAttack*>(self), arg, mode) ||
        fn_71010cc3a4(self->owner->field_0x218) == 2)
        return false;
    if (UnkActorTarget* anim = self->owner->field_0x220) {
        fn_7101178e08(anim, 6, 0, 0, -1.0f);
        fn_710117662c(anim, 0xb, 0, 1, 1, -1.0f);
    }
    return true;
}
