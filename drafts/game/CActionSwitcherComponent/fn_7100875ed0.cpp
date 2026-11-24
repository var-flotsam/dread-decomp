// CActionSwitcherComponent's unit (tools/tu_map.py, unit 1328): turns the switcher on or off. Stores the
// state (+0x88), gives the owner's weak target the "on" or "off" id and sends the owner's animation (+0x220)
// the definition's on or off action with mode 0; when notifying, the animation is first flagged (+0x2c8)
// if the owner has the object at +0x1f0, and the other descriptor goes with the request.
// Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "game/CNavMeshItemComponent.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU372.h"
#include "types.h"

struct UnkWeakTarget;

struct UnkSwitcherAction_l875ed0 {
    u8 pad_0x0[0x30];
    u64 hash;  // 0x30
};

struct UnkSwitcherDef_l875ed0 {
    u8 pad_0x0[0x58];
    UnkSwitcherAction_l875ed0* on_action;   // 0x58
    UnkSwitcherAction_l875ed0* off_action;  // 0x60
};

struct UnkSwitcherAnimation_l875ed0 {
    u8 pad_0x0[0x2c8];
    u32 field_0x2c8;
};

struct UnkSwitcherOwner_l875ed0 {
    u8 pad_0x0[0x1f0];
    void* field_0x1f0;
    u8 pad_0x1f8[0x28];
    UnkSwitcherAnimation_l875ed0* animation;  // 0x220
};
static_assert(__builtin_offsetof(UnkSwitcherOwner_l875ed0, animation) == 0x220, "layout");

struct UnkSwitcher_l875ed0 {
    u8 pad_0x0[0x30];
    UnkSwitcherOwner_l875ed0* owner;  // 0x30
    u8 pad_0x38[0x48];
    UnkSwitcherDef_l875ed0* def;  // 0x80
    bool on;                      // 0x88
};
static_assert(__builtin_offsetof(UnkSwitcher_l875ed0, def) == 0x80, "layout");

void fn_7100875ed0(UnkSwitcher_l875ed0* self, bool on, bool notify) {
    base::reflection::UnkReflTypeIdArg notify_mode;
    base::reflection::UnkReflTypeIdArg notify_mask;
    base::reflection::UnkReflTypeIdArg mode;
    base::reflection::UnkReflTypeIdArg mask;
    self->on = on;
    UnkSwitcherAction_l875ed0* const& action = on ? self->def->on_action : self->def->off_action;
    if (void* target = fn_71002b3b84(reinterpret_cast<UnkWeakTarget*>(self->owner)))
        fn_71009c0f1c(target, self->on ? 0xb15b31b835c17820ull : 0x13ae275a1bcd6558ull);
    const u64 kind = 0xf4061ff25a2163d6ull;
    if (notify) {
        if (self->owner->field_0x1f0)
            self->owner->animation->field_0x2c8 = 1;
        notify_mode.id = 0;
        notify_mask.id = UnkAll_SpitSend().value;
        CAnimationComponent_SendActionToTrack(self->owner->animation, action->hash, kind, &notify_mode, &g_unk_710164b958,
                      &g_unk_717276974c, &notify_mask);
    } else {
        mode.id = 0;
        mask.id = UnkAll_SpitSend().value;
        CAnimationComponent_SendActionToTrack(self->owner->animation, action->hash, kind, &mode, &g_unk_710164b958, &g_unk_7172769740,
                      &mask);
    }
}
