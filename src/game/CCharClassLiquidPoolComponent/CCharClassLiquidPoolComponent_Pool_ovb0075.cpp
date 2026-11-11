// CCharClassLiquidPoolComponent unit (unit 1394 of tools/tu_map.py; the class name from reflection):
// - fn_7100900b40 / fn_7100900c44: start-up steps after the base ones (fn_7100a46f74, fn_7100a46e78): the
//   registration move of fn_7100901144 plus the owner's position (+0x388) from vtable slot 0x178; the level
//   reset to 1 and the surface box (+0xc64 centre, +0xc70 size) taken from the body's bounds (+0x88);
// - fn_7100901264, fn_7100904f7c, fn_7100904ddc: a keyed array of 16-byte values with string-id keys:
//   swap-erase, shrink to fit and copy assignment (fn_7100904c48 is its reserve);
// - fn_7100903e88: its reflected iterator value's move assignment;
// - fn_7100900414: the destructor (owned object at +0xc00, shared one at +0xc88, five file-path ids at
//   +0xc18, the registration at +0xbe8), then the base one (fn_7100a45a84);
// - fn_7100900610: the base step, then the char class (+0x10 cast, kept at +0xc48) and its four
//   file-path ids copied in when the flag at +0x21 is set;
// - fn_7100901dd4, fn_7100901fe8, fn_710090219c: an actor entering / touching the surface, with the
//   splash ratio of fn_7100901900 (CLiquidPool_SetLevelRatio_A5_b70.cpp) inlined;
// - fn_710090248c, fn_7100902334: setting the level (moved over time when fn_710024fb2c holds).
// Names and layouts are placeholders.
#include <new>

#include "base/UnkCollectionIter.h"
#include "base/UnkDeferredSet.h"
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CActorComponent.h"
#include "game/CBlackboard.h"
#include "game/CCharClassBaseDamageTriggerComponent.h"
#include "game/CCharClassBaseTriggerComponent.h"
#include "game/CCharClassLiquidPoolComponent.h"
#include "game/CLiquidSimulationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkLiquidPool_b44a2;
struct CFreezeRoomComponent;

struct UnkLiquidPoolOwner_ovb0075 {
    u8 pad_0x0[0x388];
    UnkVec3f field_0x388;
};


struct UnkVec2f_ovb0075 {
    f32 x, y;
    UnkVec2f_ovb0075 operator+(const UnkVec2f_ovb0075& o) const { return {x + o.x, y + o.y}; }
    UnkVec2f_ovb0075 operator-(const UnkVec2f_ovb0075& o) const { return {x - o.x, y - o.y}; }
    UnkVec2f_ovb0075 operator*(f32 k) const { return {x * k, y * k}; }
};

struct UnkLiquidPoolBox_ovb0075 {
    void** vtable;
    u8 pad_0x8[0x38];
    UnkVec2f_ovb0075 min;  // 0x40
    UnkVec2f_ovb0075 max;  // 0x48
    u8 pad_0x50[0x10];
    bool dirty;  // 0x60
};

struct UnkLiquidPoolSelf_ovb0075 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkLiquidPoolOwner_ovb0075* owner;  // 0x30
    u8 pad_0x38[0x50];
    UnkLiquidPoolBox_ovb0075* box;  // 0x88
    void* box_used;                 // 0x90
    u8 pad_0x98[0xb50];
    base::UnkDeferredSet<void>* field_0xbe8;  // 0xbe8
    u8 pad_0xbf0[0x21];
    bool field_0xc11;
    u8 pad_0xc12[0x3e];
    f32 level;   // 0xc50
    f32 start;   // 0xc54
    f32 target;  // 0xc58
    u8 pad_0xc5c[0x8];
    UnkVec3f center;  // 0xc64
    UnkVec2f_ovb0075 size;     // 0xc70
};

void fn_7100a46f74(UnkLiquidPoolSelf_ovb0075* self);

void fn_7100900b40(UnkLiquidPoolSelf_ovb0075* self) {
    fn_7100a46f74(self);
    bool active = reinterpret_cast<char*>(g_unk_7101d47310)[0x2441] ? true : self->field_0xc11;
    reinterpret_cast<void (*)(UnkLiquidPoolSelf_ovb0075*, bool)>(self->vtable[0x208 / 8])(self, active);
    base::UnkDeferredSet<void>** slot = &self->field_0xbe8;
    char* manager = reinterpret_cast<char*>(g_unk_7101d47310);
    if (*slot) {
        fn_71004f3ea4(*slot, slot);
        *slot = nullptr;
    }
    *slot = reinterpret_cast<base::UnkDeferredSet<void>*>(manager + 0x1e28);
    fn_71004f3cf0(*slot, slot);
    UnkLiquidPoolOwner_ovb0075* owner = self->owner;
    owner->field_0x388 = reinterpret_cast<UnkVec3f (*)(UnkLiquidPoolSelf_ovb0075*)>(self->vtable[0x178 / 8])(self);
}

void fn_7100900c44(UnkLiquidPoolSelf_ovb0075* self) {
    fn_7100a46e78(reinterpret_cast<CFreezeRoomComponent*>(self));
    self->target = 1.0f;
    self->level = 1.0f;
    self->start = 1.0f;
    if (self->box_used) {
        UnkLiquidPoolBox_ovb0075* box = self->box;
        if (box->dirty)
            reinterpret_cast<void (*)(UnkLiquidPoolBox_ovb0075*)>(box->vtable[0x98 / 8])(box);
        UnkVec2f_ovb0075 a = box->min;
        UnkVec2f_ovb0075 b = box->max;
        UnkVec2f_ovb0075 c = (a + b) * 0.5f;
        self->center = UnkVec3f(c.x, c.y, 0.0f);
        self->size = b - a;
    }
    CBlackboard* blackboard = &g_unk_7101d47310->field_0x968;
    reinterpret_cast<void (*)(UnkLiquidPoolSelf_ovb0075*, CBlackboard*)>(self->vtable[0x140 / 8])(self, blackboard);
    fn_7100900cf4(reinterpret_cast<UnkLiquidPool_b44a2*>(self));
}

using base::global::UnkStringData;
using base::global::UnkStringPool;

union UnkLiquidKeySlot_ovb0075 {
    UnkStringData* p;
};

struct UnkLiquidKey_ovb0075 {
    UnkStringData* data;

    UnkLiquidKey_ovb0075(UnkLiquidKey_ovb0075&& other) : data(other.data) {
        reinterpret_cast<UnkLiquidKeySlot_ovb0075*>(&other.data)->p = &g_unk_7101d47108->empty;
    }
    ~UnkLiquidKey_ovb0075() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

typedef u32 __attribute__((may_alias)) UnkAliasU32_ovb0075;

struct UnkLiquidValue_ovb0075 {
    u32 x, y, z, w;
    UnkLiquidValue_ovb0075(const UnkLiquidValue_ovb0075& o) {
        UnkAliasU32_ovb0075* dst = reinterpret_cast<UnkAliasU32_ovb0075*>(this);
        const UnkAliasU32_ovb0075* src = reinterpret_cast<const UnkAliasU32_ovb0075*>(&o);
        dst[0] = src[0];
        dst[1] = src[1];
        dst[2] = src[2];
        dst[3] = src[3];
    }
    // Copied through words that may alias anything: the original reloads the count after it.
    UnkLiquidValue_ovb0075& operator=(const UnkLiquidValue_ovb0075& o) {
        UnkAliasU32_ovb0075* dst = reinterpret_cast<UnkAliasU32_ovb0075*>(this);
        const UnkAliasU32_ovb0075* src = reinterpret_cast<const UnkAliasU32_ovb0075*>(&o);
        dst[0] = src[0];
        dst[1] = src[1];
        dst[2] = src[2];
        dst[3] = src[3];
        return *this;
    }
};

struct UnkLiquidValueMap_ovb0075 {
    UnkLiquidValue_ovb0075* values;
    UnkLiquidKey_ovb0075* keys;
    u32 count;

    __attribute__((always_inline)) UnkLiquidValue_ovb0075* Erase(UnkLiquidValue_ovb0075* it) {
        u32 index = it - values;
        keys[index].~UnkLiquidKey_ovb0075();
        if (index != count - 1) {
            values[index] = values[count - 1];
            u32 last = count - 1;
            new (&keys[index]) UnkLiquidKey_ovb0075(static_cast<UnkLiquidKey_ovb0075&&>(keys[last]));
            keys[count - 1].~UnkLiquidKey_ovb0075();
        }
        --count;
        return &values[index];
    }
};

UnkLiquidValue_ovb0075* fn_7100901264(UnkLiquidValueMap_ovb0075* self, UnkLiquidValue_ovb0075* it) {
    return self->Erase(it);
}

using base::global::CStrId;

struct UnkLiquidValueArray_ovb0075 {
    UnkLiquidValue_ovb0075* values;
    CStrId* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};

void fn_7100904f7c(UnkLiquidValueArray_ovb0075* self) {
    if (self->count == self->capacity)
        return;
    auto* values = static_cast<UnkLiquidValue_ovb0075*>(
        self->allocator->vf_0x40((u64)self->count * sizeof(UnkLiquidValue_ovb0075), 8, self->flags));
    auto* keys = static_cast<CStrId*>(self->allocator->vf_0x40((u64)self->count * sizeof(CStrId), 8, self->flags));
    if (UnkLiquidValue_ovb0075* src = self->values) {
        UnkLiquidValue_ovb0075* dst = values;
        for (u32 i = 0; i < self->count; ++i, ++dst, ++src)
            new (dst) UnkLiquidValue_ovb0075(*src);
        self->allocator->vf_0x48(self->values);
    }
    if (CStrId* old = self->keys) {
        u64 j = 0;
        for (u32 i = 0; i < self->count; ++i, ++j, ++old) {
            new (&keys[j]) CStrId(*old);
            old->Reset();
        }
        self->allocator->vf_0x48(self->keys);
    }
    self->values = values;
    self->keys = keys;
    self->capacity = self->count;
}

void fn_7100904c48(UnkPropertyArrays* self, u32 capacity);  // reserve

UnkLiquidValueArray_ovb0075* fn_7100904ddc(UnkLiquidValueArray_ovb0075* self, const UnkLiquidValueArray_ovb0075* other) {
    if (self == other)
        return self;
    if (self->keys) {
        for (u64 i = 0; i < self->count; ++i)
            self->keys[i].Reset();
    }
    self->count = 0;
    if (other->count != self->capacity) {
        fn_7100904f7c(self);
        fn_7100904c48(reinterpret_cast<UnkPropertyArrays*>(self), other->count);
    }
    self->count = other->count;
    for (u64 i = 0; i < self->count; ++i)
        new (&self->values[i]) UnkLiquidValue_ovb0075(other->values[i]);
    self->count = other->count;
    for (u64 i = 0; i < self->count; ++i) {
        base::global::UnkStringData* data = other->keys[i].data;
        __builtin_memcpy(&self->keys[i], &data, sizeof(data));
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    return self;
}

namespace {

struct UnkLiquidKey : base::global::CStrId {
    UnkLiquidKey& operator=(UnkLiquidKey&& o) {
        Reset();
        data = o.data;
        base::global::UnkStringData* empty = &g_unk_7101d47108->empty;
        __builtin_memcpy(&o.data, &empty, sizeof(empty));
        return *this;
    }
};

struct UnkLiquidVec4 {
    f32 x, y, z, w;
    UnkLiquidVec4& operator=(const UnkLiquidVec4& o) {
        x = o.x;
        y = o.y;
        z = o.z;
        w = o.w;
        return *this;
    }
};

using UnkLiquidIterValue = base::UnkIterValue<UnkLiquidVec4, UnkLiquidKey>;

}  // namespace

void* fn_7100903e88(void* dst, void* src) {
    UnkLiquidIterValue* self = static_cast<UnkLiquidIterValue*>(dst);
    UnkLiquidIterValue* other = static_cast<UnkLiquidIterValue*>(src);
    if (other->state == 1) {
        self->Index() = static_cast<UnkLiquidKey&&>(other->value.index);
        self->Element() = other->value.element;
    } else if (other->state == 2) {
        self->Index() = static_cast<UnkLiquidKey&&>(*const_cast<UnkLiquidKey*>(other->ref.index));
        self->Element() = *other->ref.element;
    }
    return self;
}


// A file-path id released as CStrId::Reset does (the count re-read after the decrement).
struct UnkLiquidPath_ovb0075 {
    UnkStringData* data;

    void Reset() {
        if (UnkStringPool* pool = g_unk_7101d47200) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47200;
                }
            }
            data = &pool->empty;
        }
    }
};

extern void* g_unk_7101a80ae8[];

struct UnkLiquidOwned_ovb0075 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Release();  // 0x10
};

struct UnkLiquidShared_ovb0075 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Release();  // 0x10
    s32 ref_count;           // 0x8
};

struct UnkLiquidPoolDtor_ovb0075 {
    void** vtable;
    u8 pad_0x8[0xbe0];
    base::UnkDeferredSet<void>* registered_in;  // 0xbe8
    u64 field_0xbf0;
    u64 field_0xbf8;
    UnkLiquidOwned_ovb0075* owned;  // 0xc00
    u8 pad_0xc08[0x10];
    UnkLiquidPath_ovb0075 paths[5];  // 0xc18
    u8 pad_0xc40[0x48];
    UnkLiquidShared_ovb0075* shared;  // 0xc88
};
static_assert(__builtin_offsetof(UnkLiquidPoolDtor_ovb0075, shared) == 0xc88, "layout");

void fn_7100900414(UnkLiquidPoolDtor_ovb0075* self) {
    self->vtable = &g_unk_7101a80ae8[2];
    if (self->owned) {
        self->owned->Release();
        self->owned = nullptr;
    }
    if (UnkLiquidShared_ovb0075* shared = self->shared) {
        if (--shared->ref_count == 0)
            shared->Release();
    }
    for (s32 i = 4; i >= 0; --i)
        self->paths[i].Reset();
    if (self->registered_in) {
        fn_71004f3ea4(self->registered_in, &self->registered_in);
        self->registered_in = nullptr;
    }
    self->field_0xbf8 = 0;
    self->field_0xbf0 = 0;
    fn_7100a45a84(self);
}

using base::reflection::CClass;
using base::reflection::UnkReflObject;

struct UnkSteamJetComponent;
struct UnkSteamJetArg;

// A file-path id copied as CStrId's assignment, on the file-path pool.
struct UnkLiquidPathId_ovb0075 {
    UnkStringData* data;

    UnkLiquidPathId_ovb0075& operator=(const UnkLiquidPathId_ovb0075& other) {
        if (data != other.data) {
            UnkStringData* d = other.data;
            UnkStringPool* pool = g_unk_7101d47200;
            if (d != &pool->empty)
                ++d->ref_count;
            if (data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0)
                    fn_71000803a4(pool, data);
            }
            data = d;
        }
        return *this;
    }
};

struct UnkCharClassLiquidPool_ovb0075 {
    u8 pad_0x0[0x60];
    UnkLiquidPathId_ovb0075 paths[4];  // 0x60
};

struct UnkLiquidPoolInit_ovb0075 {
    u8 pad_0x0[0x10];
    UnkReflObject* field_0x10;
    u8 pad_0x18[0x9];
    bool field_0x21;
    u8 pad_0x22[0xc20 - 0x22];
    UnkLiquidPathId_ovb0075 paths[4];  // 0xc20
    u8 pad_0xc40[0x8];
    UnkCharClassLiquidPool_ovb0075* char_class;  // 0xc48
};
static_assert(__builtin_offsetof(UnkLiquidPoolInit_ovb0075, char_class) == 0xc48, "layout");

extern CClass g_unk_7172734230 __attribute__((visibility("hidden")));

static inline const CClass* LiquidPoolClass() {
    fn_71008fdfbc();
    return &g_unk_7172734230;
}

bool fn_7100900610(UnkLiquidPoolInit_ovb0075* self, UnkSteamJetArg* arg) {
    bool result = fn_7100a46e00(reinterpret_cast<UnkSteamJetComponent*>(self), arg);
    UnkReflObject* obj = self->field_0x10;
    self->char_class = obj && fn_710009d50c(obj->vf_0x0(), LiquidPoolClass())
                           ? reinterpret_cast<UnkCharClassLiquidPool_ovb0075*>(obj)
                           : nullptr;
    if (self->field_0x21) {
        self->paths[0] = self->char_class->paths[0];
        self->paths[1] = self->char_class->paths[1];
        self->paths[2] = self->char_class->paths[2];
        self->paths[3] = self->char_class->paths[3];
    }
    return result;
}

struct UnkLiquidPoolTarget_ovb0075;

struct UnkLiquidPoolFx_ovb0075 {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkLiquidPoolEnter_ovb0075 {
    void** vtable;
    u8 pad_0x8[0x9b0];
    s32 field_0x9b8;
    u8 pad_0x9bc[0x24c];
    u32 field_0xc08;
    u8 pad_0xc0c[0x7c];
    UnkLiquidPoolFx_ovb0075* fx;  // 0xc88

    UnkVec3f vf_0x178() { return reinterpret_cast<UnkVec3f (*)(UnkLiquidPoolEnter_ovb0075*)>(vtable[0x178 / 8])(this); }
    UnkVec3f vf_0x180() { return reinterpret_cast<UnkVec3f (*)(UnkLiquidPoolEnter_ovb0075*)>(vtable[0x180 / 8])(this); }
    s32 vf_0x258() { return reinterpret_cast<s32 (*)(UnkLiquidPoolEnter_ovb0075*)>(vtable[0x258 / 8])(this); }

    f32 SurfaceY() { return vf_0x180().y + vf_0x178().y * 0.5f; }
    inline void SetRatio(const f32* height, u32 a, bool b);
};

// fn_7100901900 (CLiquidPool_SetLevelRatio_A5_b70.cpp), inlined here.
inline void UnkLiquidPoolEnter_ovb0075::SetRatio(const f32* height, u32 a, bool b) {
    f32 h = *height;
    UnkVec3f pos = vf_0x180();
    f32 base = pos.x;
    UnkVec3f ext = vf_0x178();
    f32 extent = ext.x;
    UnkVec3f size = vf_0x178();
    if (fx != nullptr && fx->field_0x10 != nullptr) {
        f32 width = size.x;
        f32 ratio = (h - (base + extent * -0.5f)) / width;
        fn_7100905d8c(fx->field_0x10, ratio, a, b);
    }
}

void fn_7100a47164(UnkLiquidPoolEnter_ovb0075* self, UnkLiquidPoolTarget_ovb0075* target, const UnkVec3f* pos);
void fn_7100a4716c(UnkLiquidPoolEnter_ovb0075* self, UnkLiquidPoolTarget_ovb0075* target, const UnkVec3f* pos);
void fn_7100ecf3d4(UnkLiquidPoolTarget_ovb0075* target, const UnkVec3f* pos, s32 kind, u32 param);
void fn_7100ecf450(UnkLiquidPoolTarget_ovb0075* target, const UnkVec3f* pos);
void fn_71009014a8(UnkLiquidPoolEnter_ovb0075* self, UnkVec3f pos);

void fn_7100901fe8(UnkLiquidPoolEnter_ovb0075* self, UnkLiquidPoolTarget_ovb0075* target, const UnkVec3f* pos) {
    UnkVec3f at(pos->x, self->SurfaceY(), pos->z);
    fn_7100a47164(self, target, &at);
    if (!target)
        return;
    if (__builtin_fabsf(pos->y - self->SurfaceY()) < 50.0f) {
        fn_7100ecf3d4(target, &at, self->vf_0x258(), self->field_0xc08);
        self->SetRatio(&at.x, 1, true);
        fn_71009014a8(self, at);
    }
}

void fn_710090219c(UnkLiquidPoolEnter_ovb0075* self, UnkLiquidPoolTarget_ovb0075* target, const UnkVec3f* pos) {
    UnkVec3f at(pos->x, self->SurfaceY(), pos->z);
    fn_7100a4716c(self, target, &at);
    if (!target)
        return;
    if (__builtin_fabsf(pos->y - self->SurfaceY()) < 50.0f) {
        fn_7100ecf450(target, &at);
        self->SetRatio(&at.x, 6, true);
        fn_71009014a8(self, at);
    }
}

struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager*);

struct UnkLiquidPoolActor_ovb0075 {
    u8 pad_0x0[0x1b5];
    bool field_0x1b5;
    u8 pad_0x1b6[0x8a];
    struct UnkLiquidPoolActorPart_ovb0075* field_0x240;
};

struct UnkLiquidPoolActorPart_ovb0075 {
    void** vtable;

    bool vf_0x638() { return reinterpret_cast<bool (*)(UnkLiquidPoolActorPart_ovb0075*)>(vtable[0x638 / 8])(this); }
};

struct UnkLiquidPoolMover_ovb0075 {
    void** vtable;

    void vf_0x2f0(const UnkVec3f* pos) {
        reinterpret_cast<void (*)(UnkLiquidPoolMover_ovb0075*, const UnkVec3f*)>(vtable[0x2f0 / 8])(this, pos);
    }
};

void fn_7100a45f08(UnkLiquidPoolEnter_ovb0075* self, UnkLiquidPoolActor_ovb0075* actor, const UnkVec3f* pos);
s32 fn_7100f0d7b0(void* list, UnkLiquidPoolEnter_ovb0075* self, const UnkVec3f* pos);
UnkLiquidPoolMover_ovb0075* fn_71009019bc(UnkLiquidPoolActor_ovb0075* actor);
void fn_71010ff50c(UnkLiquidPoolActorPart_ovb0075* part, bool a, bool b);

void fn_7100901dd4(UnkLiquidPoolEnter_ovb0075* self, UnkLiquidPoolActor_ovb0075* actor, const UnkVec3f* pos) {
    fn_7100a45f08(self, actor, pos);
    if (self->field_0x9b8 == 0 && !actor->field_0x1b5 && fn_7100df5c38(g_unk_7101d47310, actor)) {
        char* player = reinterpret_cast<char*>(fn_7100dec788(g_unk_7101d47310));
        if (fn_7100f0d7b0(reinterpret_cast<char*>(g_unk_7101d47310) + 0x38, self, pos) == 0)
            self->SetRatio(reinterpret_cast<const f32*>(player + 0x44), 4, true);
        return;
    }
    if (UnkLiquidPoolMover_ovb0075* mover = fn_71009019bc(actor)) {
        mover->vf_0x2f0(pos);
        self->SetRatio(&pos->x, 6, true);
        fn_71009014a8(self, *pos);
    } else if (UnkLiquidPoolActorPart_ovb0075* part = actor->field_0x240) {
        if (part->vf_0x638())
            fn_71010ff50c(part, false, false);
    }
}

struct UnkLiquidPoolLevel;
struct UnkStruct_710024fa0c;
struct UnkActivatable_b48;

struct UnkLiquidPoolShape_ovb0075 {
    void** vtable;

    s32 vf_0x18() { return reinterpret_cast<s32 (*)(UnkLiquidPoolShape_ovb0075*)>(vtable[0x18 / 8])(this); }
};

struct UnkLiquidPoolBody_ovb0075 {
    u8 pad_0x0[0x158];
    UnkLiquidPoolShape_ovb0075* shape;  // 0x158
};

struct UnkLiquidPoolOwner2_ovb0075 {
    u8 pad_0x0[0x228];
    UnkStruct_710024fa0c* field_0x228;
    u8 pad_0x230[0x70];
    UnkStruct_710024fa0c* field_0x2a0;
};

struct UnkLiquidPoolSet_ovb0075 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkLiquidPoolOwner2_ovb0075* owner;  // 0x30
    u8 pad_0x38[0x40];
    bool field_0x78;
    f32 field_0x7c;
    u8 pad_0x80[0x8];
    UnkLiquidPoolBody_ovb0075* body;  // 0x88
    u8 pad_0x90[0xbc0];
    f32 level;     // 0xc50
    f32 start;     // 0xc54
    f32 target;    // 0xc58
    f32 duration;  // 0xc5c
    f32 elapsed;   // 0xc60
    UnkVec3f center;        // 0xc64
    UnkVec2f_ovb0075 size;  // 0xc70

    void vf_0x88() { reinterpret_cast<void (*)(UnkLiquidPoolSet_ovb0075*)>(vtable[0x88 / 8])(this); }
    UnkVec3f vf_0x178() { return reinterpret_cast<UnkVec3f (*)(UnkLiquidPoolSet_ovb0075*)>(vtable[0x178 / 8])(this); }
    UnkVec3f vf_0x180() { return reinterpret_cast<UnkVec3f (*)(UnkLiquidPoolSet_ovb0075*)>(vtable[0x180 / 8])(this); }
    f32 vf_0x260() { return reinterpret_cast<f32 (*)(UnkLiquidPoolSet_ovb0075*)>(vtable[0x260 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkLiquidPoolSet_ovb0075, size) == 0xc70, "layout");

void fn_7100a48de8(UnkLiquidPoolSet_ovb0075* self, const UnkVec3f* size, const UnkVec3f* center);

// fn_7100900928 applies a level; its declaration elsewhere (CCharClassLiquidPoolComponent_Level_b50.cpp)
// leaves out the level argument it takes.
static inline void ApplyLevel(UnkLiquidPoolSet_ovb0075* self, f32 level) {
    reinterpret_cast<void (*)(UnkLiquidPoolSet_ovb0075*, f32)>(fn_7100900928)(self, level);
}

static inline bool NearlyEqual(f32 a, f32 b) {
    return a + 1e-5f >= b && a - 1e-5f <= b;
}

static inline bool ShapeIsKind4(UnkLiquidPoolSet_ovb0075* self) {
    UnkLiquidPoolShape_ovb0075* shape = self->body->shape;
    return shape && shape->vf_0x18() == 4;
}

void fn_710090248c(UnkLiquidPoolSet_ovb0075* self, f32 level) {
    self->field_0x7c = level;
    if (!self->field_0x78)
        self->vf_0x88();
    if (!ShapeIsKind4(self)) {
        {
            UnkVec3f size;
            size.x = self->size.x;
            size.y = self->size.y;
            size.z = 0.0f;
            fn_7100a48de8(self, &size, &self->center);
        }
        // Both read back and left unused.
        UnkVec3f position = self->vf_0x180();
        UnkVec3f extent = self->vf_0x178();
        (void)position;
        (void)extent;
    }
    if (fn_710024fb2c(reinterpret_cast<UnkActivatable_b48*>(self))) {
        f32 duration = self->vf_0x260();
        __builtin_memcpy(&self->start, &self->level, sizeof(f32));
        self->duration = duration;
        self->elapsed = 0.0f;
    } else {
        self->start = level;
        self->level = level;
    }
    self->target = level;
    if (NearlyEqual(self->level, level))
        ApplyLevel(self, level);
    fn_7100900cf4(reinterpret_cast<UnkLiquidPool_b44a2*>(self));
}

void fn_7100902334(UnkLiquidPoolSet_ovb0075* self, f32 level) {
    self->field_0x7c = level;
    if (!self->field_0x78)
        self->vf_0x88();
    fn_710024fa0c(reinterpret_cast<UnkStruct_710024fa0c*>(self), true);
    if (!ShapeIsKind4(self)) {
        UnkVec3f size;
        size.x = self->size.x;
        size.y = self->size.y;
        size.z = 0.0f;
        fn_7100a48de8(self, &size, &self->center);
    }
    if (fn_710024fb2c(reinterpret_cast<UnkActivatable_b48*>(self))) {
        f32 duration = self->vf_0x260();
        __builtin_memcpy(&self->start, &self->level, sizeof(f32));
        self->duration = duration;
        self->elapsed = 0.0f;
    } else {
        self->start = level;
        self->level = level;
    }
    self->target = level;
    if (self->owner->field_0x2a0)
        fn_710024fa0c(self->owner->field_0x2a0, true);
    if (self->owner->field_0x228)
        fn_710024fa0c(self->owner->field_0x228, true);
    if (NearlyEqual(self->level, self->target))
        ApplyLevel(self, level);
    fn_7100900cf4(reinterpret_cast<UnkLiquidPool_b44a2*>(self));
}