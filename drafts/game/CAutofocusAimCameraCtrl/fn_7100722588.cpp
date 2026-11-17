// CAutofocusAimCameraCtrl unit (tools/tu_map.py, unit 1150): the aim camera's update. It places the camera
// beside the player at the Metroid camera's height and the tunable's distance, slides it along until the
// player fits the view (at most eight steps), and when something is in front pulls it in by up to 550 along
// its direction. Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

extern base::reflection::UnkTunable g_unk_717270f910;

struct UnkBeamVec_ovb0138;
void fn_710034f54c(void* object, u32 id, UnkBeamVec_ovb0138* dir);

struct UnkAimCamera_l722588;
void fn_71010edff0(UnkAimCamera_l722588* self, bool flag);
f32 fn_7100345464(void* animation, bool flag);
// Whether the view from `origin` along `dir` is blocked; the distances go to the three outputs.
bool fn_71010e6680(void* camera, const CVector3D* origin, const CVector3D* dir, f32* distance, f32* a, f32* b);

struct UnkAutofocusTunable_l722588 {
    u8 pad_0x0[0x68];
    f32 distance;  // 0x68
    f32 height;    // 0x6c
    f32 pitch;     // 0x70, in degrees
};

// The Metroid camera the aim camera follows.
struct UnkMetroidCamera_l722588 {
    virtual const base::reflection::CClass* vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48(bool flag, f32 time);

    u8 pad_0x8[0x230];
    f32 x;  // 0x238
    f32 y;  // 0x23c
    u8 pad_0x240[0x14];
    f32 z;  // 0x254

    inline f32 TargetX();
    inline f32 TargetY();
};
static_assert(__builtin_offsetof(UnkMetroidCamera_l722588, z) == 0x254, "layout");

UnkVec3f fn_71007649ec(UnkMetroidCamera_l722588* camera);

// Where the camera aims: its position plus its offset.
f32 UnkMetroidCamera_l722588::TargetX() {
    UnkVec3f offset = fn_71007649ec(this);
    return x + offset.x;
}

f32 UnkMetroidCamera_l722588::TargetY() {
    UnkVec3f offset = fn_71007649ec(this);
    return y + offset.y;
}

struct UnkAimCameraView_l722588 {
    u8 pad_0x0[0xe4];
    f32 fov;  // 0xe4, in degrees
};

struct UnkAimCameraHolder_l722588 {
    u8 pad_0x0[0x10];
    UnkAimCameraView_l722588* view;  // 0x10
};

struct UnkMetroidCameraHolder_l722588 {
    u8 pad_0x0[0x10];
    UnkMetroidCamera_l722588* camera;  // 0x10
};

struct UnkAimCamera_l722588 {
    u8 pad_0x0[0x68];
    UnkVec3f pos;  // 0x68
    UnkVec3f dir;  // 0x74
    u8 pad_0x80[0x30];
    UnkAimCameraHolder_l722588* holder;  // 0xb0
    u8 pad_0xb8[0x120];
    f32 elapsed;  // 0x1d8
    u8 pad_0x1dc[0x18];
    f32 raw_distance;  // 0x1f4
    f32 distance;      // 0x1f8
    f32 field_0x1fc;
    f32 field_0x200;
    u8 pad_0x204[0x24];
    UnkMetroidCameraHolder_l722588* metroid;  // 0x228
    f32 lead;                                 // 0x230
    u8 pad_0x234[0x4];
    f32 last_z;  // 0x238
};
static_assert(__builtin_offsetof(UnkAimCamera_l722588, metroid) == 0x228, "layout");
static_assert(__builtin_offsetof(UnkAimCamera_l722588, last_z) == 0x238, "layout");

struct UnkAimPlayer_l722588 {
    u8 pad_0x0[0x38];
    u8 transform[0xc];  // 0x38
    f32 x;              // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x248];
    void* animation;  // 0x2b0
};
static_assert(__builtin_offsetof(UnkAimPlayer_l722588, animation) == 0x2b0, "layout");

static inline CVector3D Position(UnkAimPlayer_l722588* player) {
    CVector3D v;
    v.x = player->x;
    v.y = player->y;
    v.z = player->z;
    return v;
}

// A vector's x and y.
static inline void Project(CVector2D* out, const CVector3D& v) {
    fn_710007b430(out, &v);
}

static inline f32 ToRadians(f32 degrees) { return (f32)(degrees * 3.141592653589793 / 180.0); }

// The player's facing axis: the third column of its transform.
static inline CVector3D Facing(UnkAimPlayer_l722588* player) {
    const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(player->transform));
    CVector3D v;
    v.x = m.m[2];
    v.y = m.m[6];
    v.z = m.m[10];
    return v;
}

void fn_7100722588(UnkAimCamera_l722588* self, bool flag, f32 dt) {
    if (!self->holder || !self->holder->view)
        return;
    fn_71010edff0(self, flag);
    auto* player = reinterpret_cast<UnkAimPlayer_l722588*>(fn_7100dec788(g_unk_7101d47310));
    if (!player || !self->metroid)
        return;
    UnkMetroidCamera_l722588* camera = self->metroid->camera;
    if (!camera)
        return;
    f32 fov = self->holder->view->fov;
    if (!fn_710009d50c(camera->vf_0x0(), fn_7100932fa4()))
        return;
    f32 half_fov = ToRadians(fov) * 0.5f;
    camera->vf_0x48(true, 0.0f);

    const auto* tunable = reinterpret_cast<const UnkAutofocusTunable_l722588*>(&g_unk_717270f910);
    const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(player->transform));
    f32 facing = m.m[2];
    CVector3D column = Facing(player);
    if (player->animation)
        fn_710034f54c(player->animation, 0x7fffffff, reinterpret_cast<UnkBeamVec_ovb0138*>(&column));

    // Where the player is on the plane, led along its facing while moving.
    CVector2D target;
    Project(&target, Position(player));
    if (player->animation) {
        if (self->elapsed < 0.6f)
            self->lead = fn_7100345464(player->animation, false);
        CVector2D lead;
        Project(&lead, Facing(player));
        target.x = self->lead * lead.x + target.x;
        target.y = self->lead * lead.y + target.y;
    }

    UnkVec3f pos;
    pos.x = camera->TargetX();
    f32 camera_z = camera->z;
    f32 distance = tunable->distance;
    pos.y = camera->TargetY() + tunable->height;

    // The distance eases towards the tunable's by the camera's movement.
    f32 t = dt * 1000.0f * fabsf(camera_z - self->last_z);
    if (t >= 1.0f)
        pos.z = distance;
    else if (t < 0.0f)
        pos.z = camera_z;
    else
        pos.z = distance * t + camera_z * (1.0f - t);

    f32 side = facing >= 0.0f ? -150.0f : 150.0f;
    f32 c;
    f32 s;
    u32 i = 0;
    do {
        c = cosf(facing * ToRadians(-tunable->pitch) + 1.57f);
        s = sqrtf(1.0f - c * c);
        f32 angle = half_fov * facing + acosf(c);
        f32 reach = distance / sinf(angle) * cosf(angle);
        f32 gap = side + (target.x - pos.x);
        f32 have = facing * gap;
        f32 need = facing * reach;
        if (!(have < need) || !UNK_CHANGED(have, need))
            break;
        pos.x += gap - reach;
    } while (++i < 8);

    void* view = self->holder ? self->holder->view : nullptr;
    CVector3D origin;
    origin.x = target.x;
    origin.y = target.y;
    origin.z = 0.0f;
    CVector3D ahead;
    ahead.x = facing;
    ahead.y = 0.0f;
    ahead.z = 0.0f;
    f32 nz = -s;
    if (fn_71010e6680(view, &origin, &ahead, &self->distance, &self->field_0x1fc, &self->field_0x200)) {
        f32 d = self->distance;
        self->raw_distance = d;
        if (d < 430.0f) {
            pos.x -= facing * (430.0f - d);
            d = 430.0f;
            self->distance = 430.0f;
        }
        d = d - 430.0f;
        self->distance = d;
        if (d <= 2100.0f) {
            f32 pull = (d / -2100.0f + 1.0f) * 550.0f;
            pos.x = pos.x + c * pull;
            pos.y = pos.y + pull * 0.0f;
            pos.z = pos.z + pull * nz;
            pos.x = pos.x - pull * (facing * sinf(ToRadians(tunable->pitch)));
        }
    }
    self->pos = pos;
    self->dir = UnkVec3f(c, 0.0f, nz);
    self->last_z = distance;
}
