// CAreaFXComponent unit. fn_71007ff7bc: an update step; unless the owner's object at +0x218 is in
// state 2 (fn_71010cc3a4), when the position at +0x428 is set (+0x424) and differs from the owner's
// position (the translation column of its transform at +0x38, within 0.001 per axis), it flags the
// owner's object at +0x240 (+0x16e6) and moves the owner there (vtable slot 0xd0), then runs
// fn_7100802108 and fn_7100802244. Names and layouts are placeholders.
#include "game/CAreaFXComponent.h"
#include "game/CEnemyMovement.h"
#include "game/UnkFlagHolder_af41.h"
#include "game/UnkOwner_af41.h"
#include "game/UnkSelf_af41.h"
#include "types.h"

void fn_7100802244(UnkSelf_af41* self);

static inline bool Near_af41(f32 a, f32 b) {
    f32 lo = a - 0.001f;
    f32 hi = a + 0.001f;
    return lo <= b && hi >= b;
}

void fn_71007ff7bc(UnkSelf_af41* self) {
    if (void* obj = self->owner->field_0x218)
        if (fn_71010cc3a4(obj) == 2)
            return;
    if (self->has_pos) {
        UnkOwner_af41* owner = self->owner;
        if (!(Near_af41(owner->x, self->pos[0]) && Near_af41(owner->y, self->pos[1]) &&
              Near_af41(owner->z, self->pos[2]))) {
            if (UnkFlagHolder_af41* holder = owner->field_0x240)
                holder->field_0x16e6 = 1;
            self->owner->vf_0xd0(self->pos, true);
        }
    }
    fn_7100802108(self);
    fn_7100802244(self);
}
