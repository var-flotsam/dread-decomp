// CGunComponent unit (tools/tu_map.py unit 1304): plays the animation request that `value` describes on the
// owner's animation. With the owner's movement at hand, asks it for the ledges to the left and right of the
// four animation names `value` holds, picks the animation of the near one (the far name past a threshold) and
// scales the movement by how much of 64 units its motion covers. Names and layouts are placeholders.
#include <math.h>

#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "game/CAnimationComponent.h"
#include "game/UnkMotionRule.h"
#include "game/UnkTU1141.h"
#include "game/UnkTU1728.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU363.h"
#include "types.h"

using base::global::CStrId;

struct UnkGunComponent_ovb0091;

struct UnkStruct_71007142e8 {
    f32 m[12];
    u8 pad_0x30[0x40];
};

struct UnkTimeline {
    u8 pad_0x0[0x24];
    s32 frame_count;
    u8 pad_0x28[0x58];
    UnkMotionTracks* tracks;  // 0x80
};

struct UnkMsg_b152 {
    s16 id;
    u8 field_0x2;
    u8 pad_0x3[9];
    f32 field_0xc;
};

struct UnkAnimTime_l83dd48 {
    u64 a, b;
};
struct UnkAnimBlend_l83dd48 {
    u32 a, b, c;
};

// An animation request record (as fn_7101176ea8 takes it).
struct UnkAnimRequest_l83dd48 {
    s64 id;
    UnkAnimTime_l83dd48 time;    // 0x8
    UnkAnimBlend_l83dd48 blend;  // 0x18
    u32 flags;                   // 0x24
    u32 mask;                    // 0x28

    UnkAnimRequest_l83dd48(s64 id_, UnkAnimTime_l83dd48 time_, UnkAnimBlend_l83dd48 blend_, u32 flags_, u32 mask_)
        : id(id_), time(time_), blend(blend_), flags(flags_), mask(mask_) {}
};

// What the request is read from: its id, and four animation names (left near and far, right near and far).
struct UnkGunAnimSource_l83dd48 {
    virtual void vf_0x0(); virtual void vf_0x8(); virtual void vf_0x10();
    virtual void Fill(UnkAnimRequest_l83dd48* request);  // 0x18
    u8 pad_0x8[0x10];
    CStrId id;  // 0x18
    u8 pad_0x20[0x18];
    CStrId left_near;   // 0x38
    CStrId left_far;    // 0x40
    CStrId right_near;  // 0x48
    CStrId right_far;   // 0x50
};

// The owner's movement: finds the ledges on either side (vf_0x2e8).
struct UnkGunMovement_l83dd48 {
    void** vtable;
    u8 pad_0x8[0x5250];
    f32 scale;  // 0x5258

    void FindLedges(UnkMsg_b152* right, bool* right_flag, UnkMsg_b152* left, bool* left_flag) {
        using Fn = void (*)(UnkGunMovement_l83dd48*, UnkMsg_b152*, bool*, UnkMsg_b152*, bool*);
        reinterpret_cast<Fn>(vtable[0x2e8 / 8])(this, right, right_flag, left, left_flag);
    }
};
static_assert(__builtin_offsetof(UnkGunMovement_l83dd48, scale) == 0x5258, "layout");

struct UnkGunActor_l83dd48 {
    u8 pad_0x0[0x208];
    UnkGunMovement_l83dd48* movement;  // 0x208
    u8 pad_0x210[0x10];
    UnkStruct_7101176ea8* animation;  // 0x220
};

struct UnkGunWeapon_l83dd48 {
    void** vtable;

    bool PlaysRequest() {
        using Fn = bool (*)(UnkGunWeapon_l83dd48*);
        return reinterpret_cast<Fn>(vtable[0x1c0 / 8])(this);
    }
};

struct UnkGunComponent_l83dd48 {
    u8 pad_0x0[0x30];
    UnkGunActor_l83dd48* actor;  // 0x30
    u8 pad_0x38[0xe0];
    UnkGunWeapon_l83dd48* alt;   // 0x118
    UnkGunWeapon_l83dd48* main;  // 0x120
    bool use_alt;                // 0x128
};

static inline bool IsEmpty(const CStrId& id) {
    return &g_unk_7101d47108->empty == id.data;
}

// The distance from which the far animation is used.
struct UnkLedgeDistance_l83dd48 {
    f32 value;
    UnkLedgeDistance_l83dd48() { value = 48.0f; }
};

static const UnkLedgeDistance_l83dd48& FarDistance() {
    static UnkLedgeDistance_l83dd48 s_distance asm("g_unk_717272627c");
    return s_distance;
}

static inline bool IsFound(const UnkMsg_b152& ledge) {
    return ledge.id != -1 && ledge.field_0x2 != 0xff;
}

// The height the motion of the timeline gains over its whole duration.
static inline f32 Height(UnkTimeline* timeline) {
    UnkStruct_71007142e8 motion;
    fn_71007142e8(&motion);
    UnkMotionTracks* tracks = timeline->tracks;
    UnkFlags_71007142e8 flags = UnkFlagsAll_71007142e8();
    fn_7100b8b3bc(timeline, &motion, tracks, &flags.bits, 0.0f, timeline->frame_count);
    return fabsf(motion.m[11]);
}

void fn_710083dd48(UnkGunComponent_ovb0091* gun, void* value) {
    UnkGunComponent_l83dd48* self = reinterpret_cast<UnkGunComponent_l83dd48*>(gun);
    UnkGunAnimSource_l83dd48* source = static_cast<UnkGunAnimSource_l83dd48*>(value);
    if (!source || IsEmpty(source->id))
        return;
    if (!self->actor || !self->actor->animation)
        return;

    UnkAnimRequest_l83dd48 request(-1, reinterpret_cast<const UnkAnimTime_l83dd48&>(g_unk_710164b958),
                                   reinterpret_cast<const UnkAnimBlend_l83dd48&>(g_unk_7172769740), 0,
                                   UnkAll_SpitSend().value);
    source->Fill(&request);
    request.flags |= 2;

    bool any = !(IsEmpty(source->right_near) && IsEmpty(source->left_near) && IsEmpty(source->right_far) &&
                 IsEmpty(source->left_far));
    UnkGunMovement_l83dd48* movement;
    if (any && (movement = self->actor->movement)) {
        bool left = !IsEmpty(source->left_near) || !IsEmpty(source->left_far);
        bool right = !IsEmpty(source->right_near) || !IsEmpty(source->right_far);
        UnkMsg_b152 right_ledge;
        fn_71002878dc(&right_ledge);
        UnkMsg_b152 left_ledge;
        fn_71002878dc(&left_ledge);
        movement->FindLedges(right ? &right_ledge : nullptr, nullptr, left ? &left_ledge : nullptr, nullptr);

        const UnkLedgeDistance_l83dd48& far = FarDistance();
        f32 distance = 0.0f;
        if (IsFound(left_ledge) && IsFound(right_ledge) && left_ledge.field_0xc < 128.0f &&
            right_ledge.field_0xc < 128.0f) {
            // Ledges on both sides: nothing to pick.
        } else if (IsFound(left_ledge) && left_ledge.field_0xc < 64.0f) {
            distance = left_ledge.field_0xc;
            const CStrId& name =
                !IsEmpty(source->left_far) && distance >= far.value ? source->left_far : source->left_near;
            request.id = name.data->Key();
        } else if (IsFound(right_ledge) && right_ledge.field_0xc < 64.0f) {
            distance = right_ledge.field_0xc;
            const CStrId& name =
                !IsEmpty(source->right_far) && distance >= far.value ? source->right_far : source->right_near;
            request.id = name.data->Key();
        }
        // Unless the ledge is right here, scale the movement by how much of 64 units the motion covers.
        if (distance != 0.0f) {
            UnkTimeline* timeline = static_cast<UnkTimeline*>(
                fn_7101176d34(self->actor->animation, reinterpret_cast<void*>(request.id)));
            if (timeline) {
                f32 height = Height(timeline);
                f32 room = 64.0f - distance;
                if (height < room)
                    movement->scale = room / height;
            }
        }
        fn_7100287954(&left_ledge);
        fn_7100287954(&right_ledge);
    }

    UnkGunWeapon_l83dd48* weapon = self->use_alt ? self->alt : self->main;
    if (weapon && weapon->PlaysRequest())
        fn_7101176ea8(self->actor->animation, reinterpret_cast<const UnkRequest_7101176ea8*>(&request),
                      0xf4061ff25a2163d6);
    fn_7101176a88(reinterpret_cast<UnkActorTarget*>(self->actor->animation), true);
}
