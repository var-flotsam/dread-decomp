// CDoorLifeComponent's unit (tools/tu_map.py unit 1376; the name is a hint): fn_71008d6f48 switches the door's cue
// off, clears its seven flags when the cue asks for it, and plays the owner's animation for its state: the id
// table's slot 0xb10 while the door is shut, else slot 0xb08 (or 0xb10 with the game manager's flag) followed by
// the owner's key g_unk_710164b0b0 and fn_71008d07f8, unless one of them plays already. Then it registers the
// owner in its list (+0x848) and notifies the unit's listeners (g_unk_7172731438 +0x100) with the door.
// Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/UnkCloneHelpers.h"
#include "base/global/CStrId.h"
#include "game/CInputComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkWeakTarget;
struct UnkDoor_b0251;
void fn_71008d1944(UnkDoor_b0251* self, bool on);

struct UnkClone_fn_710082d0ac;
extern template void base::refl_DeferredInvoke<UnkClone_fn_710082d0ac, u64>(
    base::UnkDeferredSet<base::UnkDeferredCallback<u64>>*, const u64*);

// The unit's static object: listeners told about each door at +0x100.
struct UnkStruct_7172731438 {
    u8 pad_0x0[0x100];
    base::UnkDeferredSet<base::UnkDeferredCallback<u64>> field_0x100;
};
extern UnkStruct_7172731438 g_unk_7172731438;
extern const u64 g_unk_710164b0b0;

// The game manager's flag at +0x2441 (inside the shared layout's padding).
struct UnkGameManagerFlag_l8d6f48 {
    u8 pad_0x0[0x2441];
    bool field_0x2441;
};

struct UnkDoorCue_l8d6f48 {
    u8 pad_0x0[0x128];
    bool field_0x128;
    u8 pad_0x129[0x12d - 0x129];
    bool field_0x12d;
};

struct UnkDoorOwner_l8d6f48 {
    u8 pad_0x0[0x1a8];
    void* field_0x1a8;
    bool field_0x1b0;
    u8 pad_0x1b1[3];
    bool field_0x1b4;
    u8 pad_0x1b5[0x220 - 0x1b5];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkDoorLife_l8d6f48 {
    u8 pad_0x0[0x30];
    UnkDoorOwner_l8d6f48* owner;  // 0x30
    u8 pad_0x38[0x62c - 0x38];
    bool field_0x62c;
    bool field_0x62d;
    bool field_0x62e;
    bool field_0x62f;
    bool field_0x630;
    bool field_0x631;
    bool field_0x632;
    u8 pad_0x633[0x848 - 0x633];
    u8 field_0x848[0x10];  // a list of owners (fn_7100874460)
    u8 pad_0x858[0x9f8 - 0x858];
    UnkDoorCue_l8d6f48* cue;  // 0x9f8
};
static_assert(__builtin_offsetof(UnkDoorLife_l8d6f48, field_0x848) == 0x848, "layout");
static_assert(__builtin_offsetof(UnkDoorLife_l8d6f48, cue) == 0x9f8, "layout");

void fn_71008d07f8(UnkDoorLife_l8d6f48* self);

// Whether the animation plays the id.
static inline bool IsPlaying(UnkAnimationComponent* animation, const base::global::CStrId& id) {
    return fn_710117ae38(animation, id.data->Key(), false);
}

using base::reflection::UnkReflTypeIdArg;

// A type id argument holding `id`.
static inline UnkReflTypeIdArg TypeIdArg(u32 id) {
    UnkReflTypeIdArg arg;
    arg.id = id;
    return arg;
}

// Sends the animation request: the id's hash under mode `mode`, with the all-bits mask.
static inline void Send(UnkAnimationComponent* animation, u64 hash, UnkReflTypeIdArg&& mode, UnkReflTypeIdArg&& mask) {
    fn_7101176690(animation, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
}

void fn_71008d6f48(UnkDoorLife_l8d6f48* self) {
    fn_71008d1944(reinterpret_cast<UnkDoor_b0251*>(self), false);
    if (self->cue->field_0x128) {
        self->field_0x62c = false;
        self->field_0x62d = false;
        self->field_0x62e = false;
        self->field_0x62f = false;
        self->field_0x630 = false;
        self->field_0x631 = false;
        self->field_0x632 = false;
    }
    UnkDoorOwner_l8d6f48* owner = self->owner;
    if (UnkAnimationComponent* animation = owner->animation) {
        if (!owner->field_0x1b4 || (owner->field_0x1b0 && !owner->field_0x1a8)) {
            if (!IsPlaying(animation, GameManagerIdTable().field_0xb10))
                Send(self->owner->animation, GameManagerIdTable().field_0xb10.data->Key(), TypeIdArg(0),
                     TypeIdArg(UnkAll_SpitSend().value));
        } else if (!IsPlaying(animation, GameManagerIdTable().field_0xb08) &&
                   !IsPlaying(self->owner->animation, GameManagerIdTable().field_0xb10)) {
            if (reinterpret_cast<UnkGameManagerFlag_l8d6f48*>(g_unk_7101d47310)->field_0x2441 &&
                self->cue->field_0x12d)
                Send(self->owner->animation, GameManagerIdTable().field_0xb10.data->Key(), TypeIdArg(0),
                     TypeIdArg(UnkAll_SpitSend().value));
            else
                Send(self->owner->animation, GameManagerIdTable().field_0xb08.data->Key(), TypeIdArg(0),
                     TypeIdArg(UnkAll_SpitSend().value));
            fn_7100d7c828(reinterpret_cast<UnkActor*>(self->owner), &g_unk_710164b0b0, false);
            fn_71008d07f8(self);
        }
    }
    fn_7100874460(self->field_0x848, reinterpret_cast<UnkWeakTarget**>(&self->owner));
    u64 door = reinterpret_cast<u64>(self);
    base::refl_DeferredInvoke<UnkClone_fn_710082d0ac, u64>(&g_unk_7172731438.field_0x100, &door);
}
