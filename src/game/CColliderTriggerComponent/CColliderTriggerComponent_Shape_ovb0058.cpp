// CColliderTriggerComponent's unit (tools/tu_map.py unit 1564): rebuilding the trigger's collider from the
// logic shape of the actor it refers to (fn_7100a4c38c, fn_7100a4c444) and reading the box size
// (fn_7100a4c780). Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "base/global/CRntFile.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkReflStatics.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkRefl_fn_7100a48b60;
template <>
base::reflection::UnkReflObject*
base::reflection::refl_FindTyped<UnkRefl_fn_7100a48b60>(void*);

// The actor's logic shape (the "LOGICSHAPE" component fn_7100a48b60 finds) as seen here.
struct UnkLogicShape_b0058 {
    u8 pad_0x0[0x30];
    void* shape;  // 0x30
};

// The trigger's logic-shape reference (+0x428): the property it is read from (+0x430), whether it
// is resolved (+0x458: 1 to resolve, 2 resolved) and the weak handle it resolved to (+0x460).
struct UnkColliderTriggerRef_b0058 {
    void** vtable;
    u8 source[0x30 - 0x8];  // 0x430
    s32 state;  // 0x458
    u8 pad_0x45c[4];
    base::UnkRefHandle handle;  // 0x460
};

// The trigger's collider: created by the collision manager, then configured through its vtable.
struct UnkTriggerCollider_b0058 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30(bool on);
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60(bool on);
};

// The owning actor: its name.
struct UnkTriggerActor_b0058 {
    u8 pad_0x0[0x30];
    base::global::CStrId name;  // 0x30
};

struct UnkColliderTrigger_b0058 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkTriggerActor_b0058* owner;  // 0x30
    u8 pad_0x38[0x58 - 0x38];
    UnkTriggerCollider_b0058* collider;  // 0x58
    u8 pad_0x60[0x428 - 0x60];
    UnkColliderTriggerRef_b0058 shape_ref;  // 0x428
};
static_assert(__builtin_offsetof(UnkColliderTrigger_b0058, shape_ref.handle) == 0x460, "layout");

__attribute__((noinline)) void fn_7100a4c444(UnkColliderTrigger_b0058* self, void* shape);

// The collision manager (as src/game/CBreakableTileGroup_Reset_b49.cpp declares it).
struct UnkManager_7100863a8c;
extern UnkManager_7100863a8c* g_unk_71726a4788;
void fn_7100d46804(UnkManager_7100863a8c* self);
struct UnkCollisionObject;
void fn_7100d46c60(UnkManager_7100863a8c* self, UnkCollisionObject* entry);
struct UnkStruct_71007e44b8;
struct UnkObj_71007e44b8;
UnkObj_71007e44b8* fn_71007e44b8(UnkStruct_71007e44b8* self, void* owner, const base::global::CStrId* name, u32 index);
void fn_7100d36d30(UnkTriggerCollider_b0058* self, void* shape);

static inline UnkManager_7100863a8c* Manager() {
    UnkManager_7100863a8c* m = g_unk_71726a4788;
    if (!m) {
        m = static_cast<UnkManager_7100863a8c*>(fn_71000839a0(0x6b0, 0x3f));
        fn_7100d46804(m);
        g_unk_71726a4788 = m;
    }
    return m;
}

// Hands the old collider back to the manager and has it create a new one named after the actor.
__attribute__((always_inline)) static inline void Recreate(UnkColliderTrigger_b0058* self) {
    if (self->collider) {
        fn_7100d46c60(Manager(), reinterpret_cast<UnkCollisionObject*>(self->collider));
        self->collider = nullptr;
    }
    UnkManager_7100863a8c* manager = Manager();
    base::global::CStrId name(self->owner->name);
    self->collider = reinterpret_cast<UnkTriggerCollider_b0058*>(
        fn_71007e44b8(reinterpret_cast<UnkStruct_71007e44b8*>(manager), self, &name, 0));
}

static inline UnkLogicShape_b0058* FindLogicShape(void* actor) {
    return reinterpret_cast<UnkLogicShape_b0058*>(
        base::reflection::refl_FindTyped<UnkRefl_fn_7100a48b60>(actor));
}

// Rebuilds the collider from the logic shape: the referenced actor's once the reference is resolved
// (resolving it first when asked to), the owner's when there is no reference.
void fn_7100a4c38c(UnkColliderTrigger_b0058* self) {
    UnkColliderTriggerRef_b0058& ref = self->shape_ref;
    void* actor;
    switch (ref.state) {
    case 1: {
        {
            UnkRefPtr resolved = fn_710023b580(ref.source);
            fn_7100234f78(&ref.handle, reinterpret_cast<base::UnkRefHandle*>(&resolved));
        }
        ref.state = 2;
        [[fallthrough]];
    }
    case 2: {
        UnkWeakHandle* handle = reinterpret_cast<UnkWeakHandle*>(ref.handle.ptr);
        if (!handle) {
            actor = nullptr;
            break;
        }
        actor = handle->target;
        break;
    }
    default:
        actor = self->owner;
        break;
    }
    if (UnkLogicShape_b0058* shape = FindLogicShape(actor))
        fn_7100a4c444(self, shape->shape);
}

// A new collider with the logic shape's shape.
void fn_7100a4c444(UnkColliderTrigger_b0058* self, void* shape) {
    Recreate(self);
    self->collider->vf_0x30(true);
    self->collider->vf_0x60(false);
    fn_7100d36d30(self->collider, shape);
    self->collider->vf_0x48();
}

void fn_7100a6081c(UnkColliderTrigger_b0058* self, base::global::CRntFile* file);
void fn_7100a4c804(UnkColliderTrigger_b0058* self, const base::math::CVector3D* size);

// Reads the component, then the trigger box's size (three floats) and builds the box collider.
void fn_7100a4c780(UnkColliderTrigger_b0058* self, base::global::CRntFile* file) {
    fn_7100a6081c(self, file);
    f32 x, z, y;  // stored as x, z, y
    fn_710007c714(file, &x, 4, 1);
    fn_710007c714(file, &z, 4, 1);
    fn_710007c714(file, &y, 4, 1);
    base::math::CVector3D size = {x, y, z};
    fn_7100a4c804(self, &size);
}
