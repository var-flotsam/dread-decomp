// CSamusFloorSlideMovementState (unit 1459): after the base update (fn_71009b1e08), while sliding (states 5
// and 6) and the movement allows it (fn_71010d15ac), sweeps a box the size of the slide up from the bottom of
// the body; a hit on a surface that is not a floor and that the movement heads into ends the slide (state 6,
// blend 0). In state 6 the pose is blended by +0x1f0. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU365.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "game/logic/collision/CAABoxShape2D.h"
#include "game/logic/collision/UnkBvhTree.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

bool fn_7100d5babc(UnkTree* self, u32 mask, UnkShape* shape, const UnkMtx44* xf, const Vec2* delta, void* arg, bool flag);

struct UnkMatrix44_7100078c9c {
    f32 m[16];
};

struct UnkCeilingHitObj_l99245c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;
};

struct UnkCeilingHitRef_l99245c {
    UnkCeilingHitObj_l99245c* ptr;

    UnkCeilingHitRef_l99245c() : ptr(nullptr) {}
    ~UnkCeilingHitRef_l99245c() {
        if (UnkCeilingHitObj_l99245c* p = ptr) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

struct UnkCeilingHit_l99245c {
    UnkCeilingHitRef_l99245c object;
    UnkCeilingHitRef_l99245c field_0x8;
    s32 id;
    UnkVec3f point;
    UnkVec3f field_0x20;
    UnkVec3f normal;
    u32 field_0x38;
    u32 flags;
    u32 field_0x40;

    UnkCeilingHit_l99245c()
        : id(-1), point(0.0f, 0.0f, 0.0f), field_0x20(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), field_0x38(0),
          flags(0), field_0x40(0) {}
};
static_assert(sizeof(UnkCeilingHit_l99245c) == 0x48, "UnkCeilingHit_l99245c layout");

// The sweep's callback object: the closest hit so far and the sweep's origin.
struct UnkCeilingQuery_l99245c {
    void** vtable;
    UnkCeilingHit_l99245c hit;
    bool found;  // 0x50
    u32 field_0x54;
    CVector3D origin;
    u64 field_0x68;

    explicit UnkCeilingQuery_l99245c(const CVector3D& o) {
        vtable = g_unk_7101a20fa8 + 2;
        found = false;
        origin = o;
        field_0x68 = 0;
    }
    ~UnkCeilingQuery_l99245c() { vtable = g_unk_7101a20ed8 + 2; }
};

// The swept axis-aligned box.
struct UnkCeilingBox_l99245c {
    void** vtable;
    u8 pad_0x8[0xa0];
    f32 field_0xa8;
    f32 field_0xac;
    f32 field_0xb0;
    f32 field_0xb4;
    u8 pad_0xb8[0x8];

    UnkCeilingBox_l99245c() { fn_7100d5e1b4(this); }
    ~UnkCeilingBox_l99245c() { fn_7100d5e2c8(reinterpret_cast<UnkCollisionShape*>(this)); }
    void vf_0x80() { reinterpret_cast<void (*)(UnkCeilingBox_l99245c*)>(vtable[0x80 / 8])(this); }
};

void fn_7100d5e3dc(UnkCeilingBox_l99245c* box, f32 scale);

struct UnkCeilingOwner_l99245c {
    void** vtable;
    u8 pad_0x8[0x30];
    UnkXform xform;  // 0x38
};

struct UnkCeilingMovement_l99245c {
    u8 pad_0x0[0x10c];
    f32 dir_x;  // 0x10c
    f32 dir_y;
    f32 dir_z;
    u8 pad_0x118[0x2b4];
    u32 mask;  // 0x3cc
};
static_assert(__builtin_offsetof(UnkCeilingMovement_l99245c, mask) == 0x3cc, "layout");

struct UnkCeilingState_l99245c {
    u8 pad_0x0[0x30];
    UnkCeilingOwner_l99245c* owner;        // 0x30
    UnkCeilingMovement_l99245c* movement;  // 0x38
    u8 pad_0x40[0x14c];
    f32 offset;  // 0x18c
    f32 height;  // 0x190
    f32 width;   // 0x194
    s32 state;   // 0x198
    u8 pad_0x19c[0x54];
    f32 blend;  // 0x1f0
};
static_assert(__builtin_offsetof(UnkCeilingState_l99245c, blend) == 0x1f0, "layout");

struct UnkCeilingAnchor_l99245c {
    f32 x;
    f32 y;
};

void fn_71009b1e08(UnkCeilingState_l99245c* self, void* a1, void* a2, const UnkCeilingAnchor_l99245c* anchor, void* a4,
                   UnkMatrix44* pose);
bool fn_71010d15ac(UnkCeilingMovement_l99245c* movement);

void fn_710099245c(UnkCeilingState_l99245c* self, void* a1, void* a2, const UnkCeilingAnchor_l99245c* anchor, void* a4,
                   UnkMatrix44* pose) {
    fn_71009b1e08(self, a1, a2, anchor, a4, pose);
    if (self->state != 5 && self->state != 6)
        return;
    if (!fn_71010d15ac(self->movement)) {
        UnkCeilingOwner_l99245c* owner = self->owner;
        CVector3D bottom = {owner->xform.m[0][3], owner->xform.m[1][3], owner->xform.m[2][3]};
        bottom.y = self->offset + bottom.y + self->height * -0.5f;
        UnkGameManagerSub_0x608* trackers = g_unk_7101d47310->field_0x608;
        UnkTree* tree = trackers ? static_cast<UnkTree*>(trackers->field_0x820) : nullptr;
        if (tree) {
            UnkMatrix44_7100078c9c xf = fn_7100078cd8(reinterpret_cast<const UnkVec3f&>(bottom));
            UnkCeilingBox_l99245c box;
            f32 width = self->width;
            box.field_0xa8 = width;
            box.field_0xac = width;
            box.field_0xb0 = width;
            box.field_0xb4 = width;
            box.vf_0x80();
            fn_7100d5e3dc(&box, 0.999f);
            CVector2D sweep;
            sweep.x = 0.0f;
            sweep.y = self->height * 0.5f + self->width;
            UnkCeilingQuery_l99245c query(bottom);
            if (fn_7100d5babc(tree, self->movement->mask, reinterpret_cast<UnkShape*>(&box),
                              reinterpret_cast<const UnkMtx44*>(&xf), reinterpret_cast<const Vec2*>(&sweep), &query,
                              false)) {
                // A surface facing straight down is the ceiling the slide fits under.
                const UnkVec3f& n = query.hit.normal;
                f32 down = n.x * 0.0f + n.y * -1.0f + n.z * 0.0f;
                bool ceiling = down - 0.001f <= 1.0f && down + 0.001f >= 1.0f;
                UnkCeilingMovement_l99245c* movement = self->movement;
                if ((query.hit.flags & 0x10) && !ceiling &&
                    n.x * movement->dir_x + n.y * movement->dir_y + n.z * movement->dir_z < 0.0f) {
                    self->state = 6;
                    self->blend = 0.0f;
                }
            }
        }
    }
    if (self->state == 6) {
        pose->m[5] = self->blend * pose->m[5];
        pose->m[8] = self->blend * pose->m[8];
        pose->m[14] = anchor->y + (pose->m[14] - anchor->y) * self->blend;
    }
}
