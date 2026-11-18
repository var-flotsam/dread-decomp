// CDredhedDiveAttack's unit: steps the dive's state machine (state at +0x30). The dive state (6) checks
// that the owner's Dredhed AI is in its dive mode and turns the dive toward the target when the owner
// does not face it. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "types.h"

struct UnkDredhedSend_b57;

struct UnkDredhedAIMode {
    void** vtable;
    const base::reflection::CClass* vf_0x0() {
        return reinterpret_cast<const base::reflection::CClass* (*)(UnkDredhedAIMode*)>(vtable[0])(this);
    }
    u8 pad_0x8[0x5f8];
    s32 field_0x600;
};

struct UnkDredhedAI {
    u8 pad_0x0[0x3f];
    u8 field_0x3f;
    u8 pad_0x40[0x11de];
    u8 field_0x121e;
    u8 pad_0x121f[0x3f1];
    UnkDredhedAIMode* field_0x1610;
};

struct UnkDredhedDiveAttackUpdate {
    void** vtable;
    UnkActor* owner;
    u8 pad_0x10[0x20];
    u32 state;
    u8 pad_0x34;
    u8 field_0x35;
    u8 pad_0x36[2];
    f32 field_0x38;
};

void fn_71004e4a38(UnkDredhedDiveAttackUpdate* self, u32 state);
bool fn_71004e5d74(UnkDredhedDiveAttackUpdate* self);
void fn_71004e5c60(UnkDredhedSend_b57* self, u64 hash, const u32* value);

static inline f32 Sign(f32 x) { return (f32)((x > 0.0f) - (x < 0.0f)); }

// True when the owner faces the side its target is on.
static inline bool FacesTarget(UnkDredhedDiveAttackUpdate* self) {
    f32 forward = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->owner->field_0x38)).m[2];
    f32 side = Sign(fn_7100d7c990(self->owner)->field_0x44 - self->owner->field_0x44);
    f32 facing = Sign(forward);
    return side - 0.1f <= facing && side + 0.1f >= facing;
}

static inline void Send(UnkDredhedDiveAttackUpdate* self, u64 hash, const u32* value) {
    fn_71004e5c60(reinterpret_cast<UnkDredhedSend_b57*>(self), hash, value);
}

void fn_71004e5e6c(UnkDredhedDiveAttackUpdate* self) {
    UnkDredhedAI* ai = static_cast<UnkDredhedAI*>(self->owner->field_0x240);
    switch (self->state) {
    case 0:
        fn_71004e4a38(self, 1);
        return;
    case 1:
        // The message arguments are declared in the switch's own scope, each with its own slot.
        alignas(8) u32 start_arg;
        if (self->field_0x38 > 0.0f) {
            self->state = 2;
            start_arg = 0;
            Send(self, 0xf3b302494c5be093, &start_arg);
            return;
        }
        fn_71004e4a38(self, 3);
        return;
    case 3:
        fn_71004e4a38(self, 4);
        return;
    case 6:
        alignas(8) u32 dive_arg;
        alignas(8) u32 turn_arg;
        alignas(8) u32 late_turn_arg;
        UnkDredhedAIMode* mode;
        mode = ai->field_0x1610;
        if (mode && fn_710009d50c(mode->vf_0x0(), fn_71004e6770()) && mode->field_0x600 == 1) {
            if (!FacesTarget(self) &&
                !(self->field_0x35 &&
                  __builtin_fabsf(fn_7100d7c990(self->owner)->field_0x44 - self->owner->field_0x44) < 400.0f)) {
                if (fn_71004e5d74(self)) {
                    dive_arg = 0;
                    Send(self, 0x0277c95e4bfb8114, &dive_arg);
                } else {
                    turn_arg = 0;
                    Send(self, 0x7153a28eb176ac96, &turn_arg);
                }
                self->state = 7;
                return;
            }
            if (!self->field_0x35 && !FacesTarget(self)) {
                late_turn_arg = 0;
                Send(self, 0x7153a28eb176ac96, &late_turn_arg);
                self->state = 7;
                return;
            }
        }
        fn_71004e4a38(self, 7);
        return;
    case 7:
        if (!ai->field_0x121e) {
            fn_71004e4a38(self, 8);
            return;
        }
        // fall through: like the waiting state, done once the AI's flag at +0x3f is set
    case 5:
        fn_71004e4a38(self, ai->field_0x3f ? 0 : 8);
        return;
    default:
        return;
    }
}
