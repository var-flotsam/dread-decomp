// CCubeMapComponent's unit (tools/tu_map.py). The destructor body (fn_710079fa98): the vtable, the
// two render objects at +0x50 and +0x60 taken off the render manager (fn_71002199d8, layer 0) and
// cleared, the string id at +0x88 reset to the pool's empty id, the counted resources at +0x68 and
// +0x58 released, then the component destructor fn_71010c4194. The update (fn_710079f5c0): when the
// owner's position (the translation column of its transform at +0x38) moved, it is passed to the
// members at +0x18 of both render objects (fn_71001f3964) and kept at +0x40; the owner's cached angles
// are refreshed; then the intensity fade (+0x70 current, +0x74 from, +0x78 to, +0x7c duration, +0x80
// time) advances and the current value is written to both render objects. A reflected setter of the
// intensity (fn_71007a0b7c), and the paths of the resources behind the two render objects (the path
// at +0x10 of a base::res::CRes) or a shared empty path (fn_710079ff50, fn_71007a0310). Names and
// layouts are placeholders.
#include "base/UnkAtExit.h"
#include "base/UnkCountedResource.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkComponent.h"
#include "game/UnkRenderManager.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;
using base::reflection::CType;
using base::reflection::UnkReflObject;
using base::reflection::UnkReflValue;

struct UnkCubeMapArg;
void fn_71001f3964(void* target, const UnkCubeMapArg* value);

extern void* g_unk_7101a6fdc8[];

struct UnkCubeMapComponent_ovb0033 {
    void** vtable;
    u8 pad_0x8[0x48];
    UnkItemBase_71011a91c4* render_a;  // 0x50
    UnkCountedResource* res_a;         // 0x58
    UnkItemBase_71011a91c4* render_b;  // 0x60
    UnkCountedResource* res_b;         // 0x68
    u8 pad_0x70[0x18];
    UnkStringData* name;  // 0x88, a CStrId
};

static inline void Release(UnkCountedResource* res) {
    if (res && --res->ref_count == 0)
        fn_710001737c(res);
}

void fn_710079fa98(UnkCubeMapComponent_ovb0033* self) {
    self->vtable = g_unk_7101a6fdc8 + 2;
    fn_71002199d8(g_unk_7101d47300, self->render_a, 0);
    fn_71002199d8(g_unk_7101d47300, self->render_b, 0);
    self->render_a = nullptr;
    self->render_b = nullptr;
    if (UnkStringPool* pool = g_unk_7101d47108) {
        UnkStringData** name = &self->name;
        if (*name && *name != &pool->empty) {
            --(*name)->ref_count;
            if ((*name)->ref_count == 0) {
                fn_71000803a4(pool, *name);
                pool = g_unk_7101d47108;
            }
        }
        *name = &pool->empty;
    }
    Release(self->res_b);
    Release(self->res_a);
    fn_71010c4194(reinterpret_cast<UnkBaseComponent*>(self));
}

struct UnkCubeMapOwner_ovb0033 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
};

struct UnkCubeMapRenderData2_ovb0033 {
    u8 pad_0x0[0x7c];
    f32 intensity;  // 0x7c
};

struct UnkCubeMapRender2_ovb0033 {
    u8 pad_0x0[0x18];
    u8 field_0x18[0x110];
    UnkCubeMapRenderData2_ovb0033* data;  // 0x128
};

struct UnkCubeMapUpdate_ovb0033 {
    u8 pad_0x0[0x30];
    UnkCubeMapOwner_ovb0033* owner;  // 0x30
    u8 pad_0x38[0x8];
    f32 pos[3];  // 0x40
    u8 pad_0x4c[0x4];
    UnkCubeMapRender2_ovb0033* render_a;  // 0x50
    u8 pad_0x58[0x8];
    UnkCubeMapRender2_ovb0033* render_b;  // 0x60
    u8 pad_0x68[0x8];
    f32 value;     // 0x70
    f32 from;      // 0x74
    f32 to;        // 0x78
    f32 duration;  // 0x7c
    f32 time;      // 0x80
};

static inline f32 AsF32(u32 bits) {
    f32 f;
    __builtin_memcpy(&f, &bits, 4);
    return f;
}

#define UNK_CHANGED(a, b, eps) ((a) - (eps) > (b) || (a) + (eps) < (b))

void fn_710079f5c0(UnkCubeMapUpdate_ovb0033* self, f32 dt) {
    UnkCubeMapOwner_ovb0033* owner = self->owner;
    const u32* m = reinterpret_cast<const u32*>(owner->xform.m);
    u32 x = m[3];
    u32 y = m[7];
    u32 z = m[11];
    alignas(8) u32 p[3];
    p[0] = x;
    p[1] = y;
    p[2] = z;
    if (UNK_CHANGED(AsF32(p[0]), self->pos[0], 0.001f) || UNK_CHANGED(AsF32(p[1]), self->pos[1], 0.001f) ||
        UNK_CHANGED(AsF32(p[2]), self->pos[2], 0.001f)) {
        fn_71001f3964(self->render_a->field_0x18, reinterpret_cast<const UnkCubeMapArg*>(p));
        fn_71001f3964(self->render_b->field_0x18, reinterpret_cast<const UnkCubeMapArg*>(p));
        u32* dst = reinterpret_cast<u32*>(self->pos);
        dst[0] = p[0];
        dst[1] = p[1];
        dst[2] = p[2];
    }
    self->owner->xform.GetAngle();

    f32 to = self->to;
    if (!(self->value - 0.00001f <= to && self->value + 0.00001f >= to)) {
        f32 time = self->time + dt;
        self->time = time;
        if (time < self->duration) {
            f32 t = time / self->duration;
            t = t > 1.0f ? 1.0f : t;
            to = self->from + t * (to - self->from);
        }
        self->value = to;
        __builtin_memcpy(&self->render_a->data->intensity, &to, 4);
        UnkCubeMapRender2_ovb0033* render_b = self->render_b;
        render_b->data->intensity = self->value;
    }
}

struct UnkCubeMapRenderData_ovb0033 {
    u8 pad_0x0[0x7c];
    f32 intensity;  // 0x7c
};

struct UnkCubeMapRender_ovb0033 {
    u8 pad_0x0[0x128];
    UnkCubeMapRenderData_ovb0033* data;  // 0x128
};

struct UnkCubeMap_ovb0033 {
    u8 pad_0x0[0x50];
    UnkCubeMapRender_ovb0033* render_a;  // 0x50
    u8 pad_0x58[0x8];
    UnkCubeMapRender_ovb0033* render_b;  // 0x60
    u8 pad_0x68[0x8];
    f32 value;     // 0x70
    f32 from;      // 0x74
    f32 to;        // 0x78
    f32 duration;  // 0x7c
    f32 time;      // 0x80
};

static inline void SetNow(UnkCubeMap_ovb0033* self, f32 v) {
    self->render_a->data->intensity = v;
    self->render_b->data->intensity = v;
    self->to = v;
    self->from = v;
    self->value = v;
}

UnkReflValue fn_71007a0b7c(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* arg = args[0];
    UnkCubeMap_ovb0033* self = *reinterpret_cast<UnkCubeMap_ovb0033**>(obj + 8);
    SetNow(self, *base::reflection::refl_ArgValue<f32>(arg));
    return base::reflection::refl_EmptyValue();
}

extern "C" int __cxa_guard_acquire(u64* guard);
extern "C" void __cxa_guard_release(u64* guard);

struct UnkCubeMapRes_ovb0033 : UnkReflObject {
    u8 pad_0x8[0x8];
    CFilePathStrId path;  // 0x10
};

struct UnkCubeMapResRef_ovb0033 {
    u8 pad_0x0[0x8];
    UnkCubeMapRes_ovb0033* res;  // 0x8
};

struct UnkCubeMapResOwner_ovb0033 {
    u8 pad_0x0[0x50];
    UnkCubeMapResRef_ovb0033* ref_a;  // 0x50
    u8 pad_0x58[0x8];
    UnkCubeMapResRef_ovb0033* ref_b;  // 0x60
};

// An empty path and the guard of its one-time construction.
struct UnkEmptyPathSlot_ovb0033 {
    u8 path[8];
    u64 guard;
};

// The two empty paths (hidden: reached directly).
extern UnkEmptyPathSlot_ovb0033 g_unk_7172718d90 __attribute__((visibility("hidden")));
extern UnkEmptyPathSlot_ovb0033 g_unk_7172718da0 __attribute__((visibility("hidden")));

static inline bool IsRes(UnkCubeMapRes_ovb0033* res) {
    return fn_710009d50c(res->vf_0x0(), reinterpret_cast<const CType*>(fn_71000e8a5c()));
}

static inline UnkCubeMapRes_ovb0033* AsRes(UnkCubeMapRes_ovb0033* res) {
    return res && IsRes(res) ? res : nullptr;
}

__attribute__((always_inline)) static inline const CFilePathStrId* EmptyPath(UnkEmptyPathSlot_ovb0033* slot) {
    u8* storage = slot->path;
    u64* guard = &slot->guard;
    if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(guard), __ATOMIC_ACQUIRE) & 1), 0)) {
        if (__cxa_guard_acquire(guard)) {
            new (storage) CFilePathStrId(g_unk_710162e2ed, true);
            fn_7100000250(fn_7100000c7c, storage, g_unk_7101a19000);
            __cxa_guard_release(guard);
        }
    }
    return reinterpret_cast<const CFilePathStrId*>(storage);
}

__attribute__((always_inline)) static inline const CFilePathStrId* PathOf(UnkCubeMapResOwner_ovb0033* self,
                                                                         UnkCubeMapResRef_ovb0033* const* ref,
                                                                         UnkEmptyPathSlot_ovb0033* slot) {
    const CFilePathStrId* empty = EmptyPath(slot);
    if (*ref && (*ref)->res && IsRes((*ref)->res))
        return &AsRes((*ref)->res)->path;
    return empty;
}

const CFilePathStrId* fn_710079ff50(UnkCubeMapResOwner_ovb0033* self) {
    return PathOf(self, &self->ref_a, &g_unk_7172718d90);
}

const CFilePathStrId* fn_71007a0310(UnkCubeMapResOwner_ovb0033* self) {
    return PathOf(self, &self->ref_b, &g_unk_7172718da0);
}
