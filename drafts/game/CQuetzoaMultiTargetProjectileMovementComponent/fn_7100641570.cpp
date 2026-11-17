// CQuetzoaMultiTargetProjectileMovementComponent unit (tools/tu_map.py unit 964): the projectile's
// launch. After the base's (fn_7100ac3880) it builds its path: the owner's position, a point along
// the launch direction, a detour of 100 up or down when the target faces away from the projectile,
// and the target point; then registers with the game manager under the class id. Names and layouts
// are placeholders.
#include <new>

#include "base/UnkMatrix44.h"
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "base/math/CVectorOps.h"
#include "game/CProjectileMovement.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::math::CVector3D;

struct UnkSelf;
struct UnkWeakTarget;
// The class id of the unit's statics (fn_71006422e0).
extern __attribute__((visibility("hidden"))) const u64 g_unk_71726fae18;

// A point of the path.
struct UnkPathPoint_l641570 {
    f32 x;
    f32 y;
    f32 z;

    UnkPathPoint_l641570(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkPathPoint_l641570(const UnkPathPoint_l641570& other) : x(other.x), y(other.y), z(other.z) {}
};

typedef base::global::CRntVector<UnkPathPoint_l641570> UnkPathPoints_l641570;

struct UnkQuetzoaActor_l641570 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38: the position is its translation (0x44, 0x54, 0x64)

    UnkPathPoint_l641570 Position() const {
        return UnkPathPoint_l641570(transform.m[3], transform.m[7], transform.m[11]);
    }
};

struct UnkQuetzoaTargetRef_l641570 {
    u8 pad_0x0[0x10];
    UnkQuetzoaActor_l641570* actor;  // 0x10
};

struct UnkQuetzoaParams_l641570 {
    u8 pad_0x0[0x144];
    f32 lead;  // 0x144, how far the second point lies along the launch direction
};

struct UnkQuetzoaProjectile_l641570 {
    u8 pad_0x0[0x30];
    UnkQuetzoaActor_l641570* owner;  // 0x30
    u8 pad_0x38[0x8];
    CVector3D launch_dir;  // 0x40
    u8 pad_0x4c[0x1e4];
    UnkQuetzoaTargetRef_l641570* target;  // 0x230
    u8 pad_0x238[0x398];
    UnkQuetzoaParams_l641570* params;  // 0x5d0
    CVector3D dir;                      // 0x5d8
    UnkPathPoint_l641570 target_pos;    // 0x5e4
    UnkPathPoints_l641570 points;  // 0x5f0
    u32 following;  // 0x618
};
static_assert(__builtin_offsetof(UnkQuetzoaProjectile_l641570, points) == 0x5f0, "layout");
static_assert(__builtin_offsetof(UnkQuetzoaProjectile_l641570, following) == 0x618, "layout");

// Appends a copy of `point`: CRntVector::PushBackCopy, as a helper of the unit's own.
static inline void AddPoint(UnkPathPoints_l641570* points, const UnkPathPoint_l641570& point) {
    u32 n = points->count;
    u32 capacity = points->capacity;
    if (capacity <= n && points->step != 0)
        capacity = points->GrownCapacity(n);
    if ((s32)n >= (s32)capacity)
        for (;;) {
        }
    points->Reserve(capacity);
    new (&points->data[points->count]) UnkPathPoint_l641570(point);
    ++points->count;
}

// -1, 0 or 1.
static inline f32 Sign(f32 v) {
    return (f32)((v > 0.0f) - (v < 0.0f));
}

void fn_7100641570(UnkQuetzoaProjectile_l641570* self) {
    fn_7100ac3880(reinterpret_cast<UnkSelf*>(self));
    self->dir.x = self->launch_dir.x;
    self->dir.y = self->launch_dir.y;
    self->dir.z = self->launch_dir.z;
    self->points.Clear();

    UnkPathPoint_l641570 start = self->owner->Position();
    AddPoint(&self->points, start);
    f32 lead = self->params->lead;
    UnkPathPoint_l641570 ahead(self->dir.x * lead + start.x, self->dir.y * lead + start.y,
                               self->dir.z * lead + start.z);
    AddPoint(&self->points, ahead);

    UnkPathPoint_l641570 from = self->owner->Position();
    CVector3D to_target = CVector3D{self->target_pos.x - from.x, self->target_pos.y - from.y,
                                    self->target_pos.z - from.z};
    f32 side = base::math::Normalized(to_target).x;
    UnkQuetzoaTargetRef_l641570* target = self->target;
    const UnkMatrix44& m = fn_710002614c(&(target ? target->actor : nullptr)->transform);
    if (Sign(m.m[2]) != Sign(side)) {
        UnkPathPoint_l641570 detour(ahead.x + 0.0f, ahead.y + Sign(self->dir.y) * 100.0f, ahead.z + 0.0f);
        AddPoint(&self->points, detour);
    }
    AddPoint(&self->points, self->target_pos);

    self->following = 1;
    fn_7100de9540(g_unk_7101d47310, &g_unk_71726fae18, reinterpret_cast<UnkWeakTarget*>(self));
}
