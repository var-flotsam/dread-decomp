// CFreeCameraCtrl unit: the per-frame step of the free camera. It takes the speeds from the held
// buttons, builds the right and up axes from the viewing direction, moves the camera along them,
// turns the direction from the right stick (or the sound group's look input), clamps the height
// around the player and stores the distance factor. Names and layouts are placeholders.
#include <math.h>

#include "base/math/CVectorOps.h"
#include "base/global/CRntVector.h"
#include "game/UnkGameManager.h"
#include "game/UnkMat4f.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU54.h"
#include "game/nn/time.h"
#include "types.h"

using base::math::CVector3D;

struct UnkSoundGroup {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual bool vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50(f32* first, f32* second);
};

struct UnkSoundHolder {
    u8 pad_0x0[8];
    UnkSoundGroup* group;  // 0x8
};

struct UnkGfxFrameSource {
    u8 pad_0x0[0x30];
    f32 field_0x30;
};

struct UnkFreeCameraCtrl_l0efa58 {
    u8 pad_0x0[0x38];
    CVector3D target;  // 0x38
    u8 pad_0x44[0x68 - 0x44];
    CVector3D position;  // 0x68
    CVector3D forward;   // 0x74
    u8 pad_0x80[0x94 - 0x80];
    f32 field_0x94;
    u8 pad_0x98[0x208 - 0x98];
    f32 field_0x208;
    u8 pad_0x20c[0xbf2 - 0x20c];
    bool fast;  // 0xbf2
    bool slow;  // 0xbf3
    f32 side;     // 0xbf4
    f32 ahead;    // 0xbf8
    f32 turn;     // 0xbfc
    f32 pitch;    // 0xc00
    f32 field_0xc04;
    f32 field_0xc08;
    bool field_0xc0c;
};

struct UnkCameraCtrl_ovb0049;
void fn_71010ede94(UnkFreeCameraCtrl_l0efa58* self);
f32 fn_71010ee560(UnkCameraCtrl_ovb0049* self);
UnkVec3f fn_710007a320(const UnkVec3f* v, f32 angle);

struct UnkLook_l0efa58 {
    f32 pitch;
    f32 turn;
};

// The vector scaled to unit length, the z component last.
inline CVector3D NormalizedAxis(const CVector3D& v) {
    f32 inv = base::math::InvLengthEps(v);
    return CVector3D{v.x * inv, v.y * inv, inv * v.z};
}

void fn_71010efa58(UnkFreeCameraCtrl_l0efa58* self) {
    fn_71010ede94(self);
    f32 dt = g_unk_7101d47328->field_0x30 * 120.0f;
    f32 move;
    f32 rate;
    if (self->fast) {
        move = dt * 3.0f;
        rate = dt * 5.0f;
    } else if (self->slow) {
        rate = dt * 0.5f;
        move = rate;
    } else if (self->field_0xc08 > 0.0f) {
        move = dt * 0.14f;
        rate = dt * 1.4f;
    } else {
        move = dt;
        rate = dt;
        if (self->field_0xc04 > 0.0f) {
            move = dt * 0.7f;
            rate = dt * 7.0f;
        }
    }
    f32 low = dt * 2664.0f;
    f32 high = dt * 24000.0f;
    f32 scaled = move * 8000.0f;
    f32 speed = scaled > low ? (scaled >= high ? high : scaled) : low;

    CVector3D right = NormalizedAxis(base::math::Cross(self->forward, CVector3D{0.0f, 1.0f, 0.0f}));
    const CVector3D& f = self->forward;
    CVector3D up = base::math::Normalized(CVector3D{right.y * f.z - right.z * f.y, right.z * f.x - f.z * right.x,
                                                    f.y * right.x - right.y * f.x});
    CVector3D step = CVector3D{0.0f, 0.0f, 0.0f} + right * self->side;
    const CVector3D* ahead = self->field_0xc04 > 0.0f ? &self->forward : &up;
    step = step + *ahead * self->ahead;
    CVector3D unit = base::math::Normalized(step);
    self->position.x = self->target.x + speed * unit.x * 0.0025f;
    self->position.y = self->target.y + speed * unit.y * 0.0025f;
    self->position.z = self->target.z + speed * unit.z * 0.0025f;

    f32 one = 1.0f;
    if (self->field_0xc0c) {
        UnkSoundHolder* holder = g_unk_7101d47340;
        if (!holder) {
            holder = static_cast<UnkSoundHolder*>(fn_71000839a0(0x48, 0x3f));
            fn_710006cd9c(reinterpret_cast<UnkSoundHolder*>(holder));
            g_unk_7101d47340 = holder;
        }
        UnkSoundGroup* group = holder->group;
        if (group && group->vf_0x28()) {
            UnkLook_l0efa58 look = {};
            group->vf_0x50(&look.turn, &look.pitch);
            f32 stick = self->fast ? 0.2f : (self->slow ? 0.01f : 0.05f);
            if (look.turn > 0.01f) {
                UnkVec3f r = fn_710007a320(reinterpret_cast<const UnkVec3f*>(&self->forward), -(stick * look.turn));
                self->forward.x = r.x;
                self->forward.y = r.y;
                self->forward.z = r.z;
            } else if (look.turn < -0.01f) {
                UnkVec3f r = fn_710007a320(reinterpret_cast<const UnkVec3f*>(&self->forward), -(stick * look.turn));
                self->forward.x = r.x;
                self->forward.y = r.y;
                self->forward.z = r.z;
            }
            f32 angle = 0.0f;
            if (look.pitch > 0.01f)
                angle = stick * look.pitch;
            else if (look.pitch < -0.01f)
                angle = stick * look.pitch;
            UnkMatrix44 m = fn_710007966c(&right, angle);
            UnkVec3f r = fn_7100078170(reinterpret_cast<const UnkMat4f*>(&m),
                                       reinterpret_cast<const UnkVec3f*>(&self->forward));
            self->forward.x = r.x;
            self->forward.y = r.y;
            self->forward.z = r.z;
        }
    } else {
        if (fabsf(self->turn) > 0.3f) {
            if (self->turn > 0.0f) {
                UnkVec3f r = fn_710007a320(reinterpret_cast<const UnkVec3f*>(&self->forward), rate * -0.00125f);
                self->forward.x = r.x;
                self->forward.y = r.y;
                self->forward.z = r.z;
            } else {
                UnkVec3f r = fn_710007a320(reinterpret_cast<const UnkVec3f*>(&self->forward), rate * 0.00125f);
                self->forward.x = r.x;
                self->forward.y = r.y;
                self->forward.z = r.z;
            }
        }
        if (fabsf(self->pitch) > 0.3f) {
            f32 angle = self->pitch > 0.0f ? -0.00125f : 0.00125f;
            CVector3D old = self->forward;
            UnkMatrix44 m = fn_710007966c(&right, rate * angle);
            UnkVec3f r = fn_7100078170(reinterpret_cast<const UnkMat4f*>(&m),
                                       reinterpret_cast<const UnkVec3f*>(&self->forward));
            self->forward.x = r.x;
            self->forward.y = r.y;
            self->forward.z = r.z;
            if (!(self->field_0xc04 > 0.0f)) {
                f32 now = fabsf(r.x * 0.0f + r.y * 0.0f - r.z);
                if (now < 0.3f) {
                    f32 before = fabsf(old.x * 0.0f + old.y * 0.0f - old.z);
                    if (now < before)
                        self->forward = old;
                }
            }
        }
    }

    self->forward = NormalizedAxis(self->forward);
    f32 limit = self->field_0x208;
    if (UnkPlayer* player = fn_7100dec788(g_unk_7101d47310)) {
        const CVector3D* at = reinterpret_cast<const CVector3D*>(fn_7100d7c9f4(player));
        f32 d = self->position.z - at->z;
        if (d < limit)
            self->position.z = limit + at->z;
        else if (d > self->field_0x208)
            self->position.z = at->z + self->field_0x208;
    }
    self->field_0x94 = fn_71010ee560(reinterpret_cast<UnkCameraCtrl_ovb0049*>(self));
    (void)one;
}
