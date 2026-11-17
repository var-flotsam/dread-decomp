// behaviortree::CChaseTargetAction's unit (tools/tu_map.py unit 471): clears the hit at `out`, then, while the
// owner's AI chases (its vf_0x698 above -0.001), casts a segment from the owner across to its target through
// the game manager's collision tree and keeps the first hit unless it is a collision component whose owner
// has +0x208 set. Returns false when the target or the tree is missing. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityShinespark.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU365.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;
using base::reflection::UnkReflObject;

struct UnkChaseRef_ovb0132 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;
    void* target;  // +0x10
};

struct UnkChaseRefSlot_ovb0132 {
    UnkChaseRef_ovb0132* ptr;
    void Reset() {
        if (ptr) {
            if (--ptr->ref_count == 0)
                ptr->vf_0x10();
            ptr = nullptr;
        }
    }
};

struct UnkChaseAction_ovb0132 {
    u8 pad_0x0[0x18];
    UnkActor* actor;  // +0x18
    u8 pad_0x20[0xc];
    u8 field_0x2c[0x1c];
    u8 pad_0x48[0x11];
    bool field_0x59;
    bool field_0x5a;
    u8 pad_0x5b;
    u32 field_0x5c;
    u32 state;  // +0x60
    u32 field_0x64;
    f32 width;   // +0x68
    f32 height;  // +0x6c
    u8 field_0x70[8];
    UnkChaseRefSlot_ovb0132 field_0x78;
    UnkChaseRefSlot_ovb0132 field_0x80;
    u8 pad_0x88[0x2c];
    u32 field_0xb4;
    u8 pad_0xb8[0x8];
    CVector3D field_0xc0;
    f32 field_0xcc;  // the minimum size
    CVector3D field_0xd0;
};
static_assert(__builtin_offsetof(UnkChaseAction_ovb0132, field_0xd0) == 0xd0, "layout");

// A reference to a hit object, counted.
struct UnkChaseHitRef_l37b7b4 {
    UnkChaseRef_ovb0132* ptr;

    UnkChaseHitRef_l37b7b4() : ptr(nullptr) {}
    UnkChaseHitRef_l37b7b4(const UnkChaseHitRef_l37b7b4& other) : ptr(other.ptr) {
        if (ptr)
            ++ptr->ref_count;
    }
    ~UnkChaseHitRef_l37b7b4() {
        if (UnkChaseRef_ovb0132* p = ptr) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
    UnkChaseHitRef_l37b7b4& operator=(const UnkChaseHitRef_l37b7b4& other) {
        fn_71002a5d1c(reinterpret_cast<UnkAssignedSlot*>(this), reinterpret_cast<const UnkAssignedSlot*>(&other));
        return *this;
    }
    void Reset() {
        if (ptr) {
            if (--ptr->ref_count == 0)
                ptr->vf_0x10();
            ptr = nullptr;
        }
    }
};

// What was hit: the object and a second reference.
struct UnkChaseHitHandle_l37b7b4 {
    UnkChaseHitRef_l37b7b4 object;
    UnkChaseHitRef_l37b7b4 field_0x8;
};

struct UnkChaseHit_l37b7b4 {
    UnkChaseHitHandle_l37b7b4 handle;
    s32 id;
    CVector3D point;
    CVector3D field_0x20;
    CVector3D normal;
    u32 field_0x38;
    u32 flags;
    u32 field_0x40;

    UnkChaseHit_l37b7b4()
        : id(-1), point{0.0f, 0.0f, 0.0f}, field_0x20{0.0f, 0.0f, 0.0f}, normal{0.0f, 0.0f, 0.0f}, field_0x38(0),
          flags(0), field_0x40(0) {}

    UnkChaseHit_l37b7b4& operator=(const UnkChaseHit_l37b7b4& other) {
        if (this != &other) {
            handle.object = other.handle.object;
            handle.field_0x8 = other.handle.field_0x8;
            id = other.id;
            point.x = other.point.x;
            point.y = other.point.y;
            point.z = other.point.z;
            field_0x20.x = other.field_0x20.x;
            field_0x20.y = other.field_0x20.y;
            field_0x20.z = other.field_0x20.z;
            normal.x = other.normal.x;
            normal.y = other.normal.y;
            normal.z = other.normal.z;
            field_0x38 = other.field_0x38;
            flags = other.flags;
            field_0x40 = other.field_0x40;
        }
        return *this;
    }
};
static_assert(sizeof(UnkChaseHit_l37b7b4) == 0x48, "layout");

// The query the cast fills with its first hit.
struct UnkChaseQuery_l37b7b4 {
    void** vtable;
    UnkChaseHit_l37b7b4 hit;
    bool field_0x50;

    UnkChaseQuery_l37b7b4() {
        field_0x50 = false;
        vtable = g_unk_7101a23590 + 2;
    }
    ~UnkChaseQuery_l37b7b4() { vtable = g_unk_7101a20ed8 + 2; }
};

// A segment in the plane: its ends and the step from one to the other.
struct UnkChaseSegment_l37b7b4 {
    CVector2D start;
    CVector2D end;
    CVector2D delta;
};

struct UnkChaseAI_l37b7b4 {
    void** vtable;
    f32 vf_0x698() { return reinterpret_cast<f32 (*)(UnkChaseAI_l37b7b4*)>(vtable[0x698 / 8])(this); }
};

struct UnkChaseMask_l37b7b4 {
    u8 pad_0x0[0x3cc];
    u32 mask;
};

struct UnkChaseColliderOwner_l37b7b4 {
    u8 pad_0x0[0x208];
    void* field_0x208;
};

struct UnkChaseCollider_l37b7b4 : UnkReflObject {
    u8 pad_0x8[0x28];
    UnkChaseColliderOwner_l37b7b4* owner;  // +0x30
};

struct UnkChaseHitTarget_l37b7b4 {
    u8 pad_0x0[0x10];
    UnkChaseCollider_l37b7b4* component;  // +0x10
};

// A zero whose constructor is not constexpr, so the static is guarded.
struct UnkChaseStaticZero_l37b7b4 {
    u32 value;
    UnkChaseStaticZero_l37b7b4() : value(0) {}
};

inline const UnkChaseStaticZero_l37b7b4& UnkChaseStaticZero_l37b7b4_Get() {
    static UnkChaseStaticZero_l37b7b4 s_value asm("g_unk_71726b5354");
    return s_value;
}

static inline CVector3D Pos(const UnkActor* actor) {
    return {actor->field_0x44, actor->field_0x54, actor->field_0x64};
}

static inline void Project(CVector2D* out, const CVector3D& v) {
    fn_710007b430(out, &v);
}

// Whether the hit object is a collision component whose owner has +0x208 set: such hits are passed over.
static inline bool IsPassedOver(const UnkChaseHitHandle_l37b7b4& handle) {
    UnkChaseRef_ovb0132* object = handle.object.ptr;
    if (!object || !object->target)
        return false;
    UnkChaseCollider_l37b7b4* component = static_cast<UnkChaseHitTarget_l37b7b4*>(object->target)->component;
    if (!component || !fn_710009d50c(component->vf_0x0(), fn_710085fc44()) || !component->owner)
        return false;
    return component->owner->field_0x208 != nullptr;
}

bool fn_710037b7b4(UnkChaseAction_ovb0132* self, UnkChaseRefSlot_ovb0132* out_slot) {
    UnkChaseHit_l37b7b4* out = reinterpret_cast<UnkChaseHit_l37b7b4*>(out_slot);
    out->handle.object.Reset();
    out->handle.field_0x8.Reset();
    out->flags = UnkChaseStaticZero_l37b7b4_Get().value;

    UnkActor* actor = self->actor;
    if (!actor)
        return true;
    UnkChaseAI_l37b7b4* ai = static_cast<UnkChaseAI_l37b7b4*>(actor->field_0x240);
    if (!ai || !(ai->vf_0x698() > -0.001f))
        return true;

    UnkActor* target = fn_7100d7c990(self->actor);
    if (!target)
        return false;
    UnkGameManagerSub_0x608* collision = g_unk_7101d47310->field_0x608;
    if (!collision)
        return false;
    UnkTree* tree = static_cast<UnkTree*>(collision->field_0x820);
    if (!tree)
        return false;
    actor = self->actor;
    UnkChaseMask_l37b7b4* flagged = static_cast<UnkChaseMask_l37b7b4*>(actor->field_0x218);
    if (!flagged)
        return false;

    f32 dx = target->field_0x44 - actor->field_0x44;
    CVector2D from;
    Project(&from, Pos(actor));
    UnkChaseSegment_l37b7b4 segment;
    UnkChaseQuery_l37b7b4 query;
    segment.start.x = from.x;
    segment.start.y = from.y;
    segment.end.x = dx + from.x;
    segment.end.y = 0.0f + from.y;
    segment.delta.x = segment.end.x - segment.start.x;
    segment.delta.y = segment.end.y - segment.start.y;
    if (fn_7100d5c518(tree, flagged->mask, reinterpret_cast<const UnkBox*>(&segment), &query, 2)) {
        UnkChaseHitHandle_l37b7b4 handle = query.hit.handle;
        if (!IsPassedOver(handle))
            *out = query.hit;
    }
    return true;
}
