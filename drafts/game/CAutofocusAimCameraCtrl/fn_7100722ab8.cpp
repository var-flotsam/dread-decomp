// CAutofocusAimCameraCtrl unit: the camera's per-frame update. Aims the camera at the player (the aim object's
// offset along the player's forward axis, then a goal point from the camera definition), blends the position
// and direction toward the slide target while the intro runs, flattens the direction and tells the render
// manager when it crosses the 0.75 mark of the forward axis, pulls the position toward the line of sight within
// the tunable's angle, and when the blend is over and a switch is pending tells the camera holder. Names and
// layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/math/CVector4D.h"
#include "base/math/CVectorOps.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CCameraComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU315.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector3D;
using base::math::CVector4D;

extern base::reflection::UnkTunable g_unk_717270f910;
extern const u64 g_unk_710164a910 __attribute__((visibility("hidden")));
extern const CVector3D g_unk_710164a918 __attribute__((visibility("hidden")));
extern const CVector3D g_unk_710164a924 __attribute__((visibility("hidden")));
extern const CVector4D g_unk_7101a6d028 __attribute__((visibility("hidden")));
extern const CVector4D g_unk_7101a6d038 __attribute__((visibility("hidden")));
void CAutofocusAimCameraCtrl_RemoveAllFromCameraList(void* holder);

struct UnkAutofocusTunable_l722ab8 {
    u8 pad_0x0[0x24];
    f32 duration;  // 0x24
};

struct UnkAutofocusDef_l722ab8 {
    u8 pad_0x0[0xe4];
    f32 aim_angle;  // 0xe4
};

struct UnkAutofocusCamera_l722ab8 {
    u8 pad_0x0[0x10];
    UnkAutofocusDef_l722ab8* def;  // 0x10
};

struct UnkAutofocusPlayer_l722ab8 {
    u8 pad_0x0[0x38];
    UnkMatrix44 xform;  // 0x38
    u8 pad_0x78[0x2b0 - 0x78];
    void* aim;  // 0x2b0
};

// A camera view: where it is and where it looks.
struct UnkAutofocusView_l722ab8 {
    CVector3D pos;
    UnkVec3f dir;
};

struct UnkAutofocusSlideView_l722ab8 {
    UnkVec3f pos;
    UnkVec3f dir;
};

struct UnkAutofocusCtrl_l722ab8;
struct UnkAutofocusSlide_l722ab8 {
    u8 pad_0x0[0x38];
    UnkAutofocusSlideView_l722ab8 view;  // 0x38
};

struct UnkAutofocusCtrl_l722ab8 {
    void** vtable;
    u8 pad_0x8[0x30];
    UnkAutofocusView_l722ab8 view;         // 0x38
    u8 pad_0x50[0x18];
    UnkAutofocusView_l722ab8 target;       // 0x68
    u8 pad_0x80[0x30];
    UnkAutofocusCamera_l722ab8* camera;    // 0xb0
    u8 pad_0xb8[0x120];
    f32 elapsed;                           // 0x1d8
    UnkVec3f slide_dir;                    // 0x1dc
    UnkVec3f slide_pos;                    // 0x1e8
    f32 field_0x1f4;
    f32 field_0x1f8;
    f32 field_0x1fc;
    f32 field_0x200;
    UnkVec3f goal;                         // 0x204
    u8 pad_0x210[0x30];
    bool pending;                          // 0x240

    void vf_0x48(bool a, f32 dt) {
        reinterpret_cast<void (*)(UnkAutofocusCtrl_l722ab8*, bool, f32)>(vtable[0x48 / 8])(this, a, dt);
    }
};
static_assert(__builtin_offsetof(UnkAutofocusCtrl_l722ab8, pending) == 0x240, "layout");
static_assert(__builtin_offsetof(UnkAutofocusCtrl_l722ab8, goal) == 0x204, "layout");

f32 fn_7100345464(void* aim, u32 flags);
bool fn_71010e6680(UnkAutofocusDef_l722ab8* def, const CVector3D* pos, const CVector3D* offset, f32* a, f32* b, f32* c);
UnkAutofocusSlide_l722ab8* fn_71010ee8bc(UnkAutofocusCtrl_l722ab8* self, f32 dt);

static inline char* IdTable() {
    return reinterpret_cast<char*>(&base::reflection::refl_GetSingleton());
}

inline f32 Blend(f32 from, f32 to, f32 t) {
    return (1.0f - t) * from + t * to;
}

void fn_7100722ab8(UnkAutofocusCtrl_l722ab8* self, f32 dt) {
    if (!self->camera || !self->camera->def)
        return;
    const UnkAutofocusTunable_l722ab8* tunable = reinterpret_cast<const UnkAutofocusTunable_l722ab8*>(&g_unk_717270f910);
    f32 t = self->elapsed / tunable->duration;
    UnkAutofocusPlayer_l722ab8* player = reinterpret_cast<UnkAutofocusPlayer_l722ab8*>(fn_7100dec788(g_unk_7101d47310));
    if (player) {
        const CVector3D down = {0.0f, 0.0f, -1.0f};
        CVector3D pos = {player->xform.m[3], player->xform.m[7], player->xform.m[11]};
        if (player->aim) {
            f32 reach = fn_7100345464(player->aim, 0);
            const UnkMatrix44& m = fn_710002614c(&player->xform);
            pos.x = reach * m.m[2] + pos.x;
            pos.y = reach * m.m[6] + pos.y;
            pos.z = reach * m.m[10] + pos.z;
        }
        const CVector3D* side_offset = self->target.dir.x > 0.0f ? &g_unk_710164a918 : &g_unk_710164a924;
        CVector3D offset = *side_offset;
        UnkAutofocusDef_l722ab8* def = self->camera ? self->camera->def : nullptr;
        f32 side;
        if (fn_71010e6680(def, &pos, &offset, &self->field_0x1f8, &self->field_0x1fc, &self->field_0x200)) {
            self->field_0x1f4 = self->field_0x1f8;
            side = self->field_0x1f8;
        } else {
            side = self->field_0x1f4;
        }
        self->goal.x = offset.x * side + pos.x;
        self->goal.y = offset.y * side + pos.y;
        self->goal.z = offset.z * side + pos.z + 150.0f;
        f32 before = base::math::Dot(CVector3D{self->view.dir.x, self->view.dir.y, self->view.dir.z}, down);
        if (self->pending) {
            UnkAutofocusSlide_l722ab8* slide = fn_71010ee8bc(self, dt);
            if (slide) {
                UnkVec3f pos = slide->view.pos;
                UnkVec3f dir = slide->view.dir;
                self->slide_pos = pos;
                self->slide_dir = dir;
                if (t < 1.0f) {
                    self->view.pos.x = Blend(self->view.pos.x, pos.x, t);
                    self->view.pos.y = Blend(self->view.pos.y, pos.y, t);
                    self->view.pos.z = Blend(self->view.pos.z, pos.z, t);
                    self->view.dir.x = Blend(self->view.dir.x, dir.x, t);
                    self->view.dir.y = Blend(self->view.dir.y, dir.y, t);
                    self->view.dir.z = Blend(self->view.dir.z, dir.z, t);
                } else {
                    self->view.pos.x = pos.x;
                    self->view.pos.y = pos.y;
                    self->view.pos.z = pos.z;
                    self->view.dir = dir;
                }
            }
        } else {
            self->vf_0x48(true, dt);
            if (t < 1.0f) {
                self->view.pos.x = Blend(self->view.pos.x, self->target.pos.x, t);
                self->view.pos.y = Blend(self->view.pos.y, self->target.pos.y, t);
                self->view.pos.z = Blend(self->view.pos.z, self->target.pos.z, t);
                self->view.dir.x = Blend(self->view.dir.x, self->target.dir.x, t);
                self->view.dir.y = Blend(self->view.dir.y, self->target.dir.y, t);
                self->view.dir.z = Blend(self->view.dir.z, self->target.dir.z, t);
            } else {
                self->view.pos = self->target.pos;
                self->view.dir = self->target.dir;
            }
        }
        CVector3D flat = {self->view.dir.x, 0.0f, self->view.dir.z};
        self->view.dir.y = 0.0f;
        f32 inv = base::math::InvLengthEps(flat);
        self->view.dir.x = self->view.dir.x * inv;
        self->view.dir.y = inv * self->view.dir.y;
        self->view.dir.z = inv * self->view.dir.z;
        f32 after = base::math::Dot(CVector3D{self->view.dir.x, self->view.dir.y, self->view.dir.z}, down);
        const CVector4D* colour = nullptr;
        if (before > 0.75f && after <= 0.75f)
            colour = &g_unk_7101a6d028;
        else if (before <= 0.75f && after > 0.75f)
            colour = &g_unk_7101a6d038;
        if (colour)
            fn_710021a0b4(g_unk_7101d47300, &g_unk_710164a910, colour);
        f32 dx = self->goal.x - self->view.pos.x;
        f32 dz = self->goal.z - self->view.pos.z;
        CVector3D to_goal = base::math::Normalized(CVector3D{dx, 0.0f, dz});
        CVector3D look = base::math::Normalized(CVector3D{self->view.dir.x, self->view.dir.y, self->view.dir.z});
        f32 cosine = base::math::Dot(to_goal, look);
        cosine = !(cosine >= 1.0f) ? cosine : 1.0f;
        cosine = !(cosine < -1.0f) ? cosine : -1.0f;
        f32 angle = acosf(cosine);
        f32 limit = static_cast<f32>(static_cast<f64>(self->camera->def->aim_angle * 0.5f) * 3.14159265358979323846 / 180.0);
        if (angle < limit) {
            f32 weight = 1.0f - angle / limit;
            CVector3D dir = {self->view.dir.x, self->view.dir.y, self->view.dir.z};
            f32 along = base::math::Dot(CVector3D{dx, 0.0f, dz}, dir) / base::math::Dot(dir, dir);
            f32 off_x = dir.x * along - dx;
            f32 off_z = dir.z * along - dz;
            self->view.pos.x = self->view.pos.x + weight * off_x;
            self->view.pos.z = self->view.pos.z + weight * off_z;
        }
    }
    if (t > 1.0f && self->pending) {
        void* holder = fn_7100defcec(g_unk_7101d47310);
        CAutofocusAimCameraCtrl_RemoveAllFromCameraList(holder);
        fn_71010e4168(holder, reinterpret_cast<const CStrId*>(IdTable() + 0x17e0), false);
    }
    self->elapsed = self->elapsed + dt;
}
