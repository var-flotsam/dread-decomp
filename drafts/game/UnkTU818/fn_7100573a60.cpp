// Unit 818 (no reliable name): when enabled and the owner's circle overlaps `arg`, sweeps 41 rays across a fan
// around the owner's heading and reports whether one of them hits a collision component the game manager accepts.
// Names and layouts are placeholders.
#include <math.h>
#include "base/UnkDestructors.h"
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU365.h"
#include "game/logic/collision/CCircleShape2D.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;
using base::reflection::UnkReflObject;

struct UnkLavaDropsShape_A2_b71 {
    u8 pad_0x0[0x8];
    u8 body[0xa8];

    UnkLavaDropsShape_A2_b71(const CVector3D& center, f32 size) { fn_7100d6169c(this, &center, size); }
    ~UnkLavaDropsShape_A2_b71() { fn_7100d61750(this); }
};

template <class T>
static bool IsKindOf_l573a60(const T* object, const base::reflection::CClass* cls) {
    return fn_710009d50c(object->vf_0x0(), cls);
}

// The owner's state, whose +0x1fc says whether it is hunting.
struct UnkSweepState_l573a60 {
    u8 pad_0x0[0x1fc];
    bool field_0x1fc;
};

// The owner's AI component (a CCharClassEmmyAIComponent when the check passes).
struct UnkSweepAI_l573a60 {
    virtual const base::reflection::CClass* vf_0x0() const;
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
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual UnkSweepState_l573a60* vf_0xd8();
};

struct UnkSweepHolder_l573a60 {
    u8 pad_0x0[0x10];
    UnkSweepAI_l573a60* ai;  // 0x10
};

struct UnkSweep_l573a60 {
    u8 pad_0x0[0x20];
    UnkSweepHolder_l573a60* holder;  // 0x20
    u8 pad_0x28[0x14];
    f32 radius;  // 0x3c
    f32 spread;  // 0x40
    u8 pad_0x44[0x4];
    CVector2D pos;  // 0x48
    CVector2D dir;  // 0x50
    u8 pad_0x58[0x20];
    bool enabled;  // 0x78
};
static_assert(__builtin_offsetof(UnkSweep_l573a60, enabled) == 0x78, "layout");

// A component hit by a ray, and what the hit object belongs to.
struct UnkHitComponent_l573a60 : UnkReflObject {
    u8 pad_0x8[0x28];
    void* field_0x30;
};
struct UnkHitOwner_l573a60 {
    u8 pad_0x0[0x10];
    UnkHitComponent_l573a60* component;
};
struct UnkHitObject_l573a60 : base::UnkRefCounted {
    UnkHitOwner_l573a60* owner;
};

struct UnkHitRef_l573a60 {
    UnkHitObject_l573a60* ptr;

    UnkHitRef_l573a60() : ptr(nullptr) {}
    ~UnkHitRef_l573a60() {
        if (UnkHitObject_l573a60* p = ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }
};

struct UnkHitVec_l573a60 {
    f32 x, y, z;

    UnkHitVec_l573a60(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
};

// A collision hit (0x48 bytes).
struct UnkHit_l573a60 {
    UnkHitRef_l573a60 object;
    UnkHitRef_l573a60 field_0x8;
    s32 id;
    UnkHitVec_l573a60 point;
    UnkHitVec_l573a60 field_0x20;
    UnkHitVec_l573a60 normal;
    u32 field_0x38;
    u32 flags;
    u32 field_0x40;

    UnkHit_l573a60()
        : id(-1), point(0.0f, 0.0f, 0.0f), field_0x20(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), field_0x38(0),
          flags(0), field_0x40(0) {}
};

// The segment query's callback object: the closest hit and the segment's origin.
struct UnkSegmentQuery_l573a60 {
    void** vtable;
    UnkHit_l573a60 hit;
    bool field_0x50;
    u32 field_0x54;
    CVector3D origin;
    u64 field_0x68;

    explicit UnkSegmentQuery_l573a60(const CVector3D& o) {
        vtable = g_unk_7101a20fa8 + 2;
        field_0x50 = false;
        origin.x = o.x;
        origin.y = o.y;
        origin.z = o.z;
        field_0x68 = 0;
    }
    ~UnkSegmentQuery_l573a60() { vtable = g_unk_7101a20ed8 + 2; }
};

// The segment fn_7100d5c518 tests: its two ends and the vector between them.
struct UnkSegment_l573a60 {
    CVector2D start;
    CVector2D end;
    CVector2D delta;

    UnkSegment_l573a60(const CVector2D& s, const CVector2D& e) {
        start.x = s.x;
        start.y = s.y;
        end.x = e.x;
        end.y = e.y;
        delta.x = e.x - s.x;
        delta.y = e.y - s.y;
    }
};

struct UnkWatch_b0258w;
struct UnkWatchArg_b0258w;

bool fn_7100573a60(UnkWatch_b0258w* watch, UnkWatchArg_b0258w* arg, f32 scale, f32 spread) {
    UnkSweep_l573a60* self = reinterpret_cast<UnkSweep_l573a60*>(watch);
    if (!self->enabled)
        return false;

    f32 radius = self->radius * scale;
    UnkLavaDropsShape_A2_b71 shape(CVector3D{self->pos.x, self->pos.y, 0.0f}, radius);
    bool found = false;
    if (fn_7100d57ca0(&shape, shape.body, arg, nullptr, 0)) {
        UnkGameManager* gm = g_unk_7101d47310;
        UnkTree* tree = gm->field_0x608 ? static_cast<UnkTree*>(gm->field_0x608->field_0x820) : nullptr;
        f32 width = spread != -1.0f ? spread : self->spread;
        f32 first = atan2f(self->dir.x, self->dir.y) + width * -0.5f;
        f32 step = width / 40.0f;
        GetDefaultAllocator();

        u32 i = 0;
        do {
            f32 angle = first + step * (f32)i;
            f32 dx = radius * sinf(angle);
            f32 dy = radius * cosf(angle);
            CVector2D origin = self->pos;
            CVector2D end = {dx + origin.x, dy + origin.y};
            UnkSegment_l573a60 segment(origin, end);
            UnkSegmentQuery_l573a60 query(CVector3D{origin.x, origin.y, 0.0f});

            u32 mask = 0x23;
            UnkSweepAI_l573a60* ai = self->holder->ai;
            if (ai && IsKindOf_l573a60(ai, fn_710051e010()) &&
                ai->vf_0xd8()->field_0x1fc)
                mask = 0x20;

            if (fn_7100d5c518(tree, mask, reinterpret_cast<const UnkBox*>(&segment), &query, 2)) {
                UnkHitComponent_l573a60* component = query.hit.object.ptr->owner->component;
                if (component && IsKindOf_l573a60(component, fn_710085fc44()) && component->field_0x30)
                    found = fn_7100df5c38(g_unk_7101d47310, component->field_0x30);
            }
        } while (++i <= 40 && !found);
    }
    return found;
}
