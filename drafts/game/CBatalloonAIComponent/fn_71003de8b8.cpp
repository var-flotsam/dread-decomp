// CBatalloonAIComponent's unit (tools/tu_map.py unit 583): answers an animation event. Events 6 and 12 clear
// the sync state; event 11 sends a request to the animation and, on the first one that is taken, gives
// every other Batalloon of the spawn group a random delay and marks it synced. Names and layouts are placeholders.
#include <stdlib.h>

#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAnimationComponent.h"
#include "game/CBatalloonAIComponent.h"
#include "game/UnkTU2702.h"
#include "game/UnkTU365.h"
#include "types.h"

using base::reflection::CType;

struct UnkNavComponent;
struct UnkNavGroupInfo;

struct UnkSwifterMember_ovb0114;
struct UnkSwifterActor_ovb0114;

struct UnkBatalloonAI_l3de8b8;

// The tunables: the base delay (negative: the animation's own length), the range the delay scales over,
// and the random part's bounds.
struct UnkBatalloonTunables_l3de8b8 {
    u8 pad_0x0[0x518];
    f32 delay;     // 0x518
    f32 range;     // 0x51c
    f32 randMin;   // 0x520
    f32 randMax;   // 0x524
};

// An animation entry: its length at +0x28.
struct UnkBatalloonAnimEntry_l3de8b8 {
    u8 pad_0x0[0x28];
    f32 length;
};

struct UnkBatalloonActor_l3de8b8 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x1b8];
    UnkTakumakuComponent* animation;  // 0x220
    u8 pad_0x228[0x18];
    UnkBatalloonAI_l3de8b8* ai;  // 0x240
};

struct UnkBatalloonAI_l3de8b8 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkBatalloonActor_l3de8b8* owner;  // 0x30
    u8 pad_0x38[0x1598];
    u8 field_0x15d0;
    u8 pad_0x15d1[0x130ff];
    UnkBatalloonTunables_l3de8b8* tunables;  // 0x146d0
    u8 pad_0x146d8;
    u8 synced;  // 0x146d9
    u8 pad_0x146da[2];
    f32 delay;  // 0x146dc
    u8 field_0x146e0;
};
static_assert(__builtin_offsetof(UnkBatalloonAI_l3de8b8, field_0x15d0) == 0x15d0, "");
static_assert(__builtin_offsetof(UnkBatalloonAI_l3de8b8, tunables) == 0x146d0, "");
static_assert(__builtin_offsetof(UnkBatalloonAI_l3de8b8, field_0x146e0) == 0x146e0, "");

struct UnkBatalloonHandle_l3de8b8 {
    u8 pad_0x0[0x10];
    UnkBatalloonActor_l3de8b8* actor;  // +0x10
};

struct UnkBatalloonMemberActor_l3de8b8 {
    u8 pad_0x0[0x40];
    UnkBatalloonHandle_l3de8b8* data;  // +0x40
};

struct UnkBatalloonMemberHandle_l3de8b8 {
    u8 pad_0x0[0x10];
    UnkBatalloonMemberActor_l3de8b8* actor;  // +0x10
};

// A spawned member (0x40 bytes).
struct UnkBatalloonMember_l3dd8b0 {
    u8 pad_0x0[0x40];
};

struct UnkBatalloonGroup_l3dd8b0 {
    u8 pad_0x0[0x68];
    UnkBatalloonMember_l3dd8b0* members;  // +0x68
    u32 memberCount;                    // +0x70
};

struct UnkBatalloonGroupSource_l3dd8b0 {
    u8 pad_0x0[0x60];
    u8 part[1];  // +0x60
};

struct UnkBatalloonGroupHandle_l3dd8b0 {
    u8 pad_0x0[0x10];
    void* target;  // +0x10
};

UnkBatalloonGroupSource_l3dd8b0* fn_71003ddc44(UnkNavGroupInfo* info);
UnkBatalloonGroup_l3dd8b0* fn_71003ddd20(void* target);

__attribute__((visibility("hidden"))) extern char g_unk_71726c0118[];

static inline bool IsBatalloonAI(UnkBatalloonAI_l3de8b8* ai) {
    const CType* type = reinterpret_cast<base::reflection::UnkReflObject*>(ai)->vf_0x0();
    fn_71003dd3e4();
    return fn_710009d50c(type, reinterpret_cast<const CType*>(g_unk_71726c0118));
}

// Whether the Batalloon is synced already or plays animation kind `kind`.
static inline bool IsSyncSource(UnkBatalloonAI_l3de8b8* ai, u32 kind) {
    UnkTakumakuComponent* animation = ai->owner->animation;
    if (ai->synced != 1 && animation)
        return fn_7101178e6c(animation, kind);
    return ai->synced == 1;
}

// Sends a request with mode 0 under the hash through the caller's two arguments; whether it was taken.
static inline bool Send(void* target, u64 hash, base::reflection::UnkReflTypeIdArg& mode,
                        base::reflection::UnkReflTypeIdArg& mask) {
    mode.id = 0;
    mask.id = UnkAll_SpitSend().value;
    return fn_7101176690(target, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
}

static inline f32 RandomRange(f32 min, f32 max) {
    f32 t = (f32)((rand() & 0x7fff) * (1.0 / 32767.0));
    return min + (max - min) * t;
}

static inline f32 DistanceSquared(const UnkBatalloonActor_l3de8b8* a, const UnkBatalloonActor_l3de8b8* b) {
    f32 dx = a->x - b->x;
    f32 dy = a->y - b->y;
    f32 dz = a->z - b->z;
    return dx * dx + dy * dy + dz * dz;
}

void fn_71003de8b8(UnkBatalloonAI_l3de8b8* self, s32 event) {
    const u64 hash = 0xd17932dd15716684;
    UnkTakumakuComponent* animation;
    switch (event) {
    case 11:
        if (self->field_0x15d0)
            return;
        // Declared in the switch body, ahead of the next case label: no lifetime markers, slots of their own.
        base::reflection::UnkReflTypeIdArg mode1;
        base::reflection::UnkReflTypeIdArg mask1;
        base::reflection::UnkReflTypeIdArg mode2;
        base::reflection::UnkReflTypeIdArg mask2;
        animation = self->owner->animation;
        if (self->field_0x146e0) {
            if (animation)
                Send(animation, 0x23b66636138dfea5, mode1, mask1);
            return;
        }
        if (!animation || !self->tunables)
            return;
        if (!Send(animation, hash, mode2, mask2))
            return;
        break;
    case 12:
    case 6:
        self->synced = 0;
        self->delay = 0.0f;
        self->field_0x146e0 = 0;
        return;
    default:
        return;
    }

    UnkNavGroupInfo* info = fn_710110c3d8(reinterpret_cast<UnkNavComponent*>(self));
    if (!info)
        return;
    UnkBatalloonGroupSource_l3dd8b0* source = fn_71003ddc44(info);
    if (!source)
        return;
    UnkBatalloonGroupHandle_l3dd8b0* handle = reinterpret_cast<UnkBatalloonGroupHandle_l3dd8b0*>(
        *fn_71002a76a4(reinterpret_cast<UnkBreakablePart*>(source->part)));
    if (!handle || !handle->target)
        return;
    UnkBatalloonGroup_l3dd8b0* group = fn_71003ddd20(handle->target);
    if (!group)
        return;

    for (UnkBatalloonMember_l3dd8b0 *it = group->members, *end = it + group->memberCount; it != end; ++it) {
        UnkBatalloonMemberHandle_l3de8b8* member = reinterpret_cast<UnkBatalloonMemberHandle_l3de8b8*>(
            *fn_71003df2d0(reinterpret_cast<UnkSwifterMember_ovb0114*>(it)));
        if (!member || !member->actor || !member->actor->data || !member->actor->data->actor)
            continue;
        UnkBatalloonActor_l3de8b8* actor = member->actor->data->actor;
        UnkBatalloonAI_l3de8b8* component = actor->ai;
        if (!component)
            continue;
        UnkBatalloonAI_l3de8b8* other = IsBatalloonAI(component) ? component : nullptr;
        if (other == self || !other)
            continue;
        if (IsSyncSource(other, 11))
            continue;

        f32 delay = self->tunables->delay;
        if (delay < -0.001f) {
            UnkBatalloonAnimEntry_l3de8b8* entry = static_cast<UnkBatalloonAnimEntry_l3de8b8*>(
                fn_7101176d34(reinterpret_cast<UnkStruct_7101176ea8*>(animation), reinterpret_cast<void*>(hash)));
            delay = entry ? entry->length : 0.0f;
        }
        UnkBatalloonTunables_l3de8b8* tunables = self->tunables;
        f32 range = tunables->range;
        if (range > 0.001f) {
            f32 range2 = range * range;
            f32 dist2 = DistanceSquared(self->owner, actor);
            delay = delay * (dist2 > range2 ? range2 : dist2) / range2;
        }
        other->delay = delay + RandomRange(tunables->randMin, tunables->randMax);
        if (other->delay > 0.001f) {
            other->synced = 1;
            other->field_0x146e0 = 1;
        }
    }
}
