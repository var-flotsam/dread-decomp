// Unit 259 (the graphics system): loads the render resources of the shaders in a fixed list of 24
// names and hands each to the program registry, keeping a reference; then looks up three
// programs. Names and layouts are placeholders.
#include <new>

#include "base/UnkCountedResource.h"
#include "base/UnkResKey.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTU259.h"
#include "game/UnkTU27.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntVector;
using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

// A file-path id held raw, as fn_7100169c74 returns it (src/game/msapi/res/CRenderRes/fn_71001b1dd0.cpp).
struct UnkPathId {
    UnkStringData* data;

    struct NoInit {};
    UnkPathId(NoInit) {}
    UnkPathId(UnkPathId&& other) {
        data = other.data;
        other.data = &g_unk_7101d47200->empty;
    }
    UnkPathId(const UnkPathId& other) : data(other.data) {
        if (&g_unk_7101d47200->empty != data)
            ++data->ref_count;
    }
    ~UnkPathId() {
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
    UnkPathId& operator=(UnkPathId&& other) {
        this->~UnkPathId();
        new (this) UnkPathId(static_cast<UnkPathId&&>(other));
        return *this;
    }
    UnkPathId& operator=(const UnkPathId& other) {
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

UnkPathId fn_7100169c74(const CStrId& name);

// The resource type the shaders are loaded as.
extern const UnkResType g_unk_716e453ef8;

// The shader names (reached through the GOT).
extern const CStrId* const g_unk_7101a1e620[24];

// The program registry (reached through the GOT; src/game_lo/fn_710014c790.cpp).
struct UnkRegistry_710014c790;
extern UnkRegistry_710014c790* unk_710b372d68;
void* fn_7100169be4(UnkRegistry_710014c790* registry, const void* key);
void fn_7100169d28(UnkRegistry_710014c790* registry, const CStrId& name, UnkCountedResource* resource);
void fn_71001545a0(void* program);
void fn_71001545b0(void* program);
void fn_71001545c0(void* program);

struct UnkGraphicsSystem;

struct UnkGfx_lcab0c {
    u8 pad_0x0[0xb0];
    CRntVector<UnkResRef> shader_resources;  // 0xb0
};

// Appends a reference (the array grows out of line; no room even then hangs, as in the original).
static inline void Append(CRntVector<UnkResRef>& vector, const UnkResRef& value) {
    u32 n = vector.count;
    u32 capacity = vector.capacity;
    if (capacity <= n && vector.step != 0)
        capacity = (n / vector.step + 1) * vector.step;
    if ((s32)n >= (s32)capacity)
        for (;;) {
        }
    fn_71000231f4(reinterpret_cast<CRntVector<u8>*>(&vector), capacity);
    UnkResRef* slot = new (&vector.data[vector.count]) UnkResRef();
    *slot = value;
    ++vector.count;
}

// The resource if it is a render resource (msapi::res::CRenderRes), else null.
static inline UnkCountedResource* AsRenderRes(UnkCountedResource* res) {
    if (!res)
        return nullptr;
    const base::reflection::UnkReflObject* obj = reinterpret_cast<const base::reflection::UnkReflObject*>(res);
    return fn_710009d50c(obj->vf_0x0(), fn_71001b122c()) ? res : nullptr;
}

void fn_71001cab0c(UnkGraphicsSystem* system) {
    UnkGfx_lcab0c* self = reinterpret_cast<UnkGfx_lcab0c*>(system);
    for (u32 i = 0; i < 24; ++i) {
        const CStrId& name = *g_unk_7101a1e620[i];
        UnkPathId path = fn_7100169c74(name);
        UnkResKey key(g_unk_716e453ef8.name, reinterpret_cast<const CFilePathStrId&>(path));
        UnkResRef resource = fn_71000ead80(g_unk_7101d472f8, &key, 1, nullptr);
        Append(self->shader_resources, resource);
        fn_7100169d28(unk_710b372d68, name, AsRenderRes(resource.ptr));
    }
    fn_71001545a0(fn_7100169be4(unk_710b372d68, g_unk_716e595bb8 + 0x48));
    fn_71001545b0(fn_7100169be4(unk_710b372d68, g_unk_716e595bb8));
    fn_71001545c0(fn_7100169be4(unk_710b372d68, g_unk_716e595bb8 + 0x48));
}
