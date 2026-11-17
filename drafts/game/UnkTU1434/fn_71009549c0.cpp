// UnkTU1434 (tools/tu_map.py unit 1434): picks the owner's locomotion animation (idle, walk, turn or stop)
// from how it moves and which animation plays, and writes its key and the new state.
// Names and layouts are placeholders.
#include <math.h>

#include "base/global/CStrId.h"
#include "game/CAnimationComponent.h"
#include "game/CEnemyMovement.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

using base::global::UnkStringData;

// One animation name of the set: a pooled string id.
struct UnkAnimName_l9549c0 {
    UnkStringData* data;

    bool IsSet() const { return data != &g_unk_7101d47108->empty; }
    u64 Key() const { return data->Key(); }
};

// The names: 0 idle, 1 and 5 walk, 2 and 6 walk backwards, 3 and 7 turn one way, 4 and 8 the other,
// 9 to 12 the stop of states 2 to 5.
struct UnkAnimSet_l9549c0 {
    UnkAnimName_l9549c0 idle;            // 0x0
    UnkAnimName_l9549c0 forward;         // 0x8
    UnkAnimName_l9549c0 backward;        // 0x10
    UnkAnimName_l9549c0 turn_left;       // 0x18
    UnkAnimName_l9549c0 turn_right;      // 0x20
    UnkAnimName_l9549c0 forward_start;   // 0x28
    UnkAnimName_l9549c0 backward_start;  // 0x30
    UnkAnimName_l9549c0 turn_left_start;   // 0x38
    UnkAnimName_l9549c0 turn_right_start;  // 0x40
    UnkAnimName_l9549c0 stop[4];           // 0x48
};

struct UnkAnimSource_l9549c0;

struct UnkAnimData_l9549c0 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual UnkAnimSet_l9549c0* GetSet(UnkAnimSource_l9549c0* owner);
    u8 pad_0x8[0x128 - 0x8];
    u32 field_0x128;
    f32 turn_left_angle;   // 0x12c
    f32 turn_right_angle;  // 0x130
};

struct UnkMovement_l9549c0 {
    u8 pad_0x0[0x40];
    f32 forward_x;  // 0x40
    f32 forward_y;
    f32 forward_z;
    u8 pad_0x4c[0x58 - 0x4c];
    f32 target_x;  // 0x58
    f32 target_y;
    f32 target_z;
    u8 pad_0x64[0x7c - 0x64];
    f32 speed;  // 0x7c
};

struct UnkOwnerSub_l9549c0 {
    u8 pad_0x0[0x1610];
    void* field_0x1610;
};

struct UnkAnimSource_l9549c0 {
    u8 pad_0x0[0x218];
    UnkMovement_l9549c0* movement;       // 0x218
    UnkAnimationComponent* animation;    // 0x220
    u8 pad_0x228[0x240 - 0x228];
    UnkOwnerSub_l9549c0* field_0x240;
};

struct UnkAnimRequest_l9549c0 {
    u64 key;
    s64 field_0x8;
    u32 field_0x10;
    u32 field_0x14;
};

struct UnkSelf_l9549c0 {
    u8 pad_0x0[0x30];
    UnkAnimSource_l9549c0* owner;  // 0x30
    u8 pad_0x38[0x8];
    UnkAnimData_l9549c0* data;  // 0x40
};

// Whether a speed is outside the dead zone around zero.
static inline bool IsMoving(f32 speed) {
    return !(speed + 0.001f >= 0.0f) || !(speed - 0.001f <= 0.0f);
}

// Whether the animation component has an animation of that name.
static inline bool Has(UnkAnimationComponent* animation, const UnkAnimName_l9549c0& name) {
    return name.IsSet() &&
           fn_7101176d34(reinterpret_cast<UnkStruct_7101176ea8*>(animation), reinterpret_cast<void*>(name.Key()));
}

// Whether that animation is the one playing.
static inline bool IsPlaying(UnkAnimationComponent* animation, const UnkAnimName_l9549c0& name) {
    return name.IsSet() && fn_710117ae38(animation, name.Key(), false);
}

// A move's animation: its start when the move begins and the loop is not already playing, else the
// start while it still plays, then the loop.
static inline const UnkAnimName_l9549c0* Choose(UnkAnimationComponent* animation, s32* state, s32 move,
                                                const UnkAnimName_l9549c0* loop,
                                                const UnkAnimName_l9549c0* start) {
    if (*state != move && Has(animation, *start) && !IsPlaying(animation, *loop))
        return start;
    if (IsPlaying(animation, *start))
        return start;
    return loop;
}

void fn_71009549c0(UnkSelf_l9549c0* self, s32* state, UnkAnimRequest_l9549c0* request) {
    UnkAnimSource_l9549c0* owner = self->owner;
    UnkMovement_l9549c0* movement = owner ? owner->movement : nullptr;
    UnkAnimationComponent* animation = owner ? owner->animation : nullptr;
    bool has_sub = owner && owner->field_0x240;
    void* sub = has_sub ? owner->field_0x240->field_0x1610 : nullptr;
    UnkAnimSet_l9549c0* set = self->data->GetSet(owner);
    if (sub && has_sub && movement && animation) {
        switch (*state) {
        case 0:
            request->key = set->idle.Key();
            *state = 1;
            break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6: {
            f32 forward_x = movement->forward_x;
            f32 forward_y = movement->forward_y;
            f32 forward_z = movement->forward_z;
            f32 target_x = movement->target_x;
            f32 target_y = movement->target_y;
            f32 target_z = movement->target_z;
            s32 next;
            if (IsMoving(movement->speed) && !fn_71010cab0c(movement)) {
                f32 dot = forward_x * target_x + forward_y * target_y + forward_z * target_z;
                f32 angle = acosf(dot);
                f32 cross = forward_x * target_y - forward_y * target_x;
                const UnkAnimName_l9549c0* pick;
                if (set->turn_left.IsSet() && angle > self->data->turn_left_angle &&
                    ((target_x < 0.0f && cross > 0.0f) || (target_x > 0.0f && cross < 0.0f))) {
                    pick = Choose(animation, state, 4, &set->turn_left, &set->turn_left_start);
                    next = 4;
                } else if (set->turn_right.IsSet() && angle > self->data->turn_right_angle &&
                           ((target_x < 0.0f && cross < 0.0f) || (target_x > 0.0f && cross > 0.0f))) {
                    pick = Choose(animation, state, 5, &set->turn_right, &set->turn_right_start);
                    next = 5;
                } else if (dot < 0.0f) {
                    pick = Choose(animation, state, 3, &set->backward, &set->backward_start);
                    next = 3;
                } else {
                    pick = Choose(animation, state, 2, &set->forward, &set->forward_start);
                    next = 2;
                }
                request->key = pick->Key();
            } else {
                // Standing: the stop of the move just ended, if there is one, else idle.
                bool idle = true;
                if (*state != 1) {
                    if (*state == 2 && Has(animation, set->stop[0])) {
                        request->key = set->stop[0].Key();
                        idle = false;
                    } else if (*state == 3 && Has(animation, set->stop[1])) {
                        request->key = set->stop[1].Key();
                        idle = false;
                    } else if (*state == 4 && Has(animation, set->stop[2])) {
                        request->key = set->stop[2].Key();
                        idle = false;
                    } else if (*state == 5 && Has(animation, set->stop[3])) {
                        request->key = set->stop[3].Key();
                        idle = false;
                    }
                }
                const UnkAnimName_l9549c0* pick = nullptr;
                if (IsPlaying(animation, set->stop[0]))
                    pick = &set->stop[0];
                else if (IsPlaying(animation, set->stop[1]))
                    pick = &set->stop[1];
                else if (IsPlaying(animation, set->stop[2]))
                    pick = &set->stop[2];
                else if (IsPlaying(animation, set->stop[3]))
                    pick = &set->stop[3];
                else if (idle)
                    pick = &set->idle;
                if (pick)
                    request->key = pick->Key();
                next = 1;
            }
            *state = next;
            request->field_0x8 = -1;
            request->field_0x10 = self->data->field_0x128;
            request->field_0x14 = 1;
            break;
        }
        }
    }
    if (request->key == static_cast<u64>(-1))
        request->key = self->data->GetSet(self->owner)->idle.Key();
}
