// CEmmyAIComponent's unit (tools/tu_map.py unit 811; no reliable name): growable-array helpers (the element
// accessor of two arrays and the copy assignment of the 0x100-byte records), the destructor of a
// component with three arrays and two file path ids, the Emmy AI's speed factor, the navigation query it
// builds for a target and a navigation-cell test. Names and layouts are placeholders.
#define UNK_VECTOR_GROW_AT_ATTR __attribute__((always_inline))
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CAnimationComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkNavMeshCell.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkObject_0x100.h"
#include "game/UnkTU357.h"
#include "game/UnkTU363.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

void fn_71002b57bc(UnkObject_0x100* self);
struct UnkMsg_b152;

struct UnkRecord_b0194 {
    u64 raw[0x20];
    UnkRecord_b0194() { fn_71002b57bc(reinterpret_cast<UnkObject_0x100*>(this)); }
    UnkRecord_b0194(const UnkRecord_b0194& o) {
        fn_71002b58c0(reinterpret_cast<UnkObject_0x100*>(this), reinterpret_cast<const UnkObject_0x100*>(&o));
    }
    UnkRecord_b0194(UnkRecord_b0194&& o) {
        fn_71002b58c0(reinterpret_cast<UnkObject_0x100*>(this), reinterpret_cast<const UnkObject_0x100*>(&o));
    }
    ~UnkRecord_b0194() { fn_71002b6094(reinterpret_cast<UnkObject_0x100*>(this)); }
};
static_assert(sizeof(UnkRecord_b0194) == 0x100, "UnkRecord_b0194 layout");

struct UnkMsg_b0194 {
    u32 a, b, c, d;
    UnkMsg_b0194() { fn_71002878dc(reinterpret_cast<UnkMsg_b152*>(this)); }
    UnkMsg_b0194(UnkMsg_b0194&& o) : a(o.a), b(o.b), c(o.c), d(o.d) {}
    ~UnkMsg_b0194() { fn_7100287954(reinterpret_cast<UnkMsg_b152*>(this)); }
};

UnkRecord_b0194* fn_71005499f8(base::UnkVector<UnkRecord_b0194>* self, u32 index) {
    return &self->GrowAt(index);
}

UnkMsg_b0194* fn_710054cdac(base::UnkVector<UnkMsg_b0194>* self, u32 index) {
    return &self->GrowAt(index);
}

base::UnkVector<UnkRecord_b0194>* fn_7100563f88(base::UnkVector<UnkRecord_b0194>* self,
                                                const base::UnkVector<UnkRecord_b0194>* other) {
    if (self != other) {
        self->Clear();
        self->Reserve(other->capacity);
        for (const UnkRecord_b0194 *it = other->data, *end = other->data + other->count; it != end; ++it)
            self->PushBackCopy(*it);
    }
    return self;
}

using base::global::CFilePathStrId;

extern void* g_unk_7101a49310[];

struct UnkSlot_b0194 {
    u64* target;
    ~UnkSlot_b0194() { *target = 0; }
};

struct UnkComponentBase_b0194;

// A path id's release, with the count decremented and tested in two steps.
static inline void Release(CFilePathStrId* id) {
    base::global::UnkStringPool* pool = g_unk_7101d47200;
    if (pool) {
        if (id->data && id->data != &pool->empty) {
            --id->data->ref_count;
            if (id->data->ref_count == 0) {
                fn_71000803a4(pool, id->data);
                pool = g_unk_7101d47200;
            }
        }
        id->data = &pool->empty;
    }
}
void fn_7100573128(UnkComponentBase_b0194* self);  // base destructor

struct UnkComponentBase_b0194 {
    void** vtable;
    u8 pad_0x8[0x60];
    CFilePathStrId path;      // 0x68
    CFilePathStrId path_alt;  // 0x70
    u8 pad_0x78[8];
    base::UnkVector<UnkSlot_b0194> slots;  // 0x80
    base::UnkVector<void*> field_0xa8;
    base::UnkVector<void*> field_0xd0;
};
static_assert(__builtin_offsetof(UnkComponentBase_b0194, field_0xd0) == 0xd0, "layout");

void fn_710053f35c(UnkComponentBase_b0194* self) {
    self->vtable = g_unk_7101a49310 + 2;
    UnkSlot_b0194* end = self->slots.data + self->slots.count;
    for (UnkSlot_b0194* it = self->slots.data; it != end; ++it)
        it->~UnkSlot_b0194();
    self->slots.count = 0;
    self->field_0xa8.Clear();
    self->field_0xd0.Clear();
    self->field_0xd0.Free();
    self->field_0xa8.Free();
    if (self->slots.data) {
        self->slots.count = 0;
        self->slots.allocator->vf_0x48(self->slots.data);
        self->slots.capacity = 0;
    }
    Release(&self->path_alt);
    Release(&self->path);
    fn_7100573128(self);
}

// A call through slot `offset` of the object's vtable.
template <class R, class... A>
static inline R CallVf_b0194(void* self, u32 offset, A... args) {
    return reinterpret_cast<R (*)(void*, A...)>((*reinterpret_cast<void***>(self))[offset / 8])(self, args...);
}

struct UnkTakumakuComponent;

struct UnkTunable_b0194 {
    u8 pad_0x0[0xa8];
    f32 field_0xa8;
};

struct UnkClampRange_b0194 {
    u8 pad_0x0[0x18];
    f32 low;   // 0x18
    f32 high;  // 0x1c
};

struct UnkEmmyAI_b0194 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x29ee0 - 0x38];
    void* tunable_source;  // 0x29ee0
};

f32 fn_71005414c8(UnkEmmyAI_b0194* self) {
    f32 speed;
    UnkAnimationComponent* animation = static_cast<UnkAnimationComponent*>(self->owner->field_0x220);
    if (animation && fn_710117b348(animation, fn_7100c2c6f8()) && !CallVf_b0194<bool>(self, 0x840))
        speed = 1.0f;
    else
        speed = CallVf_b0194<f32>(self, 0x838);
    if (CallVf_b0194<bool>(self, 0x2f8)) {
        void* source = self->tunable_source;
        speed *= static_cast<UnkTunable_b0194*>(CallVf_b0194<void*>(source, 0xd8))->field_0xa8;
    }
    if (CallVf_b0194<bool>(self, 0x868)) {
        animation = static_cast<UnkAnimationComponent*>(self->owner->field_0x220);
        if (animation) {
            if (fn_710117ae38(animation, 0x51c528eba89eee2eul, false) ||
                fn_710117ae38(static_cast<UnkAnimationComponent*>(self->owner->field_0x220), 0x7bf6fde1ddb95b8bul, false) ||
                fn_710117ae38(static_cast<UnkAnimationComponent*>(self->owner->field_0x220), 0xa62793ffe86c29a5ul, false) ||
                fn_710117ae38(static_cast<UnkAnimationComponent*>(self->owner->field_0x220), 0x641c5e36ddd0abd8ul, false) ||
                (self->owner->field_0x220 &&
                 fn_7101178e6c(static_cast<UnkTakumakuComponent*>(self->owner->field_0x220), 10)))
                speed += speed;
        }
    }
    speed = !(speed >= 4.0f) ? speed : 4.0f;
    UnkTrackQuery query;
    query.cls = fn_7100c15948();
    query.result = nullptr;
    query.value = -1.0f;
    animation = static_cast<UnkAnimationComponent*>(self->owner->field_0x220);
    if (animation) {
        fn_710117b08c(animation, &query);
        UnkClampRange_b0194* range = reinterpret_cast<UnkClampRange_b0194*>(query.result);
        if (range) {
            if (speed <= range->low)
                return range->low;
            f32 high = range->high;
            return speed >= high ? high : speed;
        }
    }
    return speed;
}

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkNavCallback_b0194 {
    void* ctx;
    void* fn;
};

// The navigation query as the AI builds it: only the members set here are named.
struct UnkNavQuery {
    u8 pad_0x0[0x20];
    UnkNavCallback_b0194 filter;  // 0x20
    u8 pad_0x30[0x28];
    UnkWeakHandle* target;  // 0x58
    u32 flags;              // 0x60
    u8 pad_0x64[0xf0];
    u32 field_0x154;
    u8 pad_0x158[0x48];
    u8 field_0x1a0;
    u8 pad_0x1a1[3];
    f32 field_0x1a4;
    f32 field_0x1a8;
    u8 pad_0x1ac[0xc];
};
static_assert(sizeof(UnkNavQuery) == 0x1b8, "UnkNavQuery layout");

struct UnkStruct_7100286798;
struct UnkActor_7100286798;
struct UnkStruct_7100286624;
struct UnkOwner;

void fn_7100286920(UnkNavQuery* self, const CVector2D* point, u16 cell, u8 side);
UnkWeakHandle* fn_7100d78378(UnkWeakTarget* self);
f32 fn_7100569f34(UnkOwner* self, void* unused, u32 key, f32 value);
char* fn_7100569fd4(char* p0, char* p1, char* p2);
void* fn_7100ebfa74(UnkNavMeshManager* self, UnkNavQuery* query, UnkNavCallback_b0194* callback);

struct UnkTracked_b0194 {
    void** vtable;
    u16* vf_0x2d0() { return reinterpret_cast<u16* (*)(UnkTracked_b0194*)>(vtable[0x2d0 / 8])(this); }
};

struct UnkNavResult_b0194 {
    bool field_0x0;
    u32 field_0x4;
    void* field_0x8;
    u8 field_0x10;
    u8 pad_0x11[0xf];
    u32 field_0x20;
};

struct UnkNavAI_b0194 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x296c8 - 0x38];
    UnkNavResult_b0194 result;  // 0x296c8
};

void* fn_7100555e14(UnkNavAI_b0194* self, UnkActor* actor, u32 flag, u32 flags, bool a, u32 b) {
    UnkTracked_b0194* tracked = static_cast<UnkTracked_b0194*>(actor->field_0x1f8);
    if (!tracked || !tracked->vf_0x2d0())
        return nullptr;
    UnkNavResult_b0194* result = &self->result;
    result->field_0x0 = a;
    result->field_0x4 = b;
    result->field_0x8 = nullptr;
    result->field_0x10 = 0;
    result->field_0x20 = 0;
    UnkNavQuery query;
    fn_710028659c(&query);
    fn_7100286798(reinterpret_cast<UnkStruct_7100286798*>(&query), reinterpret_cast<UnkActor_7100286798*>(self->owner));
    AssignRef(&query.target, fn_7100d78378(reinterpret_cast<UnkWeakTarget*>(actor)));
    query.flags = flags;
    CVector2D point;
    {
        f32 x = actor->field_0x44;
        f32 y = actor->field_0x54;
        f32 z = actor->field_0x64;
        CVector3D position;
        position.x = x;
        position.y = y;
        position.z = z;
        fn_710007b430(&point, &position);
    }
    fn_7100286920(&query, &point, *tracked->vf_0x2d0(), 0xff);
    query.filter.ctx = self;
    query.filter.fn = reinterpret_cast<void*>(&fn_7100569f34);
    query.field_0x154 = ~flag & 1;
    query.field_0x1a4 = 45.0f;
    query.field_0x1a8 = 45.0f;
    query.field_0x1a0 = 1;
    if (!g_unk_71726a4510) {
        UnkNavMeshManager* m = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(m);
        g_unk_71726a4510 = m;
    }
    UnkNavCallback_b0194 callback;
    callback.ctx = self;
    callback.fn = reinterpret_cast<void*>(&fn_7100569fd4);
    void* path = fn_7100ebfa74(g_unk_71726a4510, &query, &callback);
    if (!path)
        path = result->field_0x8;
    fn_7100286624(reinterpret_cast<UnkStruct_7100286624*>(&query));
    return path;
}

struct UnkNavCandidate_b0194 {
    u8 pad_0x0[0x18];
    u32 field_0x18;
    u8 pad_0x1c[0x18];
    u32 field_0x34;
    u8 pad_0x38[0x14];
    bool field_0x4c;
    bool field_0x4d;
    u8 pad_0x4e[0xa];
    u16* cells;  // 0x58
    u32 cell_count;  // 0x60
};

struct UnkNavCellLink_b0194 {
    u8 pad_0x0[0x68];
    void* field_0x68;
};

struct UnkNavLinked_b0194 {
    u8 pad_0x0[0x611];
    bool field_0x611;
};

struct UnkCellAI_b0194 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x29ee0 - 0x38];
    UnkNavLinked_b0194* linked;  // 0x29ee0
    u8 pad_0x29ee8[0x811 - 0x8];
    bool field_0x2a6f1;
    u8 pad_0x2a6f2;
    bool field_0x2a6f3;
    bool field_0x2a6f4;
};

bool fn_71002b3c98(void* object);
bool fn_710110fbb4(UnkCellAI_b0194* self, UnkNavCandidate_b0194* candidate, u16 cell);

bool fn_71005576e4(UnkCellAI_b0194* self, UnkNavCandidate_b0194* candidate, u16 cell) {
    if (!self->linked->field_0x611) {
        if (candidate->field_0x18 || candidate->field_0x34)
            return false;
    }
    if (self->field_0x2a6f3 && candidate->field_0x4c)
        return false;
    if (self->field_0x2a6f4)
        return fn_710110fbb4(self, candidate, cell);
    if (!g_unk_71726a4510) {
        UnkNavMeshManager* m = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(m);
        g_unk_71726a4510 = m;
    }
    UnkNavMesh* mesh = g_unk_71726a4510->navmesh;
    if (mesh && candidate->field_0x4c && !candidate->field_0x18 && candidate->cell_count) {
        const u16* it = candidate->cells;
        const u16* end = it + candidate->cell_count;
        if (u16(~cell) != 0) {
            for (; it != end; ++it) {
                if (*it == 0xffff)
                    continue;
                UnkNavCellLink_b0194* link = reinterpret_cast<UnkNavCellLink_b0194*>(fn_710027a328(mesh, *it));
                void* object = link->field_0x68;
                if (!object)
                    continue;
                if (!fn_71002b005c(fn_710027a328(mesh, cell)) && fn_71002b3c98(object))
                    return false;
            }
        } else {
            for (; it != end; ++it) {
                if (*it == 0xffff)
                    continue;
                fn_710027a328(mesh, *it);
            }
        }
    }
    if (candidate->field_0x4d && self->field_0x2a6f1 && fn_7100d7c990(self->owner)) {
        if (u16(~cell) == 0)
            return false;
        if (mesh && !fn_71002b005c(fn_710027a328(mesh, cell)))
            return false;
    }
    return fn_710110fbb4(self, candidate, cell);
}
