// behaviortree::CInFuryPatrolAction's unit (tools/tu_map.py unit 486): the action's start. It takes the
// actor's AI component and navigation, measures the height of the animation named at +0x90, starts the
// patrol in state 2 (state 1 when the point the navigation finds, pushed out by the property at +0x98,
// lies within the AI's reach of the patrol origin), listens to the actor's +0x220 component and notes
// whether the movement is in mode 4. Names and layouts are placeholders.
// Draft (61.9%): field_0x94 as u8 compared with 0 gives the original cset form (53.2% overall).
#include <math.h>

#include "base/UnkDestructors.h"
#include "base/UnkTunableLine.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/CEnemyMovement.h"
#include "game/UnkMotionRule.h"
#include "game/UnkTU1141.h"
#include "game/UnkTU1728.h"
#include "game/UnkTU57.h"
#include "game/behaviortree/CBehavior.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::math::CVector3D;

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

struct UnkPatrolConfig_l388fac {
    u8 pad_0x0[0xec];
    s32 origin_mode;  // 0xec
    u8 pad_0xf0[0x3a8];
    f32 reach;  // 0x498
};

struct UnkPatrolAI_l388fac {
    u8 pad_0x0[0x70];
    u32 mode;  // 0x70
    u8 pad_0x74[0x20];
    bool field_0x94;
    u8 pad_0x95[0x157b];
    UnkPatrolConfig_l388fac* config;  // 0x1610

    // The mode the AI would be in with nothing to chase: 1 while engaged and not held, else 0.
    u32 SettledMode() const { return !field_0x94 & (mode != 0); }
};

struct UnkPatrolMovement_l388fac {
    u8 pad_0x0[0x974];
    f32 origin_x;  // 0x974
    f32 origin_y;  // 0x978
};

struct UnkPatrolActor_l388fac {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x218 - 0x68];
    UnkPatrolMovement_l388fac* movement;  // 0x218
    UnkStruct_7101176ea8* animations;  // 0x220
    u8 pad_0x228[0x18];
    UnkPatrolAI_l388fac* ai;  // 0x240
    u8 pad_0x248[0x50];
    void* navigation;  // 0x298
};

struct UnkPatrolAction_l388fac;
void fn_710038a24c(UnkPatrolAction_l388fac* self);
void fn_710038a260(UnkPatrolAction_l388fac* self);

// A listener holding a weak handle to what it listens to, calling back the action.
struct UnkPatrolListener_l388fac {
    void** vtable;
    UnkPatrolAction_l388fac* owner;  // 0x8
    void (*callback)(UnkPatrolAction_l388fac*);  // 0x10
    UnkWeakHandle* link;  // 0x18
};

struct UnkPatrolAction_l388fac {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkPatrolActor_l388fac* actor;  // 0x18
    u8 pad_0x20[0x10];
    UnkPatrolAI_l388fac* ai;  // 0x30
    void* navigation;  // 0x38
    UnkPatrolAction_l388fac* listener_owner;  // 0x40
    void (*listener_callback)(UnkPatrolAction_l388fac*);  // 0x48
    bool field_0x50;
    u8 pad_0x51[3];
    s32 state;  // 0x54
    u8 pad_0x58[0x18];
    UnkPatrolListener_l388fac listener;  // 0x70
    base::UnkStrIdMember animation;  // 0x90
    base::UnkStrIdMember push;  // 0x98
    bool field_0xa0;
    u8 pad_0xa1[3];
    f32 height;  // 0xa4
    bool field_0xa8;
    bool moving;  // 0xa9
};
static_assert(__builtin_offsetof(UnkPatrolAction_l388fac, moving) == 0xa9, "layout");

// A line holding a string id's text.
struct UnkIdLine_l388fac : UnkTunableLine {
    explicit UnkIdLine_l388fac(const base::UnkStrIdMember& id) { fn_710007eb6c(Str(), &id); }
};

bool fn_71010fcd04(UnkPatrolAI_l388fac* ai, s32* state, const u32* flags, CVector3D* point, f32 radius);

void fn_7100388fac(UnkPatrolAction_l388fac* self) {
    fn_7100394bd0(reinterpret_cast<UnkStruct_0x68*>(self));
    UnkPatrolActor_l388fac* actor = self->actor;
    if (!(self->ai = actor->ai))
        return;
    if (!(self->navigation = actor->navigation))
        return;
    UnkStruct_7101176ea8* animations = actor->animations;
    if (!animations)
        return;
    self->field_0xa0 = false;
    self->field_0xa8 = true;
    self->listener_owner = self;
    self->listener_callback = fn_710038a24c;
    self->field_0x50 = false;
    self->state = 2;
    UnkPatrolMovement_l388fac* movement = actor->movement;
    self->height = 0.0f;

    UnkIdLine_l388fac name(self->animation);
    if (!name.IsEmpty()) {
        UnkTimeline* timeline = static_cast<UnkTimeline*>(
            fn_7101176d34(self->actor->animations, reinterpret_cast<void*>(self->animation.data->Key())));
        if (timeline) {
            UnkStruct_71007142e8 motion;
            fn_71007142e8(&motion);
            UnkMotionTracks* tracks = timeline->tracks;
            UnkFlags_71007142e8 flags = UnkFlagsAll_71007142e8();
            fn_7100b8b3bc(timeline, &motion, tracks, &flags.bits, 0.0f, timeline->frame_count);
            self->height = motion.m[11];
        }
    }

    u32 flags = 1;
    if (movement && fn_71010cc3a4(movement) == 1) {
        UnkPatrolAI_l388fac* ai = self->ai;
        if (ai->SettledMode() != ai->mode) {
            CVector3D point;
            point.x = 0.0f;
            point.y = 0.0f;
            point.z = 0.0f;
            if (fn_71010fcd04(ai, &self->state, &flags, &point, 0.0f)) {
                f32 push = 0.0f;
                if (self->push.data != &g_unk_7101d47108->empty)
                    fn_7100394e2c(reinterpret_cast<UnkSelf*>(self), &self->push, &push);
                f32 x, y, z = 0.0f;
                if (self->ai->config->origin_mode == 1) {
                    UnkPatrolActor_l388fac* a = self->actor;
                    x = a->x;
                    y = a->y;
                    z = a->z;
                } else {
                    x = movement->origin_x;
                    y = movement->origin_y;
                }
                f32 dx = point.x - x;
                f32 dy = point.y - y;
                f32 dz = point.z - z;
                f32 scale = 1.0f / (sqrtf(dx * dx + dy * dy + dz * dz) + 0.0001f);
                f32 ex = x + push * (dx * scale) - point.x;
                f32 ey = y + push * (dy * scale) - point.y;
                f32 ez = z + push * (dz * scale) - point.z;
                f32 reach = self->actor->ai->config->reach;
                if (ex * ex + ey * ey + ez * ez < reach * reach)
                    self->state = 1;
            }
        }
    }

    self->listener.owner = self;
    self->listener.callback = fn_710038a260;
    if (UnkWeakHandle* l = self->listener.link) {
        if (UnkWeakTarget* target = l->target) {
            fn_710117b3f0(target, &self->listener);
            l = self->listener.link;
        }
        if (l) {
            if (--l->ref_count == 0)
                l->vf_0x10();
            self->listener.link = nullptr;
        }
    }
    fn_7100316018(reinterpret_cast<UnkWeakTarget*>(animations), &self->listener.link);
    fn_710117b3e8(self->listener.link ? self->listener.link->target : nullptr, &self->listener);
    self->moving = false;
    if (fn_71010cc3a4(movement) == 4)
        self->moving = true;
}
