// CYamplotXAIComponent's unit: on the change from state 2 to state 1, when the landing probe is armed
// probes straight down from the actor (its radius plus 100) and keeps the hit; otherwise clears the
// jump flags, releases the pending collision (by name) and runs the base routine. Names and layouts
// are placeholders.
#include "base/UnkDestructors.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/UnkAssignedSlot.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU365.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkGroundAI_ovb0146;
void fn_7101110030(UnkGroundAI_ovb0146* self);

__attribute__((visibility("hidden"))) extern const base::global::CStrId g_unk_717270c490;

// A reference to a counted object, released when the count drops to zero; assigned out of line.
struct UnkHitRef_l6f201c {
    base::UnkRefCounted* ptr;

    UnkHitRef_l6f201c() : ptr(nullptr) {}
    ~UnkHitRef_l6f201c() {
        if (base::UnkRefCounted* p = ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }
    UnkHitRef_l6f201c& operator=(const UnkHitRef_l6f201c& other) {
        fn_71002a5d1c(reinterpret_cast<UnkAssignedSlot*>(this), reinterpret_cast<const UnkAssignedSlot*>(&other));
        return *this;
    }
};

struct UnkHitVec_l6f201c {
    f32 x, y, z;

    UnkHitVec_l6f201c(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkHitVec_l6f201c& operator=(const UnkHitVec_l6f201c& o) {
        x = o.x;
        y = o.y;
        z = o.z;
        return *this;
    }
};

// A collision hit (0x48 bytes): the object hit and a second reference, an id, the point, two
// vectors and flags.
struct UnkHit_l6f201c {
    UnkHitRef_l6f201c object;
    UnkHitRef_l6f201c field_0x8;
    s32 id;
    UnkHitVec_l6f201c point;
    UnkHitVec_l6f201c field_0x20;
    UnkHitVec_l6f201c normal;
    u32 field_0x38;
    u32 flags;
    u32 field_0x40;

    UnkHit_l6f201c()
        : id(-1), point(0.0f, 0.0f, 0.0f), field_0x20(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), field_0x38(0),
          flags(0), field_0x40(0) {}
    UnkHit_l6f201c& operator=(const UnkHit_l6f201c& o) {
        if (&o == this)
            return *this;
        object = o.object;
        field_0x8 = o.field_0x8;
        id = o.id;
        point = o.point;
        field_0x20 = o.field_0x20;
        normal = o.normal;
        field_0x38 = o.field_0x38;
        flags = o.flags;
        field_0x40 = o.field_0x40;
        return *this;
    }
};
static_assert(sizeof(UnkHit_l6f201c) == 0x48, "UnkHit_l6f201c layout");

// The segment query's callback object: the closest hit so far and the segment's origin. The
// vtables (reached through the GOT) are the callback's and its base's.
struct UnkSegmentQuery_l6f201c {
    void** vtable;
    UnkHit_l6f201c hit;
    bool field_0x50;
    u32 field_0x54;
    CVector3D origin;
    u64 field_0x68;

    explicit UnkSegmentQuery_l6f201c(const CVector3D& o) {
        vtable = g_unk_7101a20fa8 + 2;
        field_0x50 = false;
        origin.x = o.x;
        origin.y = o.y;
        origin.z = o.z;
        field_0x68 = 0;
    }
    ~UnkSegmentQuery_l6f201c() { vtable = g_unk_7101a20ed8 + 2; }
};

// The segment fn_7100d5c518 tests: its two ends and the vector between them.
struct UnkSegment_l6f201c {
    CVector2D start;
    CVector2D end;
    CVector2D delta;

    UnkSegment_l6f201c(const CVector2D& s, const CVector2D& e) {
        start.x = s.x;
        start.y = s.y;
        end.x = e.x;
        end.y = e.y;
        delta.x = e.x - s.x;
        delta.y = e.y - s.y;
    }
};

// The object at the owner's +0x218: a flag at +0xf9 that vtable slot 0x260 acts on when set.
struct UnkYamplotMovement_l6f201c {
    void** vtable;
    u8 pad_0x8[0xf1];
    u8 field_0xf9;

    void vf_0x260() { reinterpret_cast<void (*)(UnkYamplotMovement_l6f201c*)>(vtable[0x260 / 8])(this); }
};

// The object at the owner's +0x1f8: vtable slot 0x2b0 releases a collision object.
struct UnkYamplotCollisions_l6f201c {
    void** vtable;

    void vf_0x2b0(UnkCollisionObject* object) {
        reinterpret_cast<void (*)(UnkYamplotCollisions_l6f201c*, UnkCollisionObject*)>(vtable[0x2b0 / 8])(this,
                                                                                                         object);
    }
};

struct UnkYamplotOwner_l6f201c {
    u8 pad_0x0[0x44];
    f32 x;
    u8 pad_0x48[0xc];
    f32 y;
    u8 pad_0x58[0xc];
    f32 z;
    u8 pad_0x68[0x190];
    UnkYamplotCollisions_l6f201c* field_0x1f8;
    u8 pad_0x200[0x18];
    UnkYamplotMovement_l6f201c* field_0x218;
    u8 pad_0x220[0x80];
    UnkCollisionContainer* field_0x2a0;
    u8 pad_0x2a8[0xd4];
    f32 radius;
};
static_assert(__builtin_offsetof(UnkYamplotOwner_l6f201c, radius) == 0x37c, "UnkYamplotOwner_l6f201c layout");

struct UnkYamplot_l6f201c {
    u8 pad_0x0[0x30];
    UnkYamplotOwner_l6f201c* owner;
    u8 pad_0x38[0x14758 - 0x38];
    u8 field_0x14758;
    u8 pad_0x14759[8];
    u8 field_0x14761;
    u8 pad_0x14762[6];
    u8 field_0x14768;
    u8 field_0x14769;
    u8 pad_0x1476a[6];
    UnkHit_l6f201c field_0x14770;
    u8 field_0x147b8;
    u8 pad_0x147b9[0xb];
    u32 field_0x147c4;
};
static_assert(__builtin_offsetof(UnkYamplot_l6f201c, field_0x147c4) == 0x147c4, "UnkYamplot_l6f201c layout");

// The point `length` along `dir` from `from`.
static inline CVector2D Toward_l6f201c(const CVector2D& from, const CVector2D& dir, f32 length) {
    return CVector2D{from.x + length * dir.x, from.y + length * dir.y};
}

void fn_71006f201c(UnkYamplot_l6f201c* self, u32 from, u32 to) {
    if (from == to)
        return;
    if (from != 2 || to != 1)
        return;
    if (self->field_0x14769) {
        UnkYamplotOwner_l6f201c* owner = self->owner;
        CVector3D pos{owner->x, owner->y, owner->z};
        const CVector2D up{0.0f, 1.0f};
        const CVector2D down{-up.x, -up.y};
        UnkSegment_l6f201c segment(CVector2D{pos.x, pos.y},
                                   Toward_l6f201c(CVector2D{pos.x, pos.y}, down, owner->radius + 100.0f));
        UnkSegmentQuery_l6f201c query(pos);
        UnkTree* tree = static_cast<UnkTree*>(g_unk_7101d47310->field_0x608->field_0x820);
        if (fn_7100d5c518(tree, 0x80409, reinterpret_cast<const UnkBox*>(&segment), &query, 2)) {
            UnkYamplotMovement_l6f201c* movement = self->owner->field_0x218;
            if (movement->field_0xf9 != 1) {
                movement->field_0xf9 = 1;
                movement->vf_0x260();
            }
            self->field_0x14770 = query.hit;
            return;
        }
    }
    self->field_0x14758 = 0;
    self->field_0x14768 = 0;
    self->field_0x14769 = 0;
    self->field_0x14761 = 0;
    self->field_0x147b8 = 0;
    if (self->field_0x147c4 == 1) {
        self->field_0x147c4 = 0;
        UnkCollisionObject* object = fn_71008640a4(self->owner->field_0x2a0, &g_unk_717270c490);
        self->owner->field_0x1f8->vf_0x2b0(object);
    }
    fn_7101110030(reinterpret_cast<UnkGroundAI_ovb0146*>(self));
}
