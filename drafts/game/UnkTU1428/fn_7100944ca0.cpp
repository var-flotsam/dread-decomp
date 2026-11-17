// Unit 1428 (CMorphBallMovement's neighbours): adds to the ball's velocity its direction, turned by the
// side of the contact, times a speed eased between two limits over time. On the top end of an edge of
// an E.M.M.I.'s collision (or of a character class that asks for it), the push follows the way that
// actor faces. Names and layouts are placeholders.
#include <math.h>
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTU53.h"
#include "game/logic/collision/CCollider.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;
using base::reflection::CClass;
using base::reflection::UnkReflObject;

// An edge of a shape: its two ends. Returned through x8: its copy constructor is not trivial.
struct UnkEdge_l944ca0 {
    CVector2D a;
    CVector2D b;

    UnkEdge_l944ca0(const UnkEdge_l944ca0& o) : a(o.a), b(o.b) {}
};

// The edge `index` of a shape's edge list.
UnkEdge_l944ca0 fn_710012436c(const void* edges, u32 index);

struct UnkActor_l944ca0 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // +0x38
    u8 pad_0x78[0x240 - 0x78];
    UnkReflObject* ai;  // +0x240
};

struct UnkCharClass_l944ca0 : UnkReflObject {
    u8 pad_0x8[0xc9 - 0x8];
    bool field_0xc9;
};

struct UnkCollisionComponent_l944ca0 : UnkReflObject {
    u8 pad_0x8[0x10 - 0x8];
    UnkCharClass_l944ca0* char_class;  // +0x10
    u8 pad_0x18[0x30 - 0x18];
    UnkActor_l944ca0* actor;  // +0x30
};

struct UnkEdgeShape_l944ca0 {
    void** vtable;
    u8 pad_0x8[0xa8 - 0x8];
    u8 edges[1];  // +0xa8

    s32 GetKind() { return reinterpret_cast<s32 (*)(UnkEdgeShape_l944ca0*)>(vtable[0x18 / 8])(this); }
};

struct UnkCollider_l944ca0 {
    u8 pad_0x0[0x10];
    UnkCollisionComponent_l944ca0* component;  // +0x10
    u8 pad_0x18[0x158 - 0x18];
    UnkEdgeShape_l944ca0* shape;  // +0x158
};

struct UnkContactObject_l944ca0 {
    u8 pad_0x0[0x10];
    UnkCollider_l944ca0* collider;  // +0x10
};

// The ball's contact: what it touches, the edge, the point and the side.
struct UnkContact_l944ca0 {
    UnkContactObject_l944ca0* object;
    u8 pad_0x8[0x8];
    u32 edge;  // +0x10
    CVector3D point;  // +0x14
    u8 pad_0x20[0x2c - 0x20];
    f32 side;  // +0x2c

    UnkCollider_l944ca0* Collider() const { return object ? object->collider : nullptr; }
};

struct UnkCurve_l944ca0 {
    u8 pad_0x0[0x10];
    const char* text;  // +0x10
    u8 pad_0x18[0x44 - 0x18];
    u32 use_count;  // +0x44
};

struct UnkPushConfig_l944ca0 {
    u8 pad_0x0[0xa8];
    UnkCurve_l944ca0* curve;  // +0xa8
    f32 duration;  // +0xb0
    f32 min;  // +0xb4
    f32 max;  // +0xb8
};

struct UnkBallMovement_l944ca0 {
    u8 pad_0x0[0x128];
    UnkContact_l944ca0 contact;  // +0x128
    u8 pad_0x158[0x1d8 - 0x158];
    CVector3D velocity;  // +0x1d8
    u8 pad_0x1e4[0x2280 - 0x1e4];
    UnkPushConfig_l944ca0* config;  // +0x2280
    u8 pad_0x2288[0x2654 - 0x2288];
    f32 time;  // +0x2654
};
static_assert(__builtin_offsetof(UnkBallMovement_l944ca0, time) == 0x2654, "UnkBallMovement_l944ca0 layout");

template <const CClass* (*GetClass)()>
static inline bool IsA(const UnkReflObject* obj) {
    return fn_710009d50c(obj->vf_0x0(), GetClass());
}

// -1, 0 or 1 by the sign of v.
static inline s32 Sign(f32 v) {
    return (v > 0.0f) - (v < 0.0f);
}

// v clamped to [min, max] (max first).
static inline f32 Clamp(f32 v, f32 min, f32 max) {
    f32 r = v >= max ? max : v;
    if (v <= min)
        return min;
    return r;
}


static inline CVector2D Flatten(const CVector3D& v) {
    CVector2D r;
    fn_710007b430(&r, &v);
    return r;
}

static inline CVector3D Translation(const UnkMatrix44& m) {
    return CVector3D{m.m[3], m.m[7], m.m[11]};
}

static inline bool Near(const CVector2D& end, const CVector2D& p) {
    const f32 eps = 0.001f;
    return end.x + eps >= p.x && end.x - eps <= p.x && end.y + eps >= p.y && end.y - eps <= p.y;
}

void fn_7100944ca0(UnkBallMovement_l944ca0* self, const CVector3D* dir, f32 dt) {
    UnkPushConfig_l944ca0* config = self->config;
    f32 t = self->time / config->duration;
    ++config->curve->use_count;
    const char* text = config->curve->text;
    f32 eased = Clamp(fn_7100076020(t, text ? text : g_unk_710162e2ed), 0.0f, 1.0f);
    f32 speed = self->config->min + eased * (self->config->max - self->config->min);

    f32 side = Sign(self->contact.side);
    f32 x = dir->x * side;
    f32 y = dir->y * side;
    f32 z = dir->z * side;

    if (UnkCollider_l944ca0* collider = self->contact.Collider()) {
        UnkCollisionComponent_l944ca0* component = collider->component;
        if (component && IsA<fn_710085fc44>(component)) {
            UnkCharClass_l944ca0* char_class = component->char_class;
            UnkActor_l944ca0* actor;
            if (char_class && IsA<fn_710079a218>(char_class) && (actor = component->actor) &&
                ((actor->ai && IsA<fn_710053b934>(actor->ai)) || char_class->field_0xc9)) {
                UnkEdgeShape_l944ca0* shape = self->contact.object->collider->shape;
                if (shape && shape->GetKind() == 0x10) {
                    CVector2D point;
                    fn_710007b430(&point, &self->contact.point);
                    UnkEdge_l944ca0 edge = fn_710012436c(shape->edges, self->contact.edge);
                    CVector2D offset =
                        Flatten(Translation(fn_7100d36a2c(reinterpret_cast<UnkShape*>(self->contact.Collider()))));
                    edge.a.x = offset.x + edge.a.x;
                    edge.a.y = offset.y + edge.a.y;
                    edge.b.x = offset.x + edge.b.x;
                    edge.b.y = offset.y + edge.b.y;
                    // Only on the top end of the edge.
                    if (edge.a.y > edge.b.y ? Near(edge.a, point) : Near(edge.b, point))
                        x = fabsf(x) * Sign(fn_710002614c(&actor->transform).m[2]);
                }
            }
        }
    }

    self->velocity.x += speed * (x * dt);
    self->velocity.y += speed * (y * dt);
    self->velocity.z += speed * (z * dt);
}
