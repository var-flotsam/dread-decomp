// Unit 1108 of tools/tu_map.py (no class name). fn_71006eeb8c picks an id for an actor: the unit's
// own id (g_unk_717270c320) while the actor's animation component (+0x220) plays a
// CCharClassUseWalkTurnTrack, else the entry at +0x23a8 of the id table (fn_7100031894). Names and
// layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManagerIdTable.h"
#include "types.h"

extern char g_unk_717270c320[] __attribute__((visibility("hidden")));

struct UnkActor_b50_1108 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;
};

const void* fn_71006eeb8c(void* self, UnkActor_b50_1108* actor) {
    if (UnkAnimationComponent* anim = actor->animation) {
        if (fn_710117b348(anim, fn_7100c644e8()))
            return g_unk_717270c320;
    }
    return &GameManagerIdTable().field_0x23a8;
}
