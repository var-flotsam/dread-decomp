// CChozoCommanderHypersparkAttack's unit (tools/tu_map.py). fn_710042dba0: the attack's direction,
// straight up in modes 1 and 3 (+0x58), else the third column of the owner's (+0x8) matrix.
// fn_710042dbfc: the length of the owner's timeline 0xa6ff23059a36aeca (fn_7101176d34) as the
// motion fn_71007142e8 sets up reports it over the whole timeline (fn_7100b8b3bc), at +0x70, as
// fn_7100991e30 (src/game/CSamusFloorSlideMovementState/CParams/CParams_Slide_ovb0004.cpp) does.
// fn_710042c9e0: whether the target (fn_7100d7c990) is on the side the owner faces, its position led
// by its velocity or, while the player's ability is in states 3 to 6, offset by its reach.
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "game/CAbilityGhostAura.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1728.h"
#include "game/UnkTU2523.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkStruct_7101176ea8;

struct UnkFlags_71007142e8 {
    u32 bits;
    UnkFlags_71007142e8() : bits(0) {}
    UnkFlags_71007142e8(u32 b) : bits(b) {}
    UnkFlags_71007142e8(const UnkFlags_71007142e8& o) : bits(o.bits) {}
};

inline const UnkFlags_71007142e8& UnkFlagsNone_71007142e8() {
    static UnkFlags_71007142e8 s_none;
    return s_none;
}

inline const UnkFlags_71007142e8& UnkFlagsAll_71007142e8() {
    static UnkFlags_71007142e8 s_all(~UnkFlagsNone_71007142e8().bits & 7);
    return s_all;
}

// The motion fn_71007142e8 initialises: the time it ends on at +0x2c.
struct UnkStruct_71007142e8 {
    u8 pad_0x0[0x2c];
    f32 field_0x2c;
    u8 pad_0x30[0x40];
};

struct UnkMotionTracks;
struct UnkTimeline {
    u8 pad_0x0[0x24];
    s32 frame_count;
    u8 pad_0x28[0x58];
    UnkMotionTracks* tracks;  // 0x80
};

void* fn_7101176d34(UnkStruct_7101176ea8* self, void* arg);
void fn_71007142e8(UnkStruct_71007142e8* self);

struct UnkHypersparkOwner_ovb0023 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x220 - 0x94];
    UnkStruct_7101176ea8* field_0x220;
};

struct UnkHypersparkAttack_ovb0023 {
    u8 pad_0x0[0x8];
    UnkHypersparkOwner_ovb0023* owner;  // 0x8
    u8 pad_0x10[0x58 - 0x10];
    s32 field_0x58;
    u8 pad_0x5c[0x70 - 0x5c];
    f32 field_0x70;
};

void fn_710042dba0(UnkHypersparkAttack_ovb0023* self, UnkVec3f* out) {
    if ((self->field_0x58 | 2) == 3) {
        *out = UnkVec3f(0.0f, 1.0f, 0.0f);
    } else {
        const UnkMatrix44& m = self->owner->xform.GetMatrix();
        *out = UnkVec3f(m.m[2], m.m[6], m.m[10]);
    }
}

void fn_710042dbfc(UnkHypersparkAttack_ovb0023* self) {
    self->field_0x70 = 0.0f;
    UnkStruct_7101176ea8* actions = self->owner->field_0x220;
    if (!actions)
        return;
    UnkTimeline* timeline =
        static_cast<UnkTimeline*>(fn_7101176d34(actions, reinterpret_cast<void*>(0xa6ff23059a36aecaull)));
    if (!timeline)
        return;
    UnkStruct_71007142e8 motion;
    fn_71007142e8(&motion);
    UnkMotionTracks* tracks = timeline->tracks;
    UnkFlags_71007142e8 flags = UnkFlagsAll_71007142e8();
    fn_7100b8b3bc(timeline, &motion, tracks, &flags.bits, 0.0f, timeline->frame_count);
    self->field_0x70 = __builtin_fabsf(motion.field_0x2c);
}

struct UnkKraidCreatureVec {
    f32 x;
    f32 y;
    f32 z;
    UnkKraidCreatureVec(const UnkKraidCreatureVec& other) : x(other.x), y(other.y), z(other.z) {}
};

struct UnkAbility;
UnkAbility* fn_7100df554c(UnkGameManager* manager);

struct UnkHypersparkTargetFlags_ovb0023 {
    u8 pad_0x0[0xf9];
    bool field_0xf9;
};

struct UnkHypersparkAbilitySub_ovb0023 {
    u8 pad_0x0[0x10];
    u32 state;
};

struct UnkHypersparkAbility_ovb0023 {
    u8 pad_0x0[0x20];
    UnkHypersparkAbilitySub_ovb0023* sub;  // 0x20
    u8 pad_0x28[0xb4 - 0x28];
    u32 state;  // 0xb4
    u8 pad_0xb8[0x438 - 0xb8];
    s32 field_0x438;
    u8 pad_0x43c[0x46c - 0x43c];
    f32 field_0x46c;
};

struct UnkHypersparkAttack3_ovb0023 {
    u8 pad_0x0[0x8];
    UnkActor* owner;  // 0x8
};

bool fn_710042bb38(UnkHypersparkAttack3_ovb0023* self);

static inline f32 Sign(f32 v) {
    return static_cast<f32>((v > 0.0f) - (v < 0.0f));
}

static inline bool IsActive(UnkHypersparkAbility_ovb0023* ability) {
    u32 state = ability->state;
    if (state == 7) {
        if (!ability->sub)
            return true;
        state = ability->sub->state;
    }
    return state >= 3;
}

bool fn_710042c9e0(UnkHypersparkAttack3_ovb0023* self) {
    if (!fn_710042bb38(self))
        return false;
    UnkActor* target = fn_7100d7c990(self->owner);
    if (!target)
        return true;
    UnkHypersparkTargetFlags_ovb0023* flags = static_cast<UnkHypersparkTargetFlags_ovb0023*>(target->field_0x208);
    if (!flags || !flags->field_0xf9)
        return true;
    f32 x = target->field_0x44 + fn_7100d7be08(target).x * 0.2f;
    UnkHypersparkAbility_ovb0023* ability =
        reinterpret_cast<UnkHypersparkAbility_ovb0023*>(fn_7100df554c(g_unk_7101d47310));
    if (ability && fn_7100309568(reinterpret_cast<UnkAbility*>(ability)) - 3 < 4) {
        f32 side = Sign(fn_710002614c(reinterpret_cast<const UnkMatrix44*>(target->field_0x38)).m[2]);
        if (ability->field_0x438 != 1)
            side = -side;
        f32 base = target->field_0x44;
        x = base + side * (IsActive(ability) ? ability->field_0x46c : 0.0f);
    }
    UnkActor* owner = self->owner;
    x -= owner->field_0x44;
    return Sign(fn_710002614c(reinterpret_cast<const UnkMatrix44*>(owner->field_0x38)).m[2]) == Sign(x);
}
