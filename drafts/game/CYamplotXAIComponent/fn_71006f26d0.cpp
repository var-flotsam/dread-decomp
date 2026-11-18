// CYamplotXAIComponent unit: whether the target's body overlaps a 400-unit square at either of the
// owner's kind-2 limits (fn_71010fc864), each widened by twice the owner's radius. Names and layouts are
// placeholders.
#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "game/UnkActor.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU2702.h"
#include "game/UnkTU365.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

// A point of the limits fn_71010fc864 reports (zero unless set).
struct UnkBigFistLimit_ovb0050 {
    f32 x, y, z;
    UnkBigFistLimit_ovb0050() : x(0.0f), y(0.0f), z(0.0f) {}
};

// The kind of limits asked for (an 8-byte slot).
struct UnkBigFistKind_ovb0050 {
    u32 value;
    u32 unk_0x4;
};

struct UnkPlayerMovementCache;
void* fn_71010d7a6c(UnkPlayerMovementCache* self);

struct UnkHitObj_l64b884 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;
};

struct UnkHitRef_l64b884 {
    UnkHitObj_l64b884* ptr;

    UnkHitRef_l64b884() : ptr(nullptr) {}
    ~UnkHitRef_l64b884() {
        if (UnkHitObj_l64b884* p = ptr) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

struct UnkHitVec_l64b884 {
    f32 x, y, z;
    UnkHitVec_l64b884(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
};

struct UnkCollisionHit_l64b884 {
    UnkHitRef_l64b884 object;
    UnkHitRef_l64b884 field_0x8;
    s32 id;
    UnkHitVec_l64b884 point;
    UnkHitVec_l64b884 field_0x20;
    UnkHitVec_l64b884 normal;
    u32 field_0x38;
    u32 flags;
    u32 field_0x40;

    UnkCollisionHit_l64b884()
        : id(-1), point(0.0f, 0.0f, 0.0f), field_0x20(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), field_0x38(0),
          flags(0), field_0x40(0) {}
};

// The hit query the overlap test fills.
struct UnkHitQuery_l6f26d0 {
    void** vtable;
    UnkCollisionHit_l64b884 hit;

    UnkHitQuery_l6f26d0() : vtable(g_unk_7101a20ed8 + 2) {}
    ~UnkHitQuery_l6f26d0() { vtable = g_unk_7101a20ed8 + 2; }
};

// What a box shape is built from: its centre, its axes (a 2x2 rotation), its half size and an offset.
struct UnkBoxDesc_l6f26d0 {
    CVector2D center;
    f32 axes[4];
    CVector2D half_size;
    CVector2D offset;
};

struct UnkBoxShape_l6f26d0;
void fn_7100d68324(UnkBoxShape_l6f26d0* self, const UnkBoxDesc_l6f26d0* desc);
void fn_7100d6845c(UnkBoxShape_l6f26d0* self);

struct UnkBoxShape_l6f26d0 {
    u8 raw[0xd8];

    explicit UnkBoxShape_l6f26d0(const UnkBoxDesc_l6f26d0& desc) { fn_7100d68324(this, &desc); }
    ~UnkBoxShape_l6f26d0() { fn_7100d6845c(this); }
};

struct UnkYamplotXTarget_l6f26d0 {
    u8 pad_0x0[0x200];
    UnkPlayerMovementCache* movement;  // 0x200
};

struct UnkYamplotXAI_l6f26d0 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
};

inline CVector2D Project(const UnkBigFistLimit_ovb0050& point) {
    CVector2D xy;
    fn_710007b430(&xy, reinterpret_cast<const CVector3D*>(&point));
    return xy;
}

// An unrotated 400-unit square centred on `point`'s x and y.
inline UnkBoxDesc_l6f26d0 SquareAt(const UnkBigFistLimit_ovb0050& point) {
    return UnkBoxDesc_l6f26d0{Project(point), {1.0f, 0.0f, -0.0f, 1.0f}, {200.0f, 200.0f}, {0.0f, 0.0f}};
}

inline bool Overlaps(UnkBoxShape_l6f26d0* shape, void* body, UnkHitQuery_l6f26d0* query) {
    return fn_7100d57ca0(reinterpret_cast<UnkLavaDropsShape_A2_b71*>(shape), &g_unk_7101a1a680, body, query, 0);
}

bool fn_71006f26d0(UnkYamplotXAI_l6f26d0* self) {
    UnkBigFistLimit_ovb0050 min;
    UnkBigFistLimit_ovb0050 max;
    bool found;
    {
        UnkBigFistKind_ovb0050 kind;
        kind.value = 2;
        found = fn_71010fc864(static_cast<UnkBigFistAI3_ovb0050*>(self->owner->field_0x240), &kind, &min, &max);
    }
    UnkActor* owner = self->owner;
    f32 margin = owner->field_0x37c + owner->field_0x37c;
    min.x -= margin;
    max.x = margin + max.x;
    if (!found)
        return false;

    UnkActor* target = fn_7100d7c990(owner);
    void* body = fn_71010d7a6c(reinterpret_cast<UnkYamplotXTarget_l6f26d0*>(target)->movement);
    if (body == nullptr)
        return false;

    // The target's body against a square at the lower limit, then at the upper one.
    UnkBoxDesc_l6f26d0 near_desc = SquareAt(min);
    UnkBoxShape_l6f26d0 near_shape(near_desc);
    UnkHitQuery_l6f26d0 near_query;
    bool hit = Overlaps(&near_shape, body, &near_query);
    if (!hit) {
        UnkBoxDesc_l6f26d0 far_desc = SquareAt(max);
        UnkBoxShape_l6f26d0 far_shape(far_desc);
        UnkHitQuery_l6f26d0 far_query;
        hit = Overlaps(&far_shape, body, &far_query);
    }
    return hit;
}
