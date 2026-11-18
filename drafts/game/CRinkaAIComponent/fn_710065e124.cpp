// CRinkaAIComponent's unit (tools/tu_map.py unit 980): the damage handler. The base handler's answer wins;
// a hit with any of the ignored flags gets 0; otherwise 2 unless the owner is frozen and not thawing and
// the hit carries one of the 0x40800 flags. Names and layouts are placeholders.
#include "game/CFrozenComponent.h"
#include "game/UnkTU2702.h"
#include "types.h"

struct UnkRinkaEvent_l65e124 {
    u8 pad_0x0[0xb4];
    u32 flags;  // 0xb4
};

struct UnkRinkaOwner_l65e124 {
    u8 pad_0x0[0x2c8];
    UnkFrostbiteComponent* frozen;  // 0x2c8
};

struct UnkRinkaAI_l65e124 {
    u8 pad_0x0[0x30];
    UnkRinkaOwner_l65e124* owner;  // 0x30
};

u32 fn_710065e124(UnkRinkaAI_l65e124* self, UnkRinkaEvent_l65e124* event) {
    u32 result = fn_7101105e44(reinterpret_cast<UnkSluggerAI*>(self), reinterpret_cast<UnkSluggerEvent*>(event));
    if (result)
        return result;
    if (event->flags & 0x2048574)
        return 0;
    UnkFrostbiteComponent* frozen = self->owner->frozen;
    bool thawed = !frozen || !fn_71007f9f20(frozen) || fn_71007f9fd0(frozen);
    if (thawed || !(event->flags & 0x40800))
        return 2;
    return 0;
}
