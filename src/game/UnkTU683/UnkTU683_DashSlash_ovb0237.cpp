// Unit 683 (CChozoRobotSoldierDashSlashAttack): the attack's state pick (the next state's id), the length of the
// attack motion's end offset, and its start. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/CAnimationComponent.h"
#include "game/CChozoRobotSoldierCannonShotAttack.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAIComponent.h"
#include "game/UnkTU1728.h"
#include "game/UnkTU2702.h"
#include "game/UnkTU681.h"
#include "game_s2/UnkParams.h"
#include "types.h"

struct UnkGoliathAI_b58a3;
struct UnkDashSlashOwner_b0237 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* field_0x220;
    u8 pad_0x228[0x18];
    UnkGoliathAI_b58a3* field_0x240;
};

struct UnkDashSlashAttack_b0237 {
    void** vtable;
    UnkDashSlashOwner_b0237* owner;  // 0x8
};

struct UnkGoliathAI_b58a3;

// Signature as the other AI units declare it; here it hands back the next state's id, so it is called through
// a cast.


extern u8 g_unk_71726cb118[];

struct UnkHydrogigaAttack_b47;
void fn_71003627d0(UnkHydrogigaAttack_b47* self);

u8* fn_710046840c(UnkDashSlashAttack_b0237* self) {
    if (UnkAnimationComponent* animation = self->owner->field_0x220) {
        if (fn_710117b348(animation, fn_7100bf7db8()))
            return g_unk_71726cbb98;
    }
    f32 limit = *reinterpret_cast<f32*>(g_unk_71726cb118 + 0x2c);
    if (UnkGoliathAI_b58a3* ai = self->owner->field_0x240) {
        f32 value = 0.0f;
        fn_71010fc68c(ai, &value);
        limit = (value < limit || limit < 0.0f) ? value : limit;
    }
    if (!(limit <= 200.0f))
        return reinterpret_cast<u8* (*)(UnkDashSlashAttack_b0237*)>(fn_71003627d0)(self);
    return g_unk_71726cbb88;
}

struct UnkStruct_7101176ea8;
struct UnkFlags_ovb0237 {
    u32 bits;
    UnkFlags_ovb0237() : bits(0) {}
    UnkFlags_ovb0237(u32 b) : bits(b) {}
    UnkFlags_ovb0237(const UnkFlags_ovb0237& o) : bits(o.bits) {}
};

inline const UnkFlags_ovb0237& UnkFlagsNone_ovb0237() {
    static UnkFlags_ovb0237 s_none asm("g_unk_71726a4340");
    return s_none;
}

inline const UnkFlags_ovb0237& UnkFlagsAll_ovb0237() {
    static UnkFlags_ovb0237 s_all asm("g_unk_71726a4330")(~UnkFlagsNone_ovb0237().bits & 7);
    return s_all;
}

struct UnkStruct_71007142e8 {
    f32 m[12];
    u8 pad_0x30[0x40];
};

struct UnkMotionTracks;
struct UnkTimeline {
    u8 pad_0x0[0x24];
    s32 frame_count;
    u8 pad_0x28[0x58];
    UnkMotionTracks* tracks;  // 0x80
};

void fn_71007142e8(UnkStruct_71007142e8* self);

struct UnkDashSlashOwner2_b0237 {
    u8 pad_0x0[0x220];
    UnkStruct_7101176ea8* actions;  // 0x220
};

struct UnkDashSlashAttack2_b0237 {
    u8 pad_0x0[8];
    UnkDashSlashOwner2_b0237* owner;
    u8 pad_0x10[0x2c];
    f32 field_0x3c;
};

void fn_71004684d0(UnkDashSlashAttack2_b0237* self) {
    UnkDashSlashOwner2_b0237* owner = self->owner;
    *reinterpret_cast<u32*>(&self->field_0x3c) = 0;
    UnkStruct_7101176ea8* actions = owner->actions;
    if (!actions)
        return;
    UnkTimeline* timeline = static_cast<UnkTimeline*>(fn_7101176e0c(actions, 0xfb7d4bc21bc9379aull, ~0ull));
    UnkStruct_71007142e8 motion;
    fn_71007142e8(&motion);
    UnkMotionTracks* tracks = timeline->tracks;
    UnkFlags_ovb0237 flags = UnkFlagsAll_ovb0237();
    fn_7100b8b3bc(timeline, &motion, tracks, &flags.bits, 0.0f, timeline->frame_count);
    f32 x = motion.m[3];
    f32 y = motion.m[7];
    f32 z = motion.m[11];
    self->field_0x3c = __builtin_sqrtf(x * x + y * y + z * z);
}

struct UnkAttackBase;
void fn_71003608ac(UnkAttackBase* self);

extern u8 g_unk_7101649788[12] __attribute__((visibility("hidden")));
extern u8 g_unk_7101649794[12] __attribute__((visibility("hidden")));

struct UnkStrIdData_b0237 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkDashSlashStart_b0237 {
    u8 pad_0x0[8];
    UnkActor* owner;  // 0x8
    u8 pad_0x10[0x20];
    f32 field_0x30[3];
    f32 field_0x3c;
    f32 field_0x40;
    bool field_0x44;
};

void fn_7100468638(UnkDashSlashStart_b0237* self) {
    fn_71003608ac(reinterpret_cast<UnkAttackBase*>(self));
    UnkActor* owner = self->owner;
    u32 x = *reinterpret_cast<u32*>(&owner->field_0x44);
    u32 y = *reinterpret_cast<u32*>(&owner->field_0x54);
    u32 z = *reinterpret_cast<u32*>(&owner->field_0x64);
    *reinterpret_cast<u32*>(&self->field_0x30[0]) = x;
    *reinterpret_cast<u32*>(&self->field_0x30[1]) = y;
    *reinterpret_cast<u32*>(&self->field_0x30[2]) = z;
    bool hit = false;
    if (UnkAnimationComponent* animation = static_cast<UnkAnimationComponent*>(self->owner->field_0x220)) {
        UnkStrIdData_b0237* id = *reinterpret_cast<UnkStrIdData_b0237**>(g_unk_71726cbb88);
        hit = fn_710117ae38(animation, id->hash, false);
    }
    f32* remaining;
    if (hit) {
        self->field_0x44 = true;
        remaining = &self->field_0x40;
        goto clear;
    }
    {
        self->field_0x44 = false;
        f32 limit = *reinterpret_cast<f32*>(g_unk_71726cb118 + 0x2c);
        if (UnkGoliathAI_b58a3* ai = static_cast<UnkGoliathAI_b58a3*>(self->owner->field_0x240)) {
            f32 value = 0.0f;
            fn_71010fc68c(ai, &value);
            limit = (value < limit || limit < 0.0f) ? value : limit;
        }
        f32 left = limit - self->field_0x3c;
        remaining = &self->field_0x40;
        *remaining = left;
        if (!(left < 0.0f))
            goto done;
    }
clear:
    *remaining = 0.0f;
done:
    UnkActor* target = fn_7100d7c990(self->owner);
    UnkAIComponent* ai = static_cast<UnkAIComponent*>(self->owner->field_0x240);
    if (!ai)
        return;
    UnkNavigation* navigation = fn_71010fccf8(ai);
    if (!target || !navigation)
        return;
    UnkParams params;
    params.field_0x30 = 0;
    __builtin_memset(params.field_0x20, 0, 16);
    params.field_0x34 = 5;
    params.vtable = g_unk_7101a25b10 + 2;
    params.field_0x39 = 0;
    params.field_0x3b = 0;
    params.field_0x3d = 0;
    params.field_0x18 = 0;
    params.field_0x1c = 1;
    f32 target_x = target->field_0x44;
    f32 owner_x = self->owner->field_0x44;
    const u8* direction = owner_x < target_x ? g_unk_7101649788 : g_unk_7101649794;
    u64 lo;
    u32 hi;
    __builtin_memcpy(&lo, direction, 8);
    __builtin_memcpy(&hi, direction + 8, 4);
    __builtin_memcpy(params.field_0x8, &lo, 8);
    __builtin_memcpy(params.field_0x8 + 8, &hi, 4);
    params.field_0x8[12] = 1;
    fn_71006f4b0c(navigation, &params, 1);
}
