// CMorphBallMovement's unit (tools/tu_map.py unit 1428): the morph ball leaving its current state. A ball
// still rolling stops (fn_710093efdc) and, unless it was falling fast enough, keeps going; a landing ball is
// swept against the collision tracker along its velocity and moved to where it hits; then the movement is
// reset (direction, input) and the step is marked done (+0x2489). Names and layouts are placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "base/math/CVector4D.h"
#include "game/CAbilityShinespark.h"
#include "game/CCharacterMovement.h"
#include "game/UnkGameManager.h"
#include "game/UnkMatrix44_7100078c9c.h"
#include "game/UnkTU1428.h"
#include "game/UnkTU365.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "game/logic/collision/UnkBvhTree.h"
#include "game/logic/collision/UnkBvhTreeSweep.h"
#include "types.h"

struct UnkMorphBall_fd88;
void fn_710093fd88(UnkMorphBall_fd88* self, const u32* a, const u32* b, f32 speed);

struct UnkInputComponent_l872690;
struct UnkInputValueList_l872690;
void fn_7100872690(UnkInputComponent_l872690* self, void* arg, const UnkInputValueList_l872690* values, bool a,
                   bool b, bool reset, bool clear, f32 dt);

__attribute__((visibility("hidden"))) extern char g_unk_71727394b8[];

struct UnkHitObj_l94c19c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;
};

struct UnkHitRef_l94c19c {
    UnkHitObj_l94c19c* ptr;

    UnkHitRef_l94c19c() : ptr(nullptr) {}
    ~UnkHitRef_l94c19c() {
        if (UnkHitObj_l94c19c* p = ptr) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

struct UnkHitVec_l94c19c {
    f32 x, y, z;
    UnkHitVec_l94c19c(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
};

struct UnkCollisionHit_l94c19c {
    UnkHitRef_l94c19c object;
    UnkHitRef_l94c19c field_0x8;
    s32 id;
    UnkHitVec_l94c19c point;
    UnkHitVec_l94c19c field_0x20;
    UnkHitVec_l94c19c normal;
    u32 field_0x38;
    u32 flags;
    u32 field_0x40;

    UnkCollisionHit_l94c19c()
        : id(-1), point(0.0f, 0.0f, 0.0f), field_0x20(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), field_0x38(0),
          flags(0), field_0x40(0) {}
};

// The hit query the sweep fills.
struct UnkSweepQuery_l94c19c {
    void** vtable;
    UnkCollisionHit_l94c19c hit;
    bool field_0x50;

    UnkSweepQuery_l94c19c() {
        vtable = g_unk_7101a23590 + 2;
        field_0x50 = false;
    }
    ~UnkSweepQuery_l94c19c() { vtable = g_unk_7101a20ed8 + 2; }
};

struct UnkCollider_l94c19c {
    u8 pad_0x0[0x158];
    UnkShape* shape;  // 0x158
};

struct UnkBallOwner_l94c19c {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x1d8 - 0x94];
    UnkInputComponent_l872690* input;  // 0x1d8
    u8 pad_0x1e0[0x3b0 - 0x1e0];
    bool moved;  // 0x3b0
};

struct UnkBallFloor_l94c19c {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkMorphBall_l94c19c {
    u8 pad_0x0[0x30];
    UnkBallOwner_l94c19c* owner;  // 0x30
    u8 pad_0x38[0x58 - 0x38];
    u32 field_0x58[3];
    u8 pad_0x64[0x128 - 0x64];
    UnkBallFloor_l94c19c* floor;  // 0x128
    u8 pad_0x130[0x14c - 0x130];
    f32 vertical_speed;  // 0x14c
    u8 pad_0x150[0x1d8 - 0x150];
    base::math::CVector3D velocity;  // 0x1d8
    u8 pad_0x1e4[0x3b8 - 0x1e4];
    UnkCollider_l94c19c* collider;  // 0x3b8
    UnkCollider_l94c19c* override_collider;  // 0x3c0
    u32 mask;  // 0x3c8
    u8 pad_0x3cc[0x6e8 - 0x3cc];
    UnkVec3f field_0x6e8;
    u8 pad_0x6f4[0x2288 - 0x6f4];
    u32 state;  // 0x2288
    u8 pad_0x228c[0x22b4 - 0x228c];
    f32 field_0x22b4;
    f32 field_0x22b8;
    u8 pad_0x22bc[0x2310 - 0x22bc];
    base::math::CVector4D direction;  // 0x2310
    u8 pad_0x2320[0x23c4 - 0x2320];
    f32 field_0x23c4;
    u8 pad_0x23c8[0x23d0 - 0x23c8];
    f32 field_0x23d0;
    u8 pad_0x23d4[0x23e0 - 0x23d4];
    u8 input_values[0x8];  // 0x23e0: the input values (fn_7100872690)
    u8 pad_0x23e8[0x2489 - 0x23e8];
    bool done;  // 0x2489
    u8 pad_0x248a[0x2538 - 0x248a];
    bool landed;  // 0x2538
    u8 pad_0x2539[0x2634 - 0x2539];
    f32 field_0x2634[4];
};
static_assert(__builtin_offsetof(UnkMorphBall_l94c19c, landed) == 0x2538, "");
static_assert(__builtin_offsetof(UnkMorphBall_l94c19c, field_0x2634) == 0x2634, "");

// Moves the ball to the hit the sweep found.
void fn_710094c51c(UnkMorphBall_l94c19c* self, UnkCollisionHit_l94c19c* hit, UnkVec3f* position);
// The movement's base step.
void fn_71010d8798(UnkMorphBall_l94c19c* self, void* a, void* b);

static inline void ClearLanded(UnkMorphBall_l94c19c* self) {
    if (self->landed)
        self->landed = false;
}

static inline bool IsRolling(u32 state) { return state == 2 || state == 4 || state == 5; }

static inline void Stop(UnkMorphBall_l94c19c* self) {
    fn_710093efdc(reinterpret_cast<UnkMorphBall_eb64*>(self), 1, 1, 1);
}

// Whether the shape placed by `xf` and swept by `step` hits anything in `tree` on the layers of `mask`.
static inline bool Sweep(UnkTree* tree, u32 mask, UnkShape* shape, const UnkMatrix44_7100078c9c& xf,
                         const base::math::CVector2D& step, UnkSweepQuery_l94c19c* query) {
    return fn_7100d5babc(tree, mask, shape, reinterpret_cast<const UnkMtx44*>(&xf), reinterpret_cast<const Vec2*>(&step),
                         query, false);
}

// Sweeps the ball from the owner's position along its velocity and moves the owner to the hit.
static inline void Land(UnkMorphBall_l94c19c* self) {
    UnkVec3f position = self->owner->xform.GetTranslation();
    UnkGameManagerSub_0x608* collision = g_unk_7101d47310->field_0x608;
    UnkTree* tree = collision ? static_cast<UnkTree*>(collision->field_0x820) : nullptr;
    base::math::CVector2D step;
    fn_710007b430(&step, &self->velocity);
    f32 length = sqrtf(step.x * step.x + step.y * step.y);
    if (length != 0.0f) {
        step.x = step.x / length;
        step.y = step.y / length;
    }
    step.x = step.x * 60.0f;
    step.y = step.y * 60.0f;
    UnkCollider_l94c19c* collider = self->override_collider ? self->override_collider : self->collider;
    UnkShape* shape = collider ? collider->shape : nullptr;
    UnkSweepQuery_l94c19c query;
    if (tree && shape && Sweep(tree, self->mask, shape, fn_7100078cd8(position), step, &query)) {
        fn_710094c51c(self, &query.hit, &position);
        UnkBallOwner_l94c19c* owner = self->owner;
        owner->xform.SetTranslation(position);
        owner->moved = true;
    }
}

static inline void Advance(UnkMorphBall_l94c19c* self, void* a, void* b) {
    u32 state = self->state;
    switch (state) {
    case 2:
    case 4:
    case 5:
        if (!self->floor || !self->floor->field_0x10) {
            Stop(self);
            break;
        }
        if (!IsRolling(state)) {
            Stop(self);
            return;
        }
        {
            f32 speed = self->vertical_speed;
            Stop(self);
            if (!(speed <= -0.7f))
                return;
        }
        break;
    case 3:
        Land(self);
        if (self->state == 1)
            ClearLanded(self);
        else
            Stop(self);
        break;
    case 1:
        ClearLanded(self);
        break;
    default:
        Stop(self);
        return;
    }
    self->direction = {0.0f, 0.0f, 0.0f, 0.0f};
    fn_710093fd88(reinterpret_cast<UnkMorphBall_fd88*>(self), reinterpret_cast<const u32*>(&self->direction),
                  self->field_0x58, 0.0f);
    self->field_0x6e8 = UnkVec3f(0.0f, 0.0f, 0.0f);
    fn_71010d8798(self, a, b);
    self->field_0x2634[0] = 0.0f;
    self->field_0x2634[1] = 0.0f;
    self->field_0x2634[2] = 0.0f;
    self->field_0x2634[3] = 0.0f;
    fn_7100872690(self->owner->input, g_unk_71727394b8,
                  reinterpret_cast<const UnkInputValueList_l872690*>(self->input_values), true, true, false, false,
                  self->field_0x22b4);
    self->field_0x23d0 = self->field_0x22b4;
    self->field_0x23c4 = self->field_0x22b8;
}

void fn_710094c19c(UnkMorphBall_l94c19c* self, void* a, void* b) {
    Advance(self, a, b);
    self->done = true;
}
