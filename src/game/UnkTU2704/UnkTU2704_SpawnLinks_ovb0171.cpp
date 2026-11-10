// CSpawnPointComponent's unit (tools/tu_map.py unit 2704): the string id list's size setter (fn_7101132a20, this
// unit's copy of fn_71007e572c), the collider setup (fn_7101128e88, a copy of
// src/game/UnkTU2704/UnkTU2704_SetupColliders_l129064.cpp), the callback set holding a lazily resolved
// handle (its copy assignment fn_7101130850, the iterator value's move assignment fn_7101130dec and the two
// equality tests fn_7101130c3c, fn_710113111c), the iterator sync of reflected values holding an
// object (fn_710112d438, fn_710112d91c), the weak handle setter fn_7101127c24 and the setup from the two
// resolved links (fn_710112b7c4). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"
#include "base/UnkVector.h"
#include "base/reflection/CClass.h"
#include <new>
#include "game/UnkBroadphaseObject.h"
#include "base/UnkErasedVector.h"
#include "base/UnkCollectionIter.h"
#include "base/UnkRefHandle.h"
#include "base/UnkHashStream.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkGameManager.h"
#include "game_s2/UnkWeakHandle.h"

using base::global::CStrId;

struct __attribute__((packed)) UnkSpawnIdList_ovb0171 {
    u32 count;
    CStrId ids[4];  // 0x4
};
static_assert(sizeof(UnkSpawnIdList_ovb0171) == 0x24, "UnkSpawnIdList_ovb0171 layout");

void fn_7101132a20(UnkSpawnIdList_ovb0171* self, u32 n) {
    if (self->count < n) {
        for (u32 i = self->count; i < n; ++i) {
            CStrId* id = &self->ids[i];
            id->data = nullptr;
            if (!g_unk_7101d47108)
                base::global::UnkEnsureStringPool();
            id->data = &g_unk_7101d47108->empty;
        }
    } else if (self->count > n) {
        for (u64 i = n; i < self->count; ++i)
            self->ids[i].Reset();
    }
    self->count = n;
}

struct UnkManager_7100863a8c;
extern UnkManager_7100863a8c* g_unk_71726a4788;
void fn_7100d46804(UnkManager_7100863a8c* self);

struct UnkSpawnCollider_ovb0171 {
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
    virtual void vf_0x60(u32 mode);
};

struct UnkSpawnOwner_ovb0171 {
    u8 pad_0x0[0x30];
    base::global::CStrId name;  // 0x30
};

struct UnkSpawnPoint_ovb0171 {
    u8 pad_0x0[0x30];
    UnkSpawnOwner_ovb0171* owner;  // 0x30
    u8 pad_0x38[0x118 - 0x38];
    UnkSpawnCollider_ovb0171* collider;  // 0x118
    u8 pad_0x120[0x160 - 0x120];
    void* tracked;  // 0x160
};
static_assert(__builtin_offsetof(UnkSpawnPoint_ovb0171, tracked) == 0x160, "layout");

struct UnkStruct_71007e44b8;
struct UnkObj_71007e44b8;
UnkObj_71007e44b8* fn_71007e44b8(UnkStruct_71007e44b8* self, void* owner, const base::global::CStrId* name, u32 index);
void* fn_7100d475ec(UnkStruct_71007e44b8* self, const base::global::CStrId* name, u32 flag);
struct UnkTriggerCollider_b0058;
void fn_7100d36d30(UnkTriggerCollider_b0058* self, void* shape);
using base::reflection::UnkReflObject;
struct UnkTracker_71008618cc;
void fn_7100d59994(UnkTracker_71008618cc*, UnkReflObject*);
void fn_71011264b4(UnkSpawnPoint_ovb0171* self, void* shape);

static inline UnkStruct_71007e44b8* Manager() {
    UnkManager_7100863a8c* m = g_unk_71726a4788;
    if (!m) {
        m = static_cast<UnkManager_7100863a8c*>(fn_71000839a0(0x6b0, 0x3f));
        fn_7100d46804(m);
        g_unk_71726a4788 = m;
    }
    return reinterpret_cast<UnkStruct_71007e44b8*>(m);
}

void fn_7101128e88(UnkSpawnPoint_ovb0171* self, void* shape) {
    fn_71011264b4(self, shape);
    alignas(8) u8 name_storage[sizeof(base::global::CStrId)];
    {
        UnkStruct_71007e44b8* manager = Manager();
        base::global::CStrId* name = new (name_storage) base::global::CStrId(self->owner->name);
        self->collider = reinterpret_cast<UnkSpawnCollider_ovb0171*>(fn_71007e44b8(manager, self, name, 0));
        name->~CStrId();
    }
    self->collider->vf_0x30(true);
    fn_7100d36d30(reinterpret_cast<UnkTriggerCollider_b0058*>(self->collider), shape);
    self->collider->vf_0x60(2);
    fn_7100d35a98(reinterpret_cast<UnkEntry*>(self->collider), true);
    {
        UnkStruct_71007e44b8* manager = Manager();
        base::global::CStrId name(self->owner->name);
        self->tracked = fn_7100d475ec(manager, &name, 1);
    }
    fn_7100d59994(static_cast<UnkTracker_71008618cc*>(self->tracked),
                  reinterpret_cast<UnkReflObject*>(self->collider));
}

base::UnkRefHandle* fn_71006eb8cc(base::UnkRefHandle*, const base::UnkRefHandle*);  // as src/game_s7/fn_7100245b60.cpp

struct UnkCountedCamera_ovb0171 : base::UnkRefCounted {
    void* field_0x10;
};

struct UnkRef_c3 {
    UnkCountedCamera_ovb0171* ptr;
    ~UnkRef_c3() {
        if (ptr && --ptr->ref_count == 0)
            delete ptr;
    }
};

UnkRef_c3 fn_71003df480(void* from);

struct UnkCameraHolder_ovb0171 {
    u8 pad_0x0[0x8];
    u8 source[0x28];
    u32 state;
    u32 pad_0x34;
    base::UnkRefHandle handle;
};

extern void* g_unk_7101a20998[];
extern void* g_unk_7101cb2880[];

// The callback set behind the camera's lazily resolved handle: callbacks at +0x8, the state at
// +0x30 (1: not resolved yet, 2: resolved) and the handle at +0x38.
struct UnkCameraCallbackBase_ovb0070 {
    void** vtable;
    UnkOwnedErasedVector callbacks;
    u32 state;

    UnkCameraCallbackBase_ovb0070(const UnkCameraCallbackBase_ovb0070& other) asm("fn_7100da4dcc");
    ~UnkCameraCallbackBase_ovb0070() { vtable = &g_unk_7101a20998[2]; }
};

struct UnkCameraCallbackSet_ovb0171 : UnkCameraCallbackBase_ovb0070 {
    base::UnkRefHandle handle;

    // The copy resolves its own handle from the source.
    UnkCameraCallbackSet_ovb0171(const UnkCameraCallbackSet_ovb0171& other)
        : UnkCameraCallbackBase_ovb0070(other) {
        vtable = &g_unk_7101cb2880[2];
        handle.ptr = nullptr;
        if (state == 1) {
            {
                UnkRef_c3 fresh = fn_71003df480(&callbacks);
                fn_71006eb8cc(&handle, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
            }
            if (handle.ptr && static_cast<UnkCountedCamera_ovb0171*>(handle.ptr)->field_0x10)
                state = 2;
        }
    }
    ~UnkCameraCallbackSet_ovb0171() {
        vtable = &g_unk_7101cb2880[2];
        if (base::UnkRefCounted* p = handle.ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }
    UnkCameraCallbackSet_ovb0171& operator=(UnkCameraCallbackSet_ovb0171&& other) {
        fn_71006eb8cc(&handle, &other.handle);
        state = other.state;
        UnkErasedVector* from = &other.callbacks;
        fn_710023b29c(&callbacks, from);
        other.state = 0;
        fn_7100ad3588(from);
        if (base::UnkRefCounted* p = other.handle.ptr) {
            if (--p->ref_count == 0)
                delete p;
            other.handle.ptr = nullptr;
        }
        return *this;
    }
};

// The copy assignment: through a resolved copy of `other`, moved in.
UnkCameraCallbackSet_ovb0171* fn_7101130850(UnkCameraCallbackSet_ovb0171* self,
                                            const UnkCameraCallbackSet_ovb0171* other) {
    if (self != other) {
        UnkCameraCallbackSet_ovb0171 copy(*other);
        *self = static_cast<UnkCameraCallbackSet_ovb0171&&>(copy);
    }
    return self;
}

__attribute__((always_inline)) static inline void* ResolvedTarget(UnkCameraCallbackSet_ovb0171* set) {
    if (set->state == 1) {
        {
            UnkRef_c3 fresh = fn_71003df480(&set->callbacks);
            fn_71006eb8cc(&set->handle, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
        }
        set->state = 2;
    }
    auto* camera = static_cast<UnkCountedCamera_ovb0171*>(set->handle.ptr);
    return camera ? camera->field_0x10 : nullptr;
}

__attribute__((always_inline)) inline bool operator==(const UnkCameraCallbackSet_ovb0171& a, const UnkCameraCallbackSet_ovb0171& b) {
    void* target = ResolvedTarget(const_cast<UnkCameraCallbackSet_ovb0171*>(&b));
    return ResolvedTarget(const_cast<UnkCameraCallbackSet_ovb0171*>(&a)) == target;
}

using UnkCameraIterValue_ovb0171 = base::UnkIterValue<UnkCameraCallbackSet_ovb0171>;

__attribute__((flatten)) bool fn_7101130c3c(const UnkCameraIterValue_ovb0171* a, const UnkCameraIterValue_ovb0171* b) {
    return *a == *b;
}

UnkCameraIterValue_ovb0171* fn_7101130dec(UnkCameraIterValue_ovb0171* self, UnkCameraIterValue_ovb0171* other) {
    if (other->state == 1) {
        self->Index() = other->value.index;
        self->Element() = static_cast<UnkCameraCallbackSet_ovb0171&&>(other->value.element);
    } else if (other->state == 2) {
        self->Index() = *other->ref.index;
        self->Element() = static_cast<UnkCameraCallbackSet_ovb0171&&>(*other->ref.element);
    }
    return self;
}

// A second element type of the same shape (the equality is the same code for it).
struct UnkLinkSetCopy_ovb0171 : UnkCameraCallbackSet_ovb0171 {};
using UnkLinkIterValueCopy_ovb0171 = base::UnkIterValue<UnkLinkSetCopy_ovb0171>;

__attribute__((flatten)) bool fn_710113111c(const UnkLinkIterValueCopy_ovb0171* a, const UnkLinkIterValueCopy_ovb0171* b) {
    return *a == *b;
}

struct UnkBoundObj;
void fn_7100232124(UnkBoundObj* obj);
extern "C++" void operator delete(void*);

// A reflected value that releases the object it holds when it dies: through fn_7100232124 while
// the game manager is up, else it destroys and frees it.
struct UnkHeldValue_ovb0171 : base::reflection::UnkReflValue {
    __attribute__((always_inline)) void Release() {
        if (type) {
            bool live = fn_7100deabe4(g_unk_7101d47310);
            void* held = nullptr;
            if (type)
                held = field_0x14 == 1 || *reinterpret_cast<const u64*>(type + 0x18) > 8 ? field_0x0 : this;
            if (live) {
                fn_7100232124(static_cast<UnkBoundObj*>(held));
            } else if (held) {
                fn_710007c0e4(static_cast<UnkHashStream_0x38*>(held));
                ::operator delete(held);
            }
        }
    }
    __attribute__((always_inline)) ~UnkHeldValue_ovb0171() {
        Release();
        fn_71000a1c60(this);
    }
};

using UnkHeldIter_ovb0171 = base::UnkCollectionIter<UnkHeldValue_ovb0171, u64>;

__attribute__((flatten)) void fn_710112d438(UnkHeldIter_ovb0171* it) {
    if (it->element)
        it->current.SetRef(&it->index, it->element);
    else
        it->current.Reset();
}

// The same code for a second element type of the same shape.
struct UnkHeldValueCopy_ovb0171 : UnkHeldValue_ovb0171 {};
using UnkHeldIterCopy_ovb0171 = base::UnkCollectionIter<UnkHeldValueCopy_ovb0171, u64>;

__attribute__((flatten)) void fn_710112d91c(UnkHeldIterCopy_ovb0171* it) {
    if (it->element)
        it->current.SetRef(&it->index, it->element);
    else
        it->current.Reset();
}

struct UnkSpawnWatcher_ovb0171 {
    u8 pad_0x0[0x2b8];
    bool field_0x2b8;
    u8 pad_0x2b9[0x330 - 0x2b9];
    bool active;                  // 0x330
    bool flag_a;                  // 0x331
    bool flag_b;                  // 0x332
    u8 pad_0x333;
    u32 value;                    // 0x334
    bool flag_c;                  // 0x338
    u8 pad_0x339[0x340 - 0x339];
    UnkWeakHandle* second;        // 0x340
    UnkWeakHandle* first;         // 0x348
};
static_assert(__builtin_offsetof(UnkSpawnWatcher_ovb0171, first) == 0x348, "layout");

void fn_7101127c24(UnkSpawnWatcher_ovb0171* self, UnkWeakTarget* first, bool a, bool b, u32 value, bool c,
                   UnkWeakTarget* second) {
    UnkWeakHandle* handle = first ? GetWeakHandle(first) : nullptr;
    AssignRef(&self->first, handle);
    self->active = true;
    self->flag_a = a;
    if (a)
        self->field_0x2b8 = false;
    self->flag_b = b;
    self->value = value;
    self->flag_c = c;
    UnkWeakHandle* other = second ? GetWeakHandle(second) : nullptr;
    AssignRef(&self->second, other);
}

struct UnkTracker;
UnkTracker* fn_710028aca8(void* link);
struct UnkSpawnPoint_l129064;
void fn_7101129064(UnkSpawnPoint_l129064* self, void* shape);

struct UnkTrackerBody_ovb0171 {
    u8 pad_0x0[0x30];
    void* shape;  // 0x30
};

struct UnkLinkTarget_ovb0171 {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkLinkHolder_ovb0171 {
    u8 pad_0x0[0x8];
    u8 source[0x28];
    u32 state;
    u32 pad_0x34;
    union {
        UnkLinkTarget_ovb0171* target;
        base::UnkRefHandle handle;
    };
    bool IsSet() const {
        if (state == 0)
            return false;
        if (state == 2 && !(target && target->field_0x10))
            return false;
        return true;
    }
    void* Get() {
        if (state == 1) {
            {
                UnkRefPtr fresh = fn_710023b580(source);
                fn_7100234f78(&handle, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
            }
            state = 2;
        }
        return target ? target->field_0x10 : nullptr;
    }
};

struct UnkSpawnTwoLinks_ovb0171 {
    u8 pad_0x0[0x120];
    UnkLinkHolder_ovb0171 first;  // 0x120
    u8 pad_0x160[0x170 - 0x160];
    UnkLinkHolder_ovb0171 second;  // 0x170
};
static_assert(__builtin_offsetof(UnkSpawnTwoLinks_ovb0171, second) == 0x170, "layout");

void fn_710112b7c4(UnkSpawnTwoLinks_ovb0171* self) {
    if (self->first.IsSet()) {
        auto* tracker = reinterpret_cast<UnkTrackerBody_ovb0171*>(fn_710028aca8(self->first.Get()));
        if (tracker && tracker->shape)
            fn_7101128e88(reinterpret_cast<UnkSpawnPoint_ovb0171*>(self), tracker->shape);
    }
    if (self->second.IsSet()) {
        auto* tracker = reinterpret_cast<UnkTrackerBody_ovb0171*>(fn_710028aca8(self->second.Get()));
        if (tracker && tracker->shape)
            fn_7101129064(reinterpret_cast<UnkSpawnPoint_l129064*>(self), tracker->shape);
    }
}
