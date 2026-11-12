// CMeleeComponent (unit 1421): switching the melee trail effects on the owner's world. The names
// come from the playing track's record when one matches the class of fn_7100d23b98, else from the
// unit's "meleetrail" names (statics of CMeleeComponent_TUInit.cpp). Names and layouts are
// placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/CFXComponent.h"
#include "game/UnkAnimationComponent.h"

struct UnkWorld;

struct UnkMeleeName {
    u8 pad_0x0[0x30];
    u64 hash;
};

// Two name ids (pointers to their entries): the trail and its dash variant.
struct UnkMeleeNamePair {
    UnkMeleeName* trail;
    UnkMeleeName* dash;
};

extern UnkMeleeNamePair g_unk_71727385c8 __attribute__((visibility("hidden")));
extern UnkMeleeNamePair g_unk_71727385d8 __attribute__((visibility("hidden")));

struct UnkMeleeOwner {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;
    u8 pad_0x228[0x58];
    UnkWorld* world;  // 0x280
};

struct UnkMeleeComponent {
    u8 pad_0x0[0x30];
    UnkMeleeOwner* owner;
};

void fn_7100929298(UnkMeleeComponent* self, u32 mode) {
    UnkWorld* world = self->owner->world;
    if (!world)
        return;
    UnkTrackQuery query;
    query.cls = fn_7100d23b98();
    query.result = nullptr;
    query.value = -1.0f;
    fn_710117b08c(self->owner->animation, &query);
    UnkMeleeNamePair* names;
    if (query.result) {
        names = reinterpret_cast<UnkMeleeNamePair*>(reinterpret_cast<char*>(query.result) + 0x18);
    } else {
        names = &g_unk_71727385c8;
        fn_710114fe8c(world, names->dash->hash, mode == 1, 1);
    }
    u32 end = mode == 2;
    fn_710114fe8c(world, names->trail->hash, mode == 1, 1);
    fn_710114fe8c(world, g_unk_71727385d8.dash->hash, end, 1);
    fn_710114fe8c(world, g_unk_71727385d8.trail->hash, end, 1);
}
