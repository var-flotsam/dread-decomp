// CShotLaunchConfig's unit (tools/tu_map.py unit 1509): a candidate ordering (fn_71009f9ea8: primary
// candidates first; then, when the actor has an actor to aim at (fn_7100d7c990), the one nearer to it;
// then the one whose aim point (+0x18) is nearer its position (+0x64); then the higher +0x38; a
// candidate without a live target (+0x50) is infinitely far; distances within 0.001 count as equal)
// and the launch setup (fn_71009fa098: resets the launch state (fn_71009f9dc0), stores the three
// vectors, two flags, a value, the current frame word g_unk_7101d58e48 and the owner; returns true).
// Names and layouts are placeholders.
#include "game/UnkActor.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkShotCandidate_ovb0090 {
    u8 pad_0x0[0x18];
    f32 aim_x;  // 0x18
    f32 aim_y;
    f32 aim_z;
    u8 pad_0x24[0x14];
    u32 order;  // 0x38
    u8 pad_0x3c[0xc];
    bool primary;  // 0x48
    u8 pad_0x49[7];
    UnkWeakHandle* target;  // 0x50
    u8 pad_0x58[0xc];
    f32 pos_x;  // 0x64
    f32 pos_y;
    f32 pos_z;

    bool HasTarget() const { return target && target->target; }
    f32 DistSqTo(f32 x, f32 y, f32 z) const {
        f32 dx = x - pos_x;
        f32 dy = y - pos_y;
        f32 dz = z - pos_z;
        return dx * dx + dy * dy + dz * dz;
    }
};

static inline f32 ActorDistSq(const UnkShotCandidate_ovb0090* c, const UnkActor* actor) {
    return c->HasTarget() ? c->DistSqTo(actor->field_0x44, actor->field_0x54, actor->field_0x64) : 3.40282347e+38f;
}

static inline f32 AimDistSq(const UnkShotCandidate_ovb0090* c) {
    if (!c->HasTarget())
        return 3.40282347e+38f;
    f32 dx = c->aim_x - c->pos_x;
    f32 dy = c->aim_y - c->pos_y;
    f32 dz = c->aim_z - c->pos_z;
    return dx * dx + dy * dy + dz * dz;
}

bool fn_71009f9ea8(const UnkShotCandidate_ovb0090* a, const UnkShotCandidate_ovb0090* b, UnkActor* actor) {
    if (a->primary != b->primary)
        return a->primary;
    if (actor) {
        if (UnkActor* other = fn_7100d7c990(actor)) {
            f32 da = ActorDistSq(a, other);
            f32 db = ActorDistSq(b, other);
            if (__builtin_fabsf(da - db) > 0.001f)
                return da < db;
        }
    }
    f32 da = AimDistSq(a);
    f32 db = AimDistSq(b);
    if (__builtin_fabsf(da - db) > 0.001f)
        return da < db;
    return a->order > b->order;
}

struct UnkShotLaunchState;
void fn_71009f9dc0(UnkShotLaunchState* self);
extern u32 g_unk_7101d58e48;

struct UnkShotLaunch_ovb0090 {
    UnkVec3f origin;     // 0x0
    UnkVec3f target;     // 0xc
    UnkVec3f direction;  // 0x18
    u8 pad_0x24[0xc];
    bool field_0x30;
    bool field_0x31;
    u8 pad_0x32[2];
    f32 field_0x34;
    u32 field_0x38;
    u8 pad_0x3c[4];
    void* owner;  // 0x40
};

bool fn_71009fa098(void* owner, const UnkVec3f* target, const UnkVec3f* direction, const UnkVec3f* origin, bool a,
                   bool b, UnkShotLaunch_ovb0090* out, f32 value) {
    fn_71009f9dc0(reinterpret_cast<UnkShotLaunchState*>(out));
    out->origin = *origin;
    out->target = *target;
    out->direction = *direction;
    out->field_0x30 = a;
    out->field_0x31 = b;
    out->field_0x34 = value;
    out->field_0x38 = g_unk_7101d58e48;
    out->owner = owner;
    return true;
}
