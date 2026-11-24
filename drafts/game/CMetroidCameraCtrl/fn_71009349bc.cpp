// CMetroidCameraCtrl unit: the camera controller's update. Follows the player (or the morph ball), offsets
// the camera by the player's aim, grab and track state, eases its zoom and pulls back near an Emmy.
// Names and layouts are placeholders.
#include <math.h>

#include "base/UnkAtExit.h"
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAnimationComponent.h"
#include "game/CCameraBoundaryCtrl.h"
#include "game/CModelUpdaterComponent.h"
#include "game/CSamusMagnetGloveMovementState/CParams.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU0.h"
#include "game/UnkTU1501.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU39.h"
#include "game/UnkTU811.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkCameraCtrl_ovb0049;
f32 fn_71010ee560(UnkCameraCtrl_ovb0049* self);
bool fn_71009e8524(UnkFlagged*);
bool fn_71009ae85c(UnkFootstepMode* mode);
bool fn_71009ae888(UnkFootstepMode* mode);

struct UnkCamTarget_l9349bc {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkCamRange_l9349bc {
    u8 pad_0x0[0x3c];
    f32 height;  // 0x3c
    f32 min;     // 0x40
    f32 max;     // 0x44
};

struct UnkCamRangeHolder_l9349bc {
    u8 pad_0x0[0x10];
    UnkCamRange_l9349bc* range;
};

struct UnkMetroidCameraCtrl_l9349bc {
    u8 pad_0x0[0x68];
    UnkVec3f field_0x68;
    UnkVec3f field_0x74;
    u8 pad_0x80[0x8c - 0x80];
    f32 field_0x8c;
    u8 pad_0x90[0x94 - 0x90];
    f32 field_0x94;
    u8 pad_0x98[0xb0 - 0x98];
    UnkCamTarget_l9349bc* field_0xb0;
    UnkCamRangeHolder_l9349bc* field_0xb8;
    u8 pad_0xc0[0x244 - 0xc0];
    UnkVec3f pos;  // 0x244
    f32 field_0x250;
    f32 field_0x254;
    f32 field_0x258;
    f32 field_0x25c;
    u8 pad_0x260[0x2a0 - 0x260];
    UnkVec3f field_0x2a0;
    UnkVec3f field_0x2ac;
    f32 field_0x2b8;
    f32 field_0x2bc;
    f32 field_0x2c0;
    f32 field_0x2c4;
    f32 field_0x2c8;
    f32 field_0x2cc;
    u8 pad_0x2d0[0x2e0 - 0x2d0];
    f32 field_0x2e0;
    u8 pad_0x2e4[0x2e8 - 0x2e4];
    f32 field_0x2e8;
    u8 pad_0x2ec[0x2f0 - 0x2ec];
    f32 field_0x2f0;
    f32 field_0x2f4;
    u8 pad_0x2f8[0x2fc - 0x2f8];
    bool field_0x2fc;
    bool field_0x2fd;
    u8 pad_0x2fe[0x310 - 0x2fe];
    u8 timer[0x3c0 - 0x310];  // 0x310
    f32 field_0x3c0;
    f32 field_0x3c4;
};
static_assert(__builtin_offsetof(UnkMetroidCameraCtrl_l9349bc, pos) == 0x244, "");
static_assert(__builtin_offsetof(UnkMetroidCameraCtrl_l9349bc, field_0x2fd) == 0x2fd, "");
static_assert(__builtin_offsetof(UnkMetroidCameraCtrl_l9349bc, field_0x3c4) == 0x3c4, "");

struct UnkMorphMovement_l9349bc {
    u8 pad_0x0[0xf9];
    bool field_0xf9;
    u8 pad_0xfa[0x10c - 0xfa];
    f32 field_0x10c;
    f32 field_0x110;
    u8 pad_0x114[0x2288 - 0x114];
    s32 field_0x2288;
};

struct UnkGrabItem_l9349bc {
    u8 pad_0x0[0x10];
    u32 id;
};

struct __attribute__((packed)) UnkGrabItems_l9349bc {
    u32 count;
    UnkGrabItem_l9349bc* items[15];
};

struct UnkGrab_l9349bc {
    u8 pad_0x0[0xf9];
    bool field_0xf9;
    u8 pad_0xfa[0x2d18 - 0xfa];
    UnkGrabItems_l9349bc items;  // 0x2d18
};

struct UnkAimMode_l9349bc {
    u8 pad_0x0[0x10];
    s32 mode;
};

struct UnkAim_l9349bc {
    u8 pad_0x0[0x50];
    UnkAimMode_l9349bc* mode;
    u8 pad_0x58[0x2b8 - 0x58];
    f32 angle;
};

struct UnkSamusAnimation_l9349bc {
    u8 pad_0x0[0x530];
    s32 state;
};

struct UnkCamPlayer_l9349bc {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x1f0 - 0x94];
    UnkMorphMovement_l9349bc* movement;
    u8 pad_0x1f8[0x200 - 0x1f8];
    void* field_0x200;
    UnkGrab_l9349bc* grab;
    u8 pad_0x210[0x220 - 0x210];
    UnkAnimationComponent* animation;
    void* models;
    u8 pad_0x230[0x2b0 - 0x230];
    UnkAim_l9349bc* aim;
};

struct UnkEmmyActor_l9349bc {
    u8 pad_0x0[0x38];
    UnkXform xform;
    u8 pad_0x94[0x1b4 - 0x94];
    bool field_0x1b4;
};

struct UnkEmmyComponent_l9349bc {
    u8 pad_0x0[0x30];
    UnkEmmyActor_l9349bc* owner;
};

struct UnkListed_l9349bc {
    u8 pad_0x0[0x10];
    UnkEmmyComponent_l9349bc* component;
};

struct UnkFrameTimer_l9349bc {
    u8 pad_0x0[0x30];
    f32 dt;
};

struct UnkCameraTunables_l9349bc {
    u8 pad_0x0[0x64];
    f32 distance;
    f32 emmyNearDistance;
    f32 emmyFarDistance;
    f32 emmyNear;
    f32 emmyFar;
};
extern UnkCameraTunables_l9349bc g_unk_7172738c28;

inline const CStrId& GrabTrackId_l9349bc() {
    static UnkGameManagerIdTable s_table asm("g_unk_7101d552d0");
    return *reinterpret_cast<const CStrId*>(reinterpret_cast<const u8*>(&s_table) + 0x12f0);
}

template <const base::reflection::CClass* (*GetClass)()>
inline bool IsKindOf_l9349bc(void* object) {
    const base::reflection::CClass* type = reinterpret_cast<UnkReflObject*>(object)->vf_0x0();
    return fn_710009d50c(type, GetClass());
}

inline f32 Clamp_l9349bc(f32 v, f32 lo, f32 hi) {
    if (v <= lo)
        return lo;
    if (v >= hi)
        return hi;
    return v;
}

inline f32 Lerp_l9349bc(f32 a, f32 b, f32 t) {
    if (t >= 1.0f)
        return b;
    if (t < 0.0f)
        return a;
    return t * b + (1.0f - t) * a;
}

inline f32 Sign_l9349bc(f32 v) {
    return (f32)((v > 0.0f) - (v < 0.0f));
}

// The pitch input (-1.9 to 1.9) as an angle in degrees.
inline f32 PitchAngle_l9349bc(f32 pitch, f32 lo, f32 hi) {
    return Clamp_l9349bc(pitch, lo, hi) / 1.9f * 180.0f;
}

void fn_71009349bc(UnkMetroidCameraCtrl_l9349bc* self) {
    fn_7100764388(reinterpret_cast<UnkPureFollowCameraCtrl*>(self));
    if (!self->field_0xb0 || !self->field_0xb0->field_0x10)
        return;

    self->field_0x250 = self->field_0x254;
    f32 fixedDistance = self->field_0x3c0;
    if (UnkCamRangeHolder_l9349bc* holder = self->field_0xb8) {
        if (UnkCamRange_l9349bc* range = holder->range) {
            f32 lo = range->min;
            f32 hi = range->max;
            f32 distance = self->field_0x258;
            if (distance < lo || distance > hi)
                self->field_0x258 = fixedDistance > 0.0f ? fixedDistance : Clamp_l9349bc(distance, lo, hi);
        }
        if (UnkCamRange_l9349bc* range = holder->range) {
            f32 below = self->field_0x8c - 0.001f;
            f32 height = range->height;
            f32 above = self->field_0x8c + 0.001f;
            if (!(above >= height && below <= height))
                self->field_0x8c = height;
        }
    }

    UnkCamPlayer_l9349bc* player = reinterpret_cast<UnkCamPlayer_l9349bc*>(fn_7100dec788(g_unk_7101d47310));
    if (player) {
        bool tweakGravity = false;
        if (fn_710117ae38(player->animation, 0x5732a6bc3abeadull, false))
            tweakGravity = fn_710117b348(player->animation, fn_7100d30c28());

        UnkMorphMovement_l9349bc* movement = player->movement;
        UnkAim_l9349bc* aim = player->aim;
        UnkGrab_l9349bc* grab = player->grab;
        UnkAnimationComponent* animation = player->animation;
        bool grabA = false;
        bool grabB = false;
        bool grabHanging = false;
        bool grabD = false;
        if (grab) {
            grabA = fn_71009e75e8(reinterpret_cast<UnkFlagged*>(grab));
            grabB = fn_71009e8524(reinterpret_cast<UnkFlagged*>(grab));
            grabHanging = fn_71009e6434(reinterpret_cast<UnkHelper*>(grab));
            grabD = fn_71009e12d0(grab) & !grabHanging;
        }

        f32 prevY = self->pos.y;
        u64 dcCamId = 0xd280200dfa7467a3ull;
        bool inMorphBall;
        UnkMorphMovement_l9349bc* morph;
        if (movement && IsKindOf_l9349bc<fn_710093b204>(movement)) {
            self->pos = player->xform.GetTranslation();
            if (fn_710117b348(animation, fn_7100c3f96c()) &&
                !fn_710117364c(player->models, &dcCamId, reinterpret_cast<UnkGrappleAnswer_b50*>(&self->pos)))
                self->pos = player->xform.GetTranslation();
            inMorphBall = true;
            morph = movement;
        } else {
            if (!grab ||
                !fn_710117364c(player->models, &dcCamId, reinterpret_cast<UnkGrappleAnswer_b50*>(&self->pos)))
                self->pos = *reinterpret_cast<const UnkVec3f*>(fn_7100d7c9f4(reinterpret_cast<UnkPlayer*>(player)));
            self->pos.y -= 28.0f;
            inMorphBall = false;
            morph = nullptr;
        }

        if (tweakGravity) {
            self->field_0x3c4 = prevY - self->pos.y;
            self->pos.y = prevY;
        } else {
            self->pos.y = self->field_0x3c4 + self->pos.y;
        }
        self->pos.z = 0.0f;

        if (fn_710002fb70(self->timer) || fn_710117b348(animation, fn_7100d0f218())) {
            self->field_0x2c4 = 0.0f;
            self->field_0x2c8 = 0.0f;
            self->field_0x2cc = 0.0f;
        } else {
            f32 yawInput = 0.0f;
            f32 pitchInput = 0.0f;
            if (movement) {
                yawInput = movement->field_0x10c * 0.0010373297f;
                pitchInput = movement->field_0x110 * 0.0010373297f;
            }
            f32 lateral = Clamp_l9349bc(yawInput, -1.0f, 1.0f);
            if (fn_710117b348(animation, fn_7100cfd938())) {
                f32 facing = Sign_l9349bc(player->xform.GetMatrix().m[2]);
                lateral = (yawInput != 0.0f ? (fabsf(yawInput) >= 1.0f ? 1.0f : fabsf(yawInput)) : 0.0f) * facing;
            }
            f32 push = lateral * 450.0f;

            if (aim) {
                UnkAimMode_l9349bc* aimMode = aim->mode;
                if (aimMode && (aimMode->mode == 2 || aimMode->mode == 3) && animation &&
                    fn_710117b348(animation, fn_7100ceadf8())) {
                    f32 angle = aim->angle;
                    f32 facing = player->xform.GetAngle().y;
                    self->field_0x2c4 = cosf(angle) * (facing < 0.0f ? -150.0f : 150.0f);
                    self->field_0x2c8 = sinf(angle) * self->field_0x2e0;
                } else {
                    s32 state = 0;
                    bool hanging = false;
                    if (animation) {
                        if (IsKindOf_l9349bc<fn_7100713bb0>(animation))
                            state = reinterpret_cast<UnkSamusAnimation_l9349bc*>(animation)->state;
                        if (grab && grabD &&
                            !CAnimationComponent_IsPlayingActionNameContaining(reinterpret_cast<UnkStruct_710117af10*>(animation),
                                           reinterpret_cast<UnkCls_710117af10**>(
                                               const_cast<CStrId*>(&GrabTrackId_l9349bc())),
                                           false))
                            hanging = true;
                    }

                    bool aimed;
                    if (!hanging && grabHanging) {
                        u32 id = fn_71009a60fc();
                        UnkGrabItem_l9349bc* item = nullptr;
                        if (id < 15) {
                            for (u32 i = 0; i < grab->items.count; i++) {
                                if (grab->items.items[i]->id == id) {
                                    item = grab->items.items[i];
                                    break;
                                }
                            }
                        }
                        UnkFootstepMode* mode = reinterpret_cast<UnkFootstepMode*>(item);
                        if (fn_71009ae85c(mode) ? fn_71009ae888(mode) : fn_71009ae798(mode))
                            aimed = true;
                        else
                            hanging = true;
                    }
                    if (hanging) {
                        aimed = state == 2 || state == 3 || fn_710117b348(animation, fn_7100d024b8());
                    } else if (!grabHanging) {
                        if (state == 4 || state == 2) {
                            aimed = true;
                        } else {
                            f32 heading = fabsf(player->xform.GetAngle().y);
                            aimed = state != 1 && state != 5 && heading - 0.1f <= 1.5707964f &&
                                    heading + 0.1f >= 1.5707964f;
                        }
                    }

                    f32 target = 0.0f;
                    if (aimed)
                        target = Sign_l9349bc(player->xform.GetAngle().y) * 150.0f;
                    if (target > 0.0f) {
                        if (push > 0.0f && push > target)
                            target = push;
                    } else if (push < 0.0f && push < target) {
                        target = push;
                    }
                    self->field_0x2c4 = target;

                    if (!grabD && !grabA && grab && (state == 1 || state == 2)) {
                        f32 height = self->field_0x2e0;
                        if (state == 2)
                            height *= 0.6f;
                        self->field_0x2c8 = height;
                        if (grab->field_0xf9)
                            self->field_0x2c8 = height * 0.8f;
                    } else if (!grabD && !grabA && grab && (state == 4 || state == 5)) {
                        f32 height = state == 4 ? self->field_0x2e0 * -0.6f : -self->field_0x2e0;
                        self->field_0x2c8 = height;
                        if (grab->field_0xf9)
                            self->field_0x2c8 = height * 0.8f;
                    } else {
                        self->field_0x2c8 = 0.00001f;
                    }
                }
            }

            bool wasInMorphBall = self->field_0x2fd;
            if (inMorphBall) {
                if (!wasInMorphBall)
                    self->field_0x2f4 = 0.1f;
                if (morph->field_0x2288 == 3) {
                    self->field_0x2c8 = 0.00001f;
                    f32 yaw = Clamp_l9349bc(yawInput, -4.1493f, 4.1493f) / 4.1493f * 270.5f;
                    f32 pitch = Clamp_l9349bc(pitchInput, -4.1493f, 4.1493f) / 4.1493f * 270.5f;
                    self->field_0x2c4 = yaw;
                    self->field_0x2cc = pitch;
                    if (!self->field_0x2fc) {
                        self->field_0x2b8 = yaw;
                        self->field_0x2c0 = pitch;
                    }
                    self->field_0x2e8 = 50.0f;
                    self->field_0x2f0 = 50.0f;
                    self->field_0x2fc = true;
                } else {
                    f32 yaw;
                    f32 pitch;
                    if (self->field_0x2fc) {
                        yaw = 0.0f;
                        pitch = 0.0f;
                        self->field_0x2b8 = 0.0f;
                        self->field_0x2c0 = 0.0f;
                    } else {
                        yaw = push * 0.5f;
                        pitch = PitchAngle_l9349bc(pitchInput, -1.9f, 0.0f) * 0.5f;
                    }
                    self->field_0x2c4 = yaw;
                    self->field_0x2cc = pitch;
                    self->field_0x2c8 = 0.00001f;
                    self->field_0x2e8 = 3.0f;
                    self->field_0x2f0 = 20.0f;
                    self->field_0x2fc = false;
                    if (!morph->field_0xf9)
                        self->field_0x2cc = 0.0f;
                }
                self->field_0x2fd = true;
            } else {
                if (wasInMorphBall)
                    self->field_0x2f4 = 0.05f;
                f32 pitch = pitchInput > 0.0f ? pitchInput : pitchInput * 0.7f;
                if (grabB) {
                    f32 t = Clamp_l9349bc(fn_71009e8590(reinterpret_cast<UnkFlagged*>(grab)) * 0.5f, 0.0f, 1.0f);
                    f32 tilt = PitchAngle_l9349bc(pitch, -1.9f, 1.9f) * Lerp_l9349bc(1.0f, 1.6f, t);
                    if (pitchInput > 0.0f)
                        tilt *= 1.2f;
                    self->field_0x2cc = tilt;
                    self->field_0x2f0 = (t * -0.6f + 1.0f) * 20.0f;
                } else {
                    self->field_0x2cc = PitchAngle_l9349bc(pitch, -1.9f, 1.9f);
                    self->field_0x2f0 = 20.0f;
                }
                if (player->movement && !player->movement->field_0xf9)
                    self->field_0x2cc = 0.0f;
                self->field_0x2e8 = 3.0f;
                self->field_0x2fc = false;
                self->field_0x2fd = false;
            }
        }
    }

    f32 dt = reinterpret_cast<UnkFrameTimer_l9349bc*>(g_unk_7101d47328)->dt;
    f32 zoom = self->field_0x25c * (dt != 0.0f ? dt * 1.1f * 30.0f : 1.1f);
    self->field_0x25c = zoom < 1.0f ? zoom : 1.0f;
    f32 factor = fn_71010ee560(reinterpret_cast<UnkCameraCtrl_ovb0049*>(self));
    self->field_0x2a0 = self->field_0x68;
    self->field_0x94 = factor;
    self->field_0x2ac = self->field_0x74;

    if (!player || !player->field_0x200)
        return;
    if (fn_7100552d7c()->count == 0)
        return;
    UnkListed_l9349bc* first = reinterpret_cast<UnkListed_l9349bc*>(fn_7100552d7c()->data[0]);
    if (!first)
        return;
    UnkEmmyComponent_l9349bc* emmy = first->component;
    if (!emmy || !IsKindOf_l9349bc<fn_710053b934>(emmy))
        return;
    UnkEmmyActor_l9349bc* owner = emmy->owner;
    if (!owner || !owner->field_0x1b4)
        return;

    UnkVec3f a = owner->xform.GetTranslation();
    UnkVec3f b = player->xform.GetTranslation();
    f32 dx = a.x - b.x;
    f32 dy = a.y - b.y;
    f32 dz = a.z - b.z;
    f32 distance = sqrtf(dx * dx + dy * dy + dz * dz);
    f32 value;
    if (IsOpticCamouflageAbilityInState6(g_unk_7101d47310)) {
        UnkCameraTunables_l9349bc& tune = g_unk_7172738c28;
        f32 t = Clamp_l9349bc((distance - tune.emmyNearDistance) / (tune.emmyFarDistance - tune.emmyNearDistance),
                              0.0f, 1.0f);
        value = tune.emmyNear + (tune.emmyFar - tune.emmyNear) * t;
    } else {
        value = self->field_0x3c0 > 0.0f ? self->field_0x3c0 : g_unk_7172738c28.distance;
    }
    self->field_0x258 = value;
}
