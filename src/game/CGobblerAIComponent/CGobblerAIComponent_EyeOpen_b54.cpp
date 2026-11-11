// CGobblerAIComponent (tools/tu_map.py): whether a source is one of the two gobbler sources (+0x20 is one of
// the ids at g_unk_71726e5120) while the animation (+0x220 of the owner at +0x30) plays a
// CCharClassGobblerEyeOpenTrack (fn_7100c20848). Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/CGobblerAIComponent.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkGobblerSource_b54 {
    u8 pad_0x0[0x20];
    u64 id;
};

struct UnkGobblerOwner_b54 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;
};

struct UnkGobblerAI_b54 {
    u8 pad_0x0[0x30];
    UnkGobblerOwner_b54* owner;
};

bool fn_71005824cc(UnkGobblerAI_b54* self, UnkGobblerSource_b54* source) {
    if (!source || (source->id != g_unk_71726e5120[0] && source->id != g_unk_71726e5120[1]))
        return false;
    UnkAnimationComponent* animation = self->owner->animation;
    if (!animation)
        return false;
    return fn_710117b348(animation, fn_7100c20848());
}
