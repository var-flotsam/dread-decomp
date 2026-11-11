// CStandaloneFXComponent's unit (tools/tu_map.py): the component's effect instances (SFXInstanceData, 0x40
// bytes each) in the engine array at +0x50, reached by index through the array's growing accessor (its growth
// out of line in fn_7100815758, new elements built by fn_7100815150). An instance holds its owner (whose
// +0x40 is the effect kind and +0x18 the node it hangs from), a file-path id, a position (+0x10), angles
// (+0x1c), a scale (+0x28) and the effect the model manager made for it (+0x38). Here: creating the effect
// for an instance (fn_7100811f28), the loops over all instances (create, release, preload, enable, set
// visibility, release the cached files), adding an instance (fn_7100813278), the reflected array's set and
// iterator move, and the scale setter. Names and layouts are placeholders.
#include "base/UnkCollectionIter.h"
#include "base/UnkVector.h"
#include "base/reflection/UnkReflValue.h"
#include "base/global/CFilePathStrId.h"
#include "base/UnkMatrix44.h"
#include "base/math/UnkZeroVector3.h"
#include "game/CActorComponent.h"
#include "game/CComponent.h"
#include "game/CSceneComponent.h"
#include "game/CStandaloneFXComponent.h"
#include "game/UnkModelManager.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkFxScale_ovb0103 {
    f32 x, y, z;
    UnkFxScale_ovb0103(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkFxScale_ovb0103(const UnkFxScale_ovb0103& o) : x(o.x), y(o.y), z(o.z) {}
    f32& At(s32 i) { return (&x)[i]; }
};

struct UnkFxHandle_ovb0103 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(const UnkFxScale_ovb0103* scale);
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40(bool value);
    virtual void vf_0x48(bool value);
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90(u32 value);
    virtual void vf_0x98(f32 value);
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual void vf_0xf0();
    virtual void vf_0xf8();
    virtual void vf_0x100();
    virtual void vf_0x108();
    virtual void vf_0x110();
    virtual void vf_0x118();
    virtual void vf_0x120();
    virtual void vf_0x128();
    virtual void vf_0x130();
    virtual void vf_0x138();
    virtual void vf_0x140();
    virtual void vf_0x148();
    virtual void vf_0x150();
    virtual void vf_0x158();
    virtual void vf_0x160();
    virtual void vf_0x168(void* node, const UnkMatrix44* m, u32 a, s32 b);
    virtual void vf_0x170();
};

struct UnkFxDesc_ovb0103 {
    u8 unk_0x0[0x18];
    char* node;  // 0x18
    u8 unk_0x20[0x20];
    u32 kind;  // 0x40
};

using base::global::CFilePathStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

// A file-path id (CFilePathStrId's pool) released in two steps, as base::UnkStrIdMemberOf.
struct UnkFxPath_ovb0103 {
    UnkStringData* data;

    UnkFxPath_ovb0103() {
        data = nullptr;
        if (!g_unk_7101d47200)
            base::global::UnkEnsureFilePathPool();
        data = &g_unk_7101d47200->empty;
    }
    UnkFxPath_ovb0103& operator=(UnkFxPath_ovb0103&& other) {
        UnkStringPool* pool = g_unk_7101d47200;
        if (pool) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47200;
                }
            }
            data = &pool->empty;
        }
        data = other.data;
        other.data = &g_unk_7101d47200->empty;
        return *this;
    }
    ~UnkFxPath_ovb0103() {
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
    const CFilePathStrId& Id() const { return reinterpret_cast<const CFilePathStrId&>(*this); }
};

struct UnkFxVec3_ovb0103 {
    f32 x, y, z;
    UnkFxVec3_ovb0103(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
};


struct UnkFxInstance_ovb0103 {
    UnkFxDesc_ovb0103* desc;  // 0x0
    UnkFxPath_ovb0103 path;  // 0x8
    u32 field_0x10;
    UnkFxVec3_ovb0103 offset;  // 0x14
    u32 field_0x20;
    u32 field_0x24;
    UnkFxScale_ovb0103 scale;  // 0x28
    UnkFxHandle_ovb0103* handle;  // 0x38

    explicit UnkFxInstance_ovb0103(UnkFxDesc_ovb0103* desc_)
        : desc(desc_), path(), field_0x10(0), offset(0.0f, 0.0f, 0.0f), field_0x20(0), field_0x24(0),
          scale(1.0f, 1.0f, 1.0f), handle(nullptr) {}
    ~UnkFxInstance_ovb0103();

    UnkFxInstance_ovb0103& operator=(UnkFxInstance_ovb0103&& o) {
        desc = o.desc;
        path = static_cast<UnkFxPath_ovb0103&&>(o.path);
        field_0x10 = o.field_0x10;
        offset.x = o.offset.x;
        offset.y = o.offset.y;
        offset.z = o.offset.z;
        field_0x20 = o.field_0x20;
        field_0x24 = o.field_0x24;
        scale.x = o.scale.x;
        scale.y = o.scale.y;
        scale.z = o.scale.z;
        handle = o.handle;
        return *this;
    }
};
static_assert(sizeof(UnkFxInstance_ovb0103) == 0x40, "UnkFxInstance_ovb0103 layout");

struct UnkFxVector_ovb0103;
void fn_7100815758(UnkFxVector_ovb0103* self, u32 n);
void fn_7100815150(UnkFxInstance_ovb0103* self);
struct UnkStandaloneFx;
struct UnkFxCache;
void fn_71011aa5a4(UnkFxCache* cache, const CFilePathStrId* path);
struct UnkMorphBallLauncher_b57;
void fn_71011a91c4(UnkModelManager* self, void* handle, u32 id);

static inline UnkModelManager* GetManager() {
    if (!g_unk_71726b0cd8) {
        UnkModelManager* m = static_cast<UnkModelManager*>(fn_71000839a0(0x1e8, 0x3f));
        fn_71011a870c(m);
        g_unk_71726b0cd8 = m;
    }
    return g_unk_71726b0cd8;
}

struct UnkFxVector_ovb0103 {
    UnkFxInstance_ovb0103* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool growable;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) UnkFxInstance_ovb0103& At(u32 index) {
        if (growable && count <= index) {
            u32 n = capacity;
            if (n <= index && step != 0)
                n = (index / step + 1) * step;
            fn_7100815758(this, n);
            fn_7100815758(this, index + 1);
            for (u32 i = count; i < index + 1; ++i)
                fn_7100815150(&data[i]);
            count = index + 1;
        }
        return data[index];
    }
};

struct UnkStandaloneFxComponent_ovb0103 {
    u8 unk_0x0[0x48];
    u32 field_0x48;
    u8 unk_0x4c[0x4];
    UnkFxVector_ovb0103 instances;  // 0x50
};

void fn_7100812b2c(UnkStandaloneFxComponent_ovb0103* self) {
    u32 n = self->instances.count;
    for (u32 i = 0; i < n; ++i)
        fn_7100811f28(reinterpret_cast<UnkStandaloneFx*>(&self->instances.At(i)));
}

void fn_710081305c(UnkStandaloneFxComponent_ovb0103* self, bool value) {
    u32 n = self->instances.count;
    for (u32 i = 0; i < n; ++i) {
        UnkFxInstance_ovb0103& e = self->instances.At(i);
        if (e.handle)
            e.handle->vf_0x40(value);
    }
}

void fn_7100813164(UnkStandaloneFxComponent_ovb0103* self) {
    u32 n = self->instances.count;
    for (u32 i = 0; i < n; ++i) {
        UnkFxInstance_ovb0103& e = self->instances.At(i);
        if (e.handle) {
            bool active = fn_71009f1988(e.desc);
            e.handle->vf_0x48(active);
        }
    }
}

void fn_7100812f1c(UnkStandaloneFxComponent_ovb0103* self) {
    fn_71010c422c(reinterpret_cast<UnkMorphBallLauncher_b57*>(self));
    u32 n = self->instances.count;
    for (u32 i = 0; i < n; ++i) {
        UnkFxCache* cache = reinterpret_cast<UnkFxCache*>(GetManager());
        fn_71011aa5a4(cache, &self->instances.At(i).path.Id());
    }
}

void fn_710081499c(UnkFxVector_ovb0103* self, const u32* index, UnkFxInstance_ovb0103* value) {
    self->At(*index) = static_cast<UnkFxInstance_ovb0103&&>(*value);
}

bool fn_710080fc90(const CFilePathStrId* path, u32* out);
void fn_71011aa0d4(UnkModelManager* self, u32 id, const base::global::CFilePathStrId* path, u32 kind, u32 flags);

void fn_7100812d5c(UnkStandaloneFxComponent_ovb0103* self) {
    u32 n = self->instances.count;
    for (u32 i = 0; i < n; ++i) {
        u32 id;
        CFilePathStrId path = self->instances.At(i).path.Id();
        if (path.data == &g_unk_7101d47200->empty) {
            if (fn_710080fc90(&path, &id))
                fn_71011aa0d4(GetManager(), id, &path, self->field_0x48, 0);
        }
    }
}

UnkFxInstance_ovb0103::~UnkFxInstance_ovb0103() {
    if (handle) {
        fn_71011a91c4(GetManager(), handle, desc->kind);
        handle = nullptr;
    }
}

// Moves the instance in at the index (out of line).
void fn_710081588c(UnkFxVector_ovb0103* self, UnkFxInstance_ovb0103* value, u32 index);

UnkFxInstance_ovb0103* fn_7100813278(UnkStandaloneFxComponent_ovb0103* self, u32 index) {
    UnkFxInstance_ovb0103 instance(reinterpret_cast<UnkFxDesc_ovb0103*>(self));
    fn_710081588c(&self->instances, &instance, index);
    return &self->instances.At(index);
}

using UnkFxIterValue_ovb0103 = base::UnkIterValue<UnkFxInstance_ovb0103>;

// UnkIterValue's move-assignment without the self check.
UnkFxIterValue_ovb0103* fn_710081446c(UnkFxIterValue_ovb0103* self, UnkFxIterValue_ovb0103* other) {
    if (other->state == 1) {
        self->Index() = other->value.index;
        self->Element() = static_cast<UnkFxInstance_ovb0103&&>(other->value.element);
    } else if (other->state == 2) {
        self->Index() = *other->ref.index;
        self->Element() = static_cast<UnkFxInstance_ovb0103&&>(*other->ref.element);
    }
    return self;
}

base::reflection::UnkReflValue fn_7100815534(void*, char* obj, base::reflection::UnkReflValue* const* args) {
    base::reflection::UnkReflValue* arg = args[0];
    UnkFxInstance_ovb0103* self = *reinterpret_cast<UnkFxInstance_ovb0103**>(obj + 8);
    const f32* v = base::reflection::refl_ArgValue<f32>(arg);
    for (s32 i = 0; i < 3; ++i) {
        if (!(*reinterpret_cast<f32*>(reinterpret_cast<u8*>(self) + 0x28 + i * 4) - 0.001f <= v[i]) || !(*reinterpret_cast<f32*>(reinterpret_cast<u8*>(self) + 0x28 + i * 4) + 0.001f >= v[i])) {
            self->scale.x = v[0];
            self->scale.y = v[1];
            self->scale.z = v[2];
            if (self->handle)
                self->handle->vf_0x20(&self->scale);
            break;
        }
    }
    return base::reflection::refl_EmptyValue();
}

UnkMatrix44 fn_71000791a8(const UnkVec3f& translation, const UnkVec3f& angle, const UnkVec3f& scale);
extern const base::math::CVector3D g_unk_71016477a8;
UnkFxHandle_ovb0103* fn_71011a9c2c(UnkModelManager* self, u32 id, const CFilePathStrId* path, u32 kind, u32 flags);

void fn_7100811f28(UnkStandaloneFx* fx) {
    auto* self = reinterpret_cast<UnkFxInstance_ovb0103*>(fx);
    if (self->path.data == &g_unk_7101d47200->empty || self->handle)
        return;
    u32 kind = self->desc->kind;
    {
        u32 id = 0;
        if (!fn_710080fc90(&self->path.Id(), &id)) {
            self->handle = nullptr;
            return;
        }
        self->handle = fn_71011a9c2c(GetManager(), id, &self->path.Id(), kind, 0);
    }
    if (!self->handle)
        return;
    u8* raw = reinterpret_cast<u8*>(self);
    UnkMatrix44 m = fn_71000791a8(*reinterpret_cast<const UnkVec3f*>(raw + 0x10), *reinterpret_cast<const UnkVec3f*>(raw + 0x1c),
                                  reinterpret_cast<const UnkVec3f&>(g_unk_71016477a8));
    self->handle->vf_0x170();
    self->handle->vf_0x168(self->desc->node ? self->desc->node + 0x10 : nullptr, &m, 0, -1);
    self->handle->vf_0x20(&self->scale);
    self->handle->vf_0x40(fn_710024fbc8(self->desc));
    bool active = fn_71009f1988(self->desc);
    self->handle->vf_0x48(active);
    self->handle->vf_0x90(0);
    self->handle->vf_0x98(1.0f);
}
