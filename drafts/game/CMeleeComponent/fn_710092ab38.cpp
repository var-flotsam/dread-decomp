// CMeleeComponent (unit 1421): starts the melee action on the owner's character class. The action is
// the component's own id, else the one the owner's action source names, else one picked from the
// owner's movement state (morph ball, wall, facing); it is sent with mode 3 when it is forced or
// already playing, 1 otherwise. Names and layouts are placeholders.
#include <math.h>

#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CAnimationComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU1501.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU437.h"
#include "game/UnkTUInit.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

// The unit's data (its statics, merged into one block).
struct UnkMeleeTU {
    UnkTUHeader header;
    CStrId mb2;                    // 0x18
    CStrId climb2;                 // 0x20
    CFilePathStrId block_fx;       // 0x28
    CStrId melee_run;              // 0x30
    CStrId melee_mb_run;           // 0x38
    CStrId melee_trail;            // 0x40
    CStrId melee_trail_dash;       // 0x48
    CStrId melee_trail_end;        // 0x50
    CStrId melee_trail_end_dash;   // 0x58
    u8 field_0x60[0x290 - 0x60];
    UnkReflType type;              // 0x290
};

extern UnkMeleeTU g_unk_7172738588 __attribute__((visibility("hidden")));

// The aim tunables block: the slide angle range.
struct UnkAimTunable {
    u8 pad_0x0[0xa8];
    f32 field_0xa8;
    u8 pad_0xac[0xc];
    f32 field_0xb8;
};

// The game manager's state flags as read here.
struct UnkMeleeGameState {
    u8 pad_0x0[0xc4];
    bool field_0xc4;
    u8 pad_0xc5;
    bool field_0xc6;
    u8 pad_0xc7;
    s32 field_0xc8;
    u8 pad_0xcc[0x40];
    bool field_0x10c;
};

struct UnkMeleeFacing {
    u8 pad_0x0[0x9ec];
    f32 x;  // 0x9ec
    f32 y;  // 0x9f0
    f32 z;  // 0x9f4
};

struct UnkMeleeMovement {
    u8 pad_0x0[0xf9];
    bool field_0xf9;
    u8 pad_0xfa[0x10c - 0xfa];
    f32 field_0x10c;
    u8 pad_0x110[0x7e4 - 0x110];
    bool field_0x7e4;
};

struct UnkMeleeActionSource {
    virtual void vf_0x0();
    // vtable slot 0x3d0: the action id it names.
    const CStrId& vf_0x3d0() {
        return reinterpret_cast<const CStrId& (*)(UnkMeleeActionSource*)>((*reinterpret_cast<void***>(this))[0x3d0 / 8])(this);
    }
};

struct UnkMeleeAim {
    u8 pad_0x0[0x360];
    bool field_0x360;
};

struct UnkMeleeActionOwner {
    u8 pad_0x0[0x1d8];
    UnkMeleeFacing* facing;  // 0x1d8
    u8 pad_0x1e0[0x200 - 0x1e0];
    UnkMeleeActionSource* source;  // 0x200
    UnkMeleeMovement* movement;    // 0x208
    u8 pad_0x210[0x220 - 0x210];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x2b0 - 0x228];
    UnkMeleeAim* aim;  // 0x2b0
};

struct UnkMeleeComponent {
    u8 pad_0x0[0x30];
    UnkMeleeActionOwner* owner;
    u8 pad_0x38[0x9c - 0x38];
    f32 field_0x9c;
    u8 pad_0xa0[0x1e8 - 0xa0];
    bool has_action;  // 0x1e8
    CStrId action;    // 0x1f0
};

static inline bool IsEmpty(const CStrId& id) {
    return id.data == &g_unk_7101d47108->empty;
}

static inline bool HasTrack(UnkAnimationComponent* anim, const CStrId& name) {
    return CAnimationComponent_IsPlayingActionNameContaining(reinterpret_cast<UnkStruct_710117af10*>(anim),
                         reinterpret_cast<UnkCls_710117af10**>(const_cast<CStrId*>(&name)), false);
}

static inline bool NearZero(f32 v, f32 eps) {
    return v + eps >= 0.0f && v - eps <= 0.0f;
}

static inline bool IsBusy(const UnkMeleeGameState* state) {
    return (state->field_0xc8 != 0 || state->field_0x10c) && state->field_0xc4 && !state->field_0xc6;
}

static inline const UnkMeleeGameState* GameState() {
    return reinterpret_cast<const UnkMeleeGameState*>(g_unk_7101d47310);
}

// The table's ids this unit reads that its header does not name yet: 0x17a8, 0x17b8 and 0x1e50.
static inline const CStrId& IdAfter(const CStrId& id, int n) {
    return (&id)[n];
}

static const CStrId* PickAction(UnkMeleeComponent* self, UnkAnimationComponent* anim, UnkMeleeMovement* movement) {
    UnkMeleeTU& tu = g_unk_7172738588;
    if (!HasTrack(anim, tu.mb2) || HasTrack(anim, tu.climb2)) {
        if (movement->field_0xf9)
            return &GameManagerIdTable().field_0x17a0;
        if (fn_71009e12d0(movement))
            return &IdAfter(GameManagerIdTable().field_0x17a0, 1);
        if (UnkMeleeFacing* facing = self->owner->facing) {
            f32 angle;
            if (!NearZero(facing->z, 0.001f))
                angle = atan2f(facing->y, fabsf(facing->x));
            else
                angle = -3.14159274f;
            if (angle > g_unk_71726b1438.field_0xa8 || angle < g_unk_71726b1438.field_0xb8)
                return &GameManagerIdTable().field_0x1770;
        } else if (NearZero(fabsf(movement->field_0x10c), 0.1f)) {
            return &GameManagerIdTable().field_0x1770;
        }
        if (fn_710117b348(anim, fn_7100c4c958()) || self->owner->aim->field_0x360 ||
            IsOpticCamouflageAbilityInState6(g_unk_7101d47310) || IsBusy(GameState()))
            return &GameManagerIdTable().field_0x1770;
        return &tu.melee_run;
    }
    if (movement->field_0xf9)
        return &GameManagerIdTable().field_0x17a0;
    if (movement->field_0x7e4 || IsBusy(GameState()))
        return &GameManagerIdTable().field_0x1770;
    if (!HasTrack(anim, IdAfter(GameManagerIdTable().field_0x1e70, -4)))
        return &IdAfter(GameManagerIdTable().field_0x17a0, 3);
    return fn_710117b348(anim, fn_7100d14318()) ? &tu.melee_mb_run : &tu.melee_run;
}

// The action the component or its owner's action source names (the empty id when neither does).
static inline const CStrId& CurrentAction(UnkMeleeComponent* self) {
    if (!self->has_action) {
        UnkMeleeActionSource* source = self->owner->source;
        return source ? source->vf_0x3d0() : g_unk_7101d470e0;
    }
    return self->action;
}

static inline UnkMeleeMovement* Movement(UnkMeleeActionOwner* owner) {
    return owner ? owner->movement : nullptr;
}

bool fn_710092ab38(UnkMeleeComponent* self) {
    UnkAnimationComponent* anim = self->owner->animation;
    if (!anim)
        return false;

    const CStrId* action = &CurrentAction(self);
    UnkMeleeMovement* movement = Movement(self->owner);
    if (movement && IsEmpty(*action))
        action = PickAction(self, anim, movement);
    if (IsEmpty(*action))
        return false;

    u32 mode;
    if (fn_710117b348(anim, fn_7100cfb318()) || self->field_0x9c == -3.40282347e+38f ||
        fn_710117ae38(anim, action->data->Key(), false))
        mode = 3;
    else
        mode = 1;
    return base::reflection::refl_EmitU32(anim, action->data->Key(), 0xf4061ff25a2163d6, mode, &g_unk_710164b958,
                                          &g_unk_7172769740);
}
