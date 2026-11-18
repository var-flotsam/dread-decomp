// CEnemyMovement unit: limits a move from `from` to `to`. Unless the owner may ignore the can't-fall
// track, a path past either end stops at the nearer point; otherwise the player's collision is probed
// along the move, and the navigation's rule has the last word. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CLifeComponent.h"
#include "game/CMovementComponent.h"
#include "game/UnkAIComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU365.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkPlayerMovementCache;
void* fn_71010d7a6c(UnkPlayerMovementCache* self);
UnkVec3f fn_71010fc154(base::reflection::UnkReflObject* component);
UnkVec3f fn_71010fc1f4(base::reflection::UnkReflObject* component);
struct UnkLayered_58e24;
struct UnkBox_58e24;
bool fn_7100d58e24(UnkLayered_58e24* obj, const UnkBox_58e24* box, void* arg, u32 flags);

// The path's settings: whether the end may be passed, and the margin kept from it.
struct UnkPathSettings_l0c9574 {
    u8 pad_0x0[0xb0];
    bool open_ends;  // 0xb0
    f32 margin;      // 0xb4
};

// The owner's path component: its points and whether it closes on itself.
struct UnkPath_l0c9574 {
    u8 pad_0x0[0x70];
    u32 count;  // 0x70
    u8 pad_0x74[0x20];
    u8 closed;  // 0x94
    u8 pad_0x95[0x1610 - 0x95];
    UnkPathSettings_l0c9574* settings;  // 0x1610

    bool HasSegment() const {
        bool open = closed == 0;
        u32 n = count;
        return n != ((n != 0) & open);
    }
};

struct UnkEnemyOwner_l0c9574 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x18];
    UnkPath_l0c9574* path;  // 0x240
};

struct UnkEnemyMovement_l0c9574 {
    u8 pad_0x0[0x30];
    UnkEnemyOwner_l0c9574* owner;  // 0x30
    u8 pad_0x38[0x6fc - 0x38];
    bool limited;  // 0x6fc
};

struct UnkPlayer_l0c9574 {
    u8 pad_0x0[0x1b8];
    void* life;  // 0x1b8
    u8 pad_0x1c0[0x40];
    UnkPlayerMovementCache* movement;  // 0x200
};

// The navigation's rule for a move: may change the target and says whether it was left alone.
struct UnkMoveRule_l0c9574 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual bool Adjust(const CVector3D* from, const CVector3D* to, CVector3D* target);  // vf_0x58
};
struct UnkMoveRuleSlot_l0c9574 {
    u8 pad_0x0[0x10];
    UnkMoveRule_l0c9574* rule;  // 0x10
};

struct UnkCounted_l0c9574 {
    virtual void vf_0x0();
    virtual ~UnkCounted_l0c9574();
    s32 ref_count;
};

struct UnkHitRef_l0c9574 {
    UnkCounted_l0c9574* ptr;

    UnkHitRef_l0c9574() : ptr(nullptr) {}
    ~UnkHitRef_l0c9574() {
        if (UnkCounted_l0c9574* p = ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }
};

struct UnkHitVec_l0c9574 {
    f32 x, y, z;

    UnkHitVec_l0c9574(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
};

// A collision hit: the object hit and a second reference, an id, the point, two vectors and flags.
struct UnkHit_l0c9574 {
    UnkHitRef_l0c9574 object;
    UnkHitRef_l0c9574 field_0x8;
    s32 id;
    UnkHitVec_l0c9574 point;
    UnkHitVec_l0c9574 field_0x20;
    UnkHitVec_l0c9574 normal;
    u32 field_0x38;
    u32 flags;
    u32 field_0x40;

    UnkHit_l0c9574()
        : id(-1), point(0.0f, 0.0f, 0.0f), field_0x20(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), field_0x38(0),
          flags(0), field_0x40(0) {}
};

// The segment query's callback object: the closest hit so far and the segment's origin.
struct UnkSegmentQuery_l0c9574 {
    void** vtable;
    UnkHit_l0c9574 hit;
    bool found;
    u32 field_0x54;
    CVector3D origin;
    u64 field_0x68;

    explicit UnkSegmentQuery_l0c9574(const CVector3D& o) {
        vtable = g_unk_7101a20fa8 + 2;
        found = false;
        origin.x = o.x;
        origin.y = o.y;
        origin.z = o.z;
        field_0x68 = 0;
    }
    ~UnkSegmentQuery_l0c9574() { vtable = g_unk_7101a20ed8 + 2; }
};

// The segment probed: its two ends and the vector between them.
struct UnkSegment_l0c9574 {
    CVector2D start;
    CVector2D end;
    CVector2D delta;

    UnkSegment_l0c9574(const CVector2D& s, const CVector2D& e) {
        start.x = s.x;
        start.y = s.y;
        end.x = e.x;
        end.y = e.y;
        delta.x = e.x - s.x;
        delta.y = e.y - s.y;
    }
};

static inline bool IgnoresCantFallTrack(UnkEnemyOwner_l0c9574* owner) {
    UnkAnimationComponent* animation = owner->animation;
    return animation && fn_710117b348(animation, fn_7100d06b98());
}

static inline base::reflection::UnkReflObject* AsComponent(UnkPath_l0c9574* path) {
    return reinterpret_cast<base::reflection::UnkReflObject*>(path);
}

// Stops `target` at the path's end when the move passes it; whether it did.
static inline bool StopAtPathEnd(UnkEnemyMovement_l0c9574* self, const CVector3D* from, const CVector3D* to,
                                 CVector3D* target) {
    if (IgnoresCantFallTrack(self->owner))
        return false;
    UnkPath_l0c9574* path = self->owner->path;
    if (!path || !path->settings || path->settings->open_ends ||
        fn_71010d0e38(reinterpret_cast<UnkMovement_71010d0e38*>(self)) || !path->HasSegment())
        return false;
    UnkPath_l0c9574* current = self->owner->path;
    f32 margin = 0.0f;
    if (current && current->settings)
        margin = current->settings->margin;
    f32 dx = to->x - from->x;
    if (dx > 0) {
        UnkVec3f end = fn_71010fc154(AsComponent(current));
        end.x += margin;
        if (!(to->x > end.x))
            return false;
        if (to->x > from->x) {
            target->x = from->x;
            target->y = from->y;
            target->z = from->z;
        } else {
            target->x = to->x;
            target->y = to->y;
            target->z = to->z;
        }
        return true;
    }
    if (dx < 0) {
        UnkVec3f start = fn_71010fc1f4(AsComponent(current));
        start.x -= margin;
        if (!(to->x < start.x))
            return false;
        if (to->x < from->x) {
            target->x = from->x;
            target->y = from->y;
            target->z = from->z;
        } else {
            target->x = to->x;
            target->y = to->y;
            target->z = to->z;
        }
        return true;
    }
    return false;
}

// Moves `target` back to where the player's collision cuts the move from `from`.
static inline void StopAtPlayer(const CVector3D* from, CVector3D* target) {
    UnkPlayer_l0c9574* player = reinterpret_cast<UnkPlayer_l0c9574*>(fn_7100dec788(g_unk_7101d47310));
    if (!player || !player->life || fn_7101161088(player->life) || !player->movement)
        return;
    void* body = fn_71010d7a6c(player->movement);
    if (!body)
        return;
    CVector2D wanted = {target->x, target->y};
    CVector2D start;
    fn_710007b430(&start, from);
    CVector2D end;
    fn_710007b430(&end, target);
    UnkSegment2D line(start, end);
    UnkSegment_l0c9574 segment(start, end);
    UnkSegmentQuery_l0c9574 query(CVector3D{start.x, start.y, 0.0f});
    if (fn_7100d58e24(static_cast<UnkLayered_58e24*>(body), reinterpret_cast<const UnkBox_58e24*>(&segment), &query,
                      2)) {
        if (query.found) {
            target->x = query.hit.point.x;
            target->y = query.hit.point.y;
        } else {
            target->x = wanted.x;
            target->y = wanted.y;
        }
        target->z = from->z;
    }
}

bool fn_71010c9574(UnkEnemyMovement_l0c9574* self, const CVector3D* from, const CVector3D* to, CVector3D* out) {
    if (!self->limited)
        return true;
    CVector3D target;
    target.x = to->x;
    target.y = to->y;
    target.z = to->z;

    bool unchanged = false;
    if (!StopAtPathEnd(self, from, to, &target)) {
        StopAtPlayer(from, &target);
        unchanged = true;
        if (UnkPath_l0c9574* path = self->owner->path) {
            if (UnkNavigation* navigation = fn_71010fccf8(reinterpret_cast<UnkAIComponent*>(path))) {
                UnkMoveRuleSlot_l0c9574* slot = *static_cast<UnkMoveRuleSlot_l0c9574**>(navigation->field_0xb0);
                if (slot && slot->rule)
                    unchanged = slot->rule->Adjust(from, to, &target);
            }
        }
    }
    out->x = target.x;
    out->y = target.y;
    out->z = target.z;
    return unchanged;
}
