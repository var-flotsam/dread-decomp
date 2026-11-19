// CFulmiteBellyMineAIComponent unit (tools/tu_map.py unit 826): a contact with a polygon edge. A floor edge
// (normal along the owner's up axis) or a wall edge facing the mine's direction starts a climb onto it, unless
// the contact is where the last one of that kind was; then the base handling, the climbing flag on the
// component and the follow-up. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkPolygonCollection2D;
UnkSegment2D fn_710012436c(const UnkPolygonCollection2D* self, u32 handle);

struct UnkMineContact_l57e278;
struct UnkMineAI_l57e278;
void fn_710057e7c4(UnkMineAI_l57e278* self, UnkMineContact_l57e278* contact);
void fn_710057e5cc(UnkMineAI_l57e278* self, UnkMineContact_l57e278* contact);
void fn_71011065a8(UnkMineAI_l57e278* self, UnkMineContact_l57e278* contact);
void fn_710057eb64(UnkMineAI_l57e278* self);

struct UnkMineShape_l57e278 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual s32 GetKind();  // 0x18
};

// The shape of kind 0x10: a polygon collection.
struct UnkMinePolygonShape_l57e278 : UnkMineShape_l57e278 {
    u8 pad_0x8[0xa0];
    u8 polygons[1];  // 0xa8: an UnkPolygonCollection2D
};

static inline UnkMinePolygonShape_l57e278* AsPolygons(UnkMineShape_l57e278* shape) {
    return shape->GetKind() == 0x10 ? static_cast<UnkMinePolygonShape_l57e278*>(shape) : nullptr;
}

struct UnkMineCollider_l57e278 {
    u8 pad_0x0[0x158];
    UnkMineShape_l57e278* shape;  // 0x158
};

struct UnkMineCollision_l57e278 {
    u8 pad_0x0[0x10];
    UnkMineCollider_l57e278* collider;  // 0x10
};

struct UnkMineContact_l57e278 {
    UnkMineCollision_l57e278* collision;
    u8 pad_0x8[0x8];
    u32 edge;          // 0x10
    CVector3D point;   // 0x14
};

struct UnkMineOwner_l57e278 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
};

struct UnkMineComponent_l57e278 {
    virtual void vf_0x0();
    u8 pad_0x8[0x1c08];
    bool climbing;  // 0x1c10

    void OnLost(UnkMineContact_l57e278* contact) {
        reinterpret_cast<void (*)(UnkMineComponent_l57e278*, UnkMineContact_l57e278*)>(
            (*reinterpret_cast<void***>(this))[0x218 / 8])(this, contact);
    }
};

struct UnkMineTuning_l57e278 {
    u8 pad_0x0[0x538];
    u32 max_retries;  // 0x538
};

// A remembered contact point.
struct UnkMineSpot_l57e278 {
    bool set;
    f32 x;
    f32 y;
};

static inline bool IsEqual(f32 a, f32 b, f32 tolerance) {
    return a - tolerance <= b && a + tolerance >= b;
}

static inline bool IsAt(const UnkMineSpot_l57e278& spot, const CVector2D& p) {
    return IsEqual(spot.x, p.x, 5.0f) && IsEqual(spot.y, p.y, 5.0f) && spot.set;
}

// A vector's part along the transform's second column.
static inline f32 UpDot(const CVector3D& v, const UnkMatrix44& m) {
    return v.x * m.m[1] + v.y * m.m[5] + v.z * m.m[9];
}

struct UnkMineState_l57e278 {
    UnkMineComponent_l57e278* component;  // 0x146c8
    UnkMineTuning_l57e278* tuning;        // 0x146d0
    u8 pad_0x10[0x23];
    bool lost;  // 0x146fb
    u8 pad_0x34[0x14];
    CVector2D direction;  // 0x14710
    u32 retries;          // 0x14718
    UnkMineSpot_l57e278 floor;  // 0x1471c
    UnkMineSpot_l57e278 wall;   // 0x14728
};

struct UnkMineAI_l57e278 {
    u8 pad_0x0[0x30];
    UnkMineOwner_l57e278* owner;  // 0x30
    u8 pad_0x38[0x146c8 - 0x38];
    UnkMineState_l57e278 state;  // 0x146c8
};
static_assert(__builtin_offsetof(UnkMineAI_l57e278, state) == 0x146c8, "layout");
static_assert(__builtin_offsetof(UnkMineState_l57e278, wall) == 0x60, "layout");

// Starts a climb onto the contact's edge; false when there is none to start.
static inline bool TryClimb(UnkMineAI_l57e278* self, UnkMineContact_l57e278* contact) {
    const f32 eps = 0.01f;
    UnkMineState_l57e278& state = self->state;
    UnkMinePolygonShape_l57e278* shape = AsPolygons(contact->collision->collider->shape);
    if (!shape)
        return false;
    UnkSegment2D edge =
        fn_710012436c(reinterpret_cast<const UnkPolygonCollection2D*>(shape->polygons), contact->edge);
    if (!(fn_7100125e78(&edge).y > 0.0f || IsEqual(fn_7100125e78(&edge).y, 0.0f, eps)))
        return false;
    f32 up;
    {
        UnkVec2Result n = fn_7100125e78(&edge);
        CVector3D normal3 = {n.x, n.y, 0.0f};
        up = UpDot(normal3, fn_710002614c(&self->owner->transform));
    }
    UnkVec2Result normal = fn_7100125e78(&edge);
    if (IsEqual(up, 1.0f, eps) || (up < 1.0f && up > 0.7f)) {
        if (state.floor.set) {
            CVector2D p;
            fn_710007b430(&p, &contact->point);
            if (IsAt(state.floor, p))
                return false;
        }
        if (state.retries < state.tuning->max_retries) {
            ++state.retries;
        } else if (!state.floor.set || !(state.floor.y - contact->point.y > 100.0f)) {
            state.component->OnLost(contact);
            state.lost = true;
            return false;
        }
        fn_710057e5cc(self, contact);
        return true;
    }
    f32 facing = normal.x * state.direction.x + normal.y * state.direction.y;
    if (!IsEqual(facing, -1.0f, eps))
        return false;
    if (state.wall.set) {
        CVector2D p;
        fn_710007b430(&p, &contact->point);
        if (IsAt(state.wall, p))
            return false;
    }
    state.retries = 0;
    fn_710057e7c4(self, contact);
    return true;
}

void fn_710057e278(UnkMineAI_l57e278* self, UnkMineContact_l57e278* contact) {
    bool climbed = TryClimb(self, contact);
    fn_71011065a8(self, contact);
    UnkMineState_l57e278& state = self->state;
    if (state.floor.set || state.lost)
        state.component->climbing = state.lost;
    if (climbed)
        fn_710057eb64(self);
}
