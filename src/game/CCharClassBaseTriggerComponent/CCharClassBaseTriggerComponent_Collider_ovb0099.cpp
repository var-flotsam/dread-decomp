// CCharClassBaseTriggerComponent unit (tools/tu_map.py): the trigger's constructors, rebuilding its
// collider from a logic shape, a margin test against the collider's box, and the copy-assignment of
// its weak-reference records. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/UnkRefHandle.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CComponent.h"
#include "game/CDemolitionBlockComponent.h"
#include "game/UnkBroadphaseObject.h"
#include "game/UnkGameManager.h"
#include "game/logic/collision/CCollider.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkRefl_fn_7100a48b60;
template <>
base::reflection::UnkReflObject*
base::reflection::refl_FindTyped<UnkRefl_fn_7100a48b60>(void*);

// The trigger's collider: created by the collision manager, then configured through its vtable.
struct UnkBaseTriggerCollider {
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
    virtual void vf_0x60(u32 value);
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78(const UnkMatrix44& transform);
};

// The actor's logic shape (the "LOGICSHAPE" component fn_7100a48b60 finds) as seen here.
struct UnkBaseTriggerLogicShape {
    u8 pad_0x0[0x30];
    void* shape;  // 0x30
};

// An actor as seen here: its name and its transform.
struct UnkBaseTriggerActor {
    u8 pad_0x0[0x30];
    base::global::CStrId name;  // 0x30
    UnkMatrix44 transform;      // 0x38 (passed by address only)
};

// The trigger's logic-shape reference (+0x9d0): the property it is read from (+0x9d8), whether it
// is resolved (+0xa00: 1 to resolve, 2 resolved) and the weak handle it resolved to (+0xa08).
struct UnkBaseTriggerRef {
    void** vtable;
    u8 source[0x30 - 0x8];  // 0x9d8
    s32 state;              // 0xa00
    u8 pad_0xa04[4];
    base::UnkRefHandle handle;  // 0xa08
};

struct UnkBaseTrigger {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkBaseTriggerActor* owner;  // 0x30
    u8 pad_0x38[0x80 - 0x38];
    u32 field_0x80;
    u8 pad_0x84[4];
    UnkBaseTriggerCollider* collider;  // 0x88
    void* shape;                       // 0x90
    u8 pad_0x98[0x9d0 - 0x98];
    UnkBaseTriggerRef shape_ref;  // 0x9d0
};
static_assert(__builtin_offsetof(UnkBaseTrigger, shape_ref.handle) == 0xa08, "layout");

// The collision manager (as src/game/CBreakableTileGroup_Reset_b49.cpp declares it).
struct UnkManager_7100863a8c;
extern UnkManager_7100863a8c* g_unk_71726a4788;
void fn_7100d46804(UnkManager_7100863a8c* self);
struct UnkCollisionObject;
void fn_7100d46c60(UnkManager_7100863a8c* self, UnkCollisionObject* entry);
struct UnkStruct_71007e44b8;
struct UnkObj_71007e44b8;
UnkObj_71007e44b8* fn_71007e44b8(UnkStruct_71007e44b8* self, void* owner, const base::global::CStrId* name, u32 index);
struct UnkTriggerCollider_b0058;
void fn_7100d36e24(UnkTriggerCollider_b0058* self, void* shape);

static inline UnkManager_7100863a8c* Manager() {
    UnkManager_7100863a8c* m = g_unk_71726a4788;
    if (!m) {
        m = static_cast<UnkManager_7100863a8c*>(fn_71000839a0(0x6b0, 0x3f));
        fn_7100d46804(m);
        g_unk_71726a4788 = m;
    }
    return m;
}

// Hands the old collider back to the manager, has it create a new one named after the actor and
// sets it up with `shape` (placed at `transform` when the game manager allows it); `swept` picks
// the shape setter.
void fn_7100a48c3c(UnkBaseTrigger* self, void* shape, const UnkMatrix44* transform, bool swept, u32 value) {
    if (self->collider) {
        fn_7100d46c60(Manager(), reinterpret_cast<UnkCollisionObject*>(self->collider));
        self->collider = nullptr;
    }
    UnkManager_7100863a8c* manager = Manager();
    {
        base::global::CStrId name(self->owner->name);
        self->collider = reinterpret_cast<UnkBaseTriggerCollider*>(
            fn_71007e44b8(reinterpret_cast<UnkStruct_71007e44b8*>(manager), self, &name, 0));
    }
    if (fn_7100deabe4(g_unk_7101d47310))
        self->collider->vf_0x78(fn_710002614c(transform));
    self->collider->vf_0x30(true);
    self->collider->vf_0x60(value);
    UnkTriggerCollider_b0058* collider = reinterpret_cast<UnkTriggerCollider_b0058*>(self->collider);
    if (swept)
        fn_7100d36e24(collider, shape);
    else
        fn_7100d36d30(collider, shape);
    self->shape = shape;
    fn_7100d35a98(reinterpret_cast<UnkEntry*>(self->collider), true);
}

static inline UnkBaseTriggerLogicShape* FindLogicShape(void* actor) {
    return reinterpret_cast<UnkBaseTriggerLogicShape*>(
        base::reflection::refl_FindTyped<UnkRefl_fn_7100a48b60>(actor));
}

// Rebuilds the collider from the logic shape: the referenced actor's once the reference is resolved
// (resolving it first when asked to), the owner's when there is no reference.
void fn_7100a46e9c(UnkBaseTrigger* self) {
    UnkBaseTriggerRef& ref = self->shape_ref;
    UnkBaseTriggerActor* actor;
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
        actor = reinterpret_cast<UnkBaseTriggerActor*>(handle->target);
        break;
    }
    default:
        actor = self->owner;
        break;
    }
    if (UnkBaseTriggerLogicShape* shape = FindLogicShape(actor))
        fn_7100a48c3c(self, shape->shape, &actor->transform, false, self->field_0x80);
}

// The collider (+0x88) as the bounds test reads it: a box (+0x40 min, +0x48 max) that vf_0x98
// recomputes while +0x60 is set.
struct UnkTriggerShape {
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
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    u8 pad_0x8[0x38];
    f32 min_x;  // 0x40
    f32 min_y;  // 0x44
    f32 max_x;  // 0x48
    f32 max_y;  // 0x4c
    u8 pad_0x50[0x10];
    bool dirty;  // 0x60
};

struct UnkTriggerPoint {
    f32 x, y;
};

struct UnkBaseTriggerBounds {
    u8 pad_0x0[0x88];
    UnkTriggerShape* shape;  // 0x88
};

static inline bool Near(f32 v, f32 limit) {
    return v - 1.0f <= limit && v + 1.0f >= limit;
}

static inline bool AboveMin(f32 v, f32 min) {
    return v >= min || Near(v, min);
}

static inline bool BelowMax(f32 v, f32 max) {
    return v <= max || Near(v, max);
}

// Whether the point is inside the shape's box, with a margin of one unit.
bool fn_7100a47e00(UnkBaseTriggerBounds* self, const UnkTriggerPoint* point) {
    UnkTriggerShape* shape = self->shape;
    if (!shape)
        return false;
    if (shape->dirty)
        shape->vf_0x98();
    f32 x = point->x;
    if (!AboveMin(x, shape->min_x) || !BelowMax(x, shape->max_x))
        return false;
    f32 y = point->y;
    if (!AboveMin(y, shape->min_y) || !BelowMax(y, shape->max_y))
        return false;
    return true;
}

// A weak reference and three words after it (the same record as CTriggerComponent's, in
// src/game/CTriggerComponent/CTriggerComponent_WeakRecord_ovb0067.cpp).
struct UnkBaseTriggerWeakRecord {
    UnkWeakHandle* handle;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;

    // The copy-assignment, out of line (fn_7100a495e4): the handle of the other's target is taken
    // (created on first use) unless both are the same record.
    UnkBaseTriggerWeakRecord& operator=(const UnkBaseTriggerWeakRecord& other) asm("fn_7100a495e4");
};

UnkBaseTriggerWeakRecord& UnkBaseTriggerWeakRecord::operator=(const UnkBaseTriggerWeakRecord& other) {
    if (this != &other)
        CopyWeakRef(&handle, other.handle);
    field_0x8 = other.field_0x8;
    field_0xc = other.field_0xc;
    field_0x10 = other.field_0x10;
    return *this;
}

struct UnkStateObject;
void fn_710002f4e4(void*);
extern void* g_unk_7101a7ac20[];
extern void* g_unk_7101a8ebc8[];
extern void* g_unk_7101a20998[];
extern void* g_unk_7101a20950[];

// An allocator-backed list (base vtable, empty array with step 10 and flags 0xf, allocator, final
// vtable), as in src/game/CAIComponent/UnkAIDeviceList.cpp.
struct UnkBaseTriggerList {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    u32 field_0x18;
    bool field_0x1c;
    u32 field_0x20;
    base::UnkAllocator* field_0x28;
    u32 field_0x30;
    u64 field_0x38;
};

struct UnkBaseTriggerCtor {
    void** vtable;
    u8 pad_0x8[0x38];
    u8 field_0x40[0x38];
    bool field_0x78;
    u8 pad_0x79[3];
    u32 field_0x7c;
    u32 field_0x80;
    u8 pad_0x84[4];
    void* field_0x88;
    void* field_0x90;
    u32 field_0x98;
    u8 pad_0x9c[4];
    u64 field_0xa0;
    u32 field_0xa8;
    u8 pad_0xac[0x3ac - 0xac];
    u32 field_0x3ac;
    u8 pad_0x3b0[0x9b0 - 0x3b0];
    base::global::CStrId field_0x9b0;
    u32 field_0x9b8;
    bool field_0x9bc;
    bool field_0x9bd;
    bool field_0x9be;
    u8 pad_0x9bf;
    u64 field_0x9c0;
    u32 field_0x9c8;
    u8 pad_0x9cc[4];
    UnkBaseTriggerList field_0x9d0;
    u8 field_0xa10[0x8];
};
static_assert(__builtin_offsetof(UnkBaseTriggerCtor, field_0xa10) == 0xa10, "layout");

// The same object with the list written out flat (the other constructor matches that way).
struct UnkBaseTriggerCtorFlat {
    void** vtable;
    u8 pad_0x8[0x38];
    u8 field_0x40[0x38];
    bool field_0x78;
    u8 pad_0x79[3];
    u32 field_0x7c;
    u32 field_0x80;
    u8 pad_0x84[4];
    void* field_0x88;
    void* field_0x90;
    u32 field_0x98;
    u8 pad_0x9c[4];
    u64 field_0xa0;
    u32 field_0xa8;
    u8 pad_0xac[0x3ac - 0xac];
    u32 field_0x3ac;
    u8 pad_0x3b0[0x9b0 - 0x3b0];
    base::global::CStrId field_0x9b0;
    u32 field_0x9b8;
    bool field_0x9bc;
    bool field_0x9bd;
    bool field_0x9be;
    u8 pad_0x9bf;
    u64 field_0x9c0;
    u32 field_0x9c8;
    u8 pad_0x9cc[4];
    void** field_0x9d0;
    u64 field_0x9d8;
    u64 field_0x9e0;
    u32 field_0x9e8;
    bool field_0x9ec;
    u32 field_0x9f0;
    base::UnkAllocator* field_0x9f8;
    u32 field_0xa00;
    u64 field_0xa08;
    u8 field_0xa10[0x8];
};
static_assert(__builtin_offsetof(UnkBaseTriggerCtorFlat, field_0xa10) == 0xa10, "layout");

void fn_7100a49318(UnkBaseTriggerCtorFlat* self) {
    fn_71010c4148(reinterpret_cast<UnkStateObject*>(self));
    self->vtable = g_unk_7101a7ac20 + 2;
    fn_71007b1528(reinterpret_cast<char*>(self->field_0x40));
    self->field_0x78 = false;
    self->field_0x7c = 0;
    self->vtable = g_unk_7101a8ebc8 + 2;
    self->field_0x80 = 0x1090;
    self->field_0x88 = nullptr;
    self->field_0x90 = nullptr;
    self->field_0x98 = 0;
    self->field_0xa0 = 0;
    self->field_0xa8 = 0;
    self->field_0x3ac = 0;
    new (&self->field_0x9b0) base::global::CStrId(g_unk_710162e2ed, true);
    self->field_0x9b8 = 0;
    self->field_0x9bc = false;
    self->field_0x9bd = false;
    self->field_0x9be = false;
    self->field_0x9c0 = 0;
    self->field_0x9c8 = 0;
    self->field_0x9d0 = g_unk_7101a20998 + 2;
    self->field_0x9d8 = 0;
    self->field_0x9e0 = 0;
    self->field_0x9e8 = 10;
    self->field_0x9ec = true;
    self->field_0x9f0 = 15;
    self->field_0x9f8 = fn_7100083990();
    self->field_0xa00 = 0;
    self->field_0x9d0 = g_unk_7101a20950 + 2;
    self->field_0xa08 = 0;
    fn_710002f4e4(self->field_0xa10);
    self->field_0x9bc = false;
    self->field_0x9be = false;
    self->field_0x9c0 = 0;
    self->field_0x9c8 = 0;
    reinterpret_cast<void (*)(UnkBaseTriggerCtorFlat*)>(self->vtable[3])(self);
}

void fn_7100a46a68(UnkBaseTriggerCtor* self) {
    fn_71010c4148(reinterpret_cast<UnkStateObject*>(self));
    self->vtable = g_unk_7101a7ac20 + 2;
    fn_71007b1528(reinterpret_cast<char*>(self->field_0x40));
    self->field_0x78 = false;
    self->field_0x7c = 0;
    self->vtable = g_unk_7101a8ebc8 + 2;
    self->field_0x80 = 0x1090;
    self->field_0x88 = nullptr;
    self->field_0x90 = nullptr;
    self->field_0x98 = 0;
    self->field_0xa0 = 0;
    self->field_0xa8 = 0;
    self->field_0x3ac = 0;
    new (&self->field_0x9b0) base::global::CStrId(g_unk_710162e2ed, true);
    self->field_0x9b8 = 0;
    self->field_0x9bc = false;
    self->field_0x9bd = false;
    self->field_0x9be = false;
    self->field_0x9c0 = 0;
    self->field_0x9c8 = 0;
    self->field_0x9d0.vtable = g_unk_7101a20998 + 2;
    self->field_0x9d0.field_0x8 = 0;
    self->field_0x9d0.field_0x10 = 0;
    self->field_0x9d0.field_0x18 = 10;
    self->field_0x9d0.field_0x1c = true;
    self->field_0x9d0.field_0x20 = 15;
    self->field_0x9d0.field_0x28 = fn_7100083990();
    self->field_0x9d0.field_0x30 = 0;
    self->field_0x9d0.vtable = g_unk_7101a20950 + 2;
    self->field_0x9d0.field_0x38 = 0;
    fn_710002f4e4(self->field_0xa10);
    self->field_0x9bc = false;
    self->field_0x9be = false;
    self->field_0x9c0 = 0;
    self->field_0x9c8 = 0;
}
