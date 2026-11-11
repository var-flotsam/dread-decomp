// CGobblerAIComponent (tools/tu_map.py): the damage handler. The base handler (fn_7101105e44) decides
// first; otherwise a hit without the flags 0x2008000 counts only when it comes from one of the two
// gobbler sources (+0x20 of its +0x60 is one of the ids at g_unk_71726e5120) while the animation
// (+0x220 of the owner at +0x30) plays a CGobbler* state (fn_7100c20848); else the answer is 2. A
// counted hit with flag 0x40 from such a source applies the owner's life factor (+0x54 of +0x1b8;
// fn_71008c1538). Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU2702.h"
#include "types.h"

struct UnkSluggerAI;
struct UnkSluggerEvent;
struct UnkRequest;
void fn_71008c1538(UnkRequest*, f32);
extern u64 g_unk_71726e5120[2] __attribute__((visibility("hidden")));  // the two gobbler source ids

struct UnkGobblerSource_b49 {
    u8 pad_0x0[0x20];
    u64 id;
};

struct UnkGobblerHit_b49 {
    u8 pad_0x0[0x60];
    UnkGobblerSource_b49* source;
    u8 pad_0x68[0x4c];
    u32 flags;

    bool FromGobbler() const {
        return source && (source->id == g_unk_71726e5120[0] || source->id == g_unk_71726e5120[1]);
    }
};

struct UnkGobblerLife_b49 {
    u8 pad_0x0[0x54];
    f32 factor;
};

struct UnkGobblerOwner_b49 {
    u8 pad_0x0[0x1b8];
    UnkGobblerLife_b49* life;
    u8 pad_0x1c0[0x60];
    UnkAnimationComponent* animation;
};

struct UnkGobblerAI_b49 {
    u8 pad_0x0[0x30];
    UnkGobblerOwner_b49* owner;

    bool InState() {
        UnkAnimationComponent* animation = owner->animation;
        return animation && fn_710117b348(animation, fn_7100c20848());
    }
};

u32 fn_71005823a4(UnkGobblerAI_b49* self, UnkGobblerHit_b49* hit) {
    if (u32 r = fn_7101105e44(reinterpret_cast<UnkSluggerAI*>(self), reinterpret_cast<UnkSluggerEvent*>(hit)))
        return r;
    if (!(hit->flags & 0x2008000)) {
        if (!hit->FromGobbler() || !self->InState())
            return 2;
    }
    if ((hit->flags & 0x40) && hit->FromGobbler() && self->InState())
        fn_71008c1538(reinterpret_cast<UnkRequest*>(hit), self->owner->life->factor);
    return 0;
}
