// CObsydomithonAttack's unit (tools/tu_map.py unit 945): picks the id of the attack's animation from
// what the owner's character class (+0x220) is playing: one of the unit's ids while a known track
// plays, the stage's own id once the attack has a stage, or the empty id. Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStrIdBase;

extern CStrId g_unk_71726f8de8 __attribute__((visibility("hidden")));
extern CStrId g_unk_71726f8df0 __attribute__((visibility("hidden")));
extern CStrId g_unk_71726f8df8 __attribute__((visibility("hidden")));
extern CStrId g_unk_71726f8e00 __attribute__((visibility("hidden")));
extern CStrId g_unk_71726f8e18 __attribute__((visibility("hidden")));

struct UnkObsydomithonOwner_l6299e4 {
    u8 pad_0x0[0x1f0];
    void* field_0x1f0;
    u8 pad_0x1f8[0x28];
    UnkAnimationComponent* anim;  // 0x220
};

struct UnkObsydomithonAttack_l6299e4 {
    void* vtable;
    UnkObsydomithonOwner_l6299e4* owner;  // 0x8
    u8 pad_0x10[0x40];
    s32 stage;  // 0x50
};

static inline UnkActor* Actor(UnkObsydomithonOwner_l6299e4* owner) {
    return reinterpret_cast<UnkActor*>(owner);
}

// NON_MATCHING(functional): Control flow differ from the original; behaves the same on every input tools/equiv_test.py --any-kind tried, with full coverage. Owes a match.
const UnkStrIdBase* fn_71006299e4(UnkObsydomithonAttack_l6299e4* self) {
    if (!self->owner->anim || !fn_7100d7c990(Actor(self->owner)) || !self->owner->field_0x1f0)
        return &g_unk_7101d470e0;
    // The target is looked up once more; the original does not use it.
    fn_7100d7c990(Actor(self->owner));
    bool first = fn_710117ae38(self->owner->anim, 0x6596032ae6e891b5, false) ||
                 fn_710117ae38(self->owner->anim, 0xdeb7707676ca4f15, false);
    const UnkStrIdBase* id = &g_unk_7101d470e0;
    if (fn_710117ae38(self->owner->anim, g_unk_71726f8de8.data->Key(), false) ||
        fn_710117ae38(self->owner->anim, g_unk_71726f8e18.data->Key(), false))
        id = &g_unk_71726f8df8;
    if (first)
        id = &g_unk_71726f8df0;
    if (self->stage != 0)
        id = &g_unk_71726f8e00;
    return id;
}
