// CScorpiusAIComponent unit: entering a state. The first state starts its timer and picks the blend for
// the current mode; others, in phase 2, fire an attack once the owner is in range; the second state clears
// a flag when the target is behind the owner. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "game/CBossAIComponent.h"
#include "game/CCharClassLifeComponent.h"
#include "game/UnkActor.h"
#include "types.h"

struct UnkScorpiusState_l697ce0 {
    u8 pad_0x0[0x58];
};

// The tunables block as this function reads it.
struct UnkScorpiusTunables_l697ce0 {
    u8 pad_0x0[0x1d8];
    f32 duration;  // 0x1d8
};

// What the owner's component at +0x1b8 is read for.
struct UnkScorpiusMovement_l697ce0 {
    u8 pad_0x0[0x54];
    f32 distance;  // 0x54
};

struct UnkScorpiusAI_l697ce0 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x1108];
    s32 phase;  // 0x1140
    u8 pad_0x1144[0x1391c];
    UnkScorpiusState_l697ce0 start_state;  // 0x14a60
    UnkScorpiusState_l697ce0 turn_state;   // 0x14ab8
    u8 pad_0x14b10[0x11c];
    f32 blend;  // 0x14c2c
    s32 mode;   // 0x14c30
    u8 pad_0x14c34[0x14];
    u8 field_0x14c48[0x70];
    u32 field_0x14cb8;
    f32 timer;  // 0x14cbc
    bool field_0x14cc0;
    u8 pad_0x14cc1[3];
    f32 field_0x14cc4;
    f32 field_0x14cc8;
    u8 pad_0x14ccc[8];
    u32 field_0x14cd4;
    u8 pad_0x14cd8[0xac];
    s32 field_0x14d84;
    s32 attack_count;  // 0x14d88
};
static_assert(__builtin_offsetof(UnkScorpiusAI_l697ce0, blend) == 0x14c2c, "blend");
static_assert(__builtin_offsetof(UnkScorpiusAI_l697ce0, field_0x14cd4) == 0x14cd4, "field_0x14cd4");
static_assert(__builtin_offsetof(UnkScorpiusAI_l697ce0, attack_count) == 0x14d88, "attack_count");

struct UnkVec3_l697ce0 {
    f32 x, y, z;
    UnkVec3_l697ce0(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
};

inline UnkVec3_l697ce0 operator-(const UnkVec3_l697ce0& a, const UnkVec3_l697ce0& b) {
    return UnkVec3_l697ce0(a.x - b.x, a.y - b.y, a.z - b.z);
}

inline f32 Dot(const UnkVec3_l697ce0& a, const UnkVec3_l697ce0& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

// The translation column of an actor's transform.
inline UnkVec3_l697ce0 Position(const UnkActor* actor) {
    return UnkVec3_l697ce0(actor->field_0x44, actor->field_0x54, actor->field_0x64);
}

struct UnkScorpiusAI_l692a98;
void fn_7100692a98(UnkScorpiusAI_l692a98* self, f32 amount);
void fn_71006920f8(UnkScorpiusAI_l697ce0* self, s32 id, s32* mode, u8* out, bool flag);
void fn_71006935a4(UnkScorpiusAI_l697ce0* self, s32 id, bool flag);

void fn_7100697ce0(UnkScorpiusAI_l697ce0* self, UnkScorpiusState_l697ce0* state) {
    fn_71003ad414(self);
    if (state == &self->start_state) {
        f32 duration = reinterpret_cast<const UnkScorpiusTunables_l697ce0&>(g_unk_717272fd38).duration;
        self->timer = duration;
        fn_7100692a98(reinterpret_cast<UnkScorpiusAI_l692a98*>(self), self->timer / duration);
        self->field_0x14cc0 = false;
        if (self->mode != 2)
            self->field_0x14cb8 = 0;
        fn_71006920f8(self, 2, &self->mode, self->field_0x14c48, false);
        f32 blend = self->mode == 3 ? 1.0f : self->mode == 2 ? 0.5f : 0.0f;
        self->blend = blend <= 0.0f ? 0.0f : blend;
        self->attack_count++;
    } else if (state != &self->turn_state && self->phase == 2) {
        f32 distance = reinterpret_cast<UnkScorpiusMovement_l697ce0*>(self->owner->field_0x1b8)->distance;
        bool attack = false;
        if (self->attack_count == 1) {
            if (self->field_0x14cc8 > 250.0f)
                attack = distance <= 750.0f;
            else
                attack = distance <= 1900.0f;
        } else if (self->attack_count == 2) {
            if (self->field_0x14cc4 >= 400.0f) {
                if (self->field_0x14cc8 > 250.0f)
                    attack = distance <= 650.0f && distance >= 300.0f;
                else
                    attack = distance <= 750.0f;
            }
        }
        if (attack)
            fn_71006935a4(self, 1, true);
    }

    if (state == &self->turn_state) {
        UnkActor* target = fn_7100d7c990(self->owner);
        UnkVec3_l697ce0 to_target = Position(target) - Position(self->owner);
        const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->owner->field_0x38));
        UnkVec3_l697ce0 forward(m.m[2], m.m[6], m.m[10]);
        if (Dot(to_target, forward) < 0.0f)
            self->field_0x14cd4 = 0;
    } else if (state != &self->start_state && self->phase == 2) {
        self->field_0x14d84++;
    }
}
