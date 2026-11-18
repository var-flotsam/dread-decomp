// behaviortree::CSetAnimationForImpact unit (tools/tu_map.py unit 451): fn_710036e020 updates the node: 6 until it
// is started; 2 once the owner's animation on the usual track is the node's and has not finished, or while the
// owner is frozen when that is asked for; 5 otherwise. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/CAnimationComponent.h"
#include "game/CFrozenComponent.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

struct UnkImpactOwner_l36e020 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0xa0];
    void* frozen;  // 0x2c8
};

struct UnkSetAnimationForImpact_l36e020 {
    u8 pad_0x0[0x18];
    UnkImpactOwner_l36e020* owner;  // 0x18
    u8 pad_0x20[0x20];
    CStrId animation_name;  // 0x40
    bool wait_animation;    // 0x48
    bool wait_frozen;       // 0x49
    bool started;           // 0x4a
};

// The pooled string of an id, counting the read as UnkStrIdBase::Text does.
static inline const CRntString& Str(const CStrId& id) {
    ++id.data->reads;
    return id.data->String();
}

u32 fn_710036e020(UnkSetAnimationForImpact_l36e020* self) {
    if (!self->started)
        return 6;
    if (!self->wait_animation)
        return 5;
    UnkImpactOwner_l36e020* owner = self->owner;
    if (owner && owner->animation) {
        UnkAnimationComponent* animation = owner->animation;
        const u64 track = 0xf4061ff25a2163d6;
        const CRntString& current = Str(*fn_7101177ae8(animation, track));
        const CRntString& wanted = Str(self->animation_name);
        if (current.key == wanted.key && !fn_71011782ec(animation, track))
            return 2;
    }
    if (!self->wait_frozen || !self->owner)
        return 5;
    void* frozen = self->owner->frozen;
    if (!frozen)
        return 5;
    return fn_71007f9f20(frozen) ? 2 : 5;
}
