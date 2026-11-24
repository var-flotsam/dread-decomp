// CModelUpdaterComponent unit (tools/tu_map.py): loads the updater's model from a file path. The model
// already shown is dropped (unless the owner keeps it), the path is remembered, the model is created on
// the render manager's layer (inside a resource context naming a second path when one is given), and
// the new model takes the owner's scale, visibility and a bone's height. Returns whether a model was
// made. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/UnkResKey.h"
#include "base/core/CAsset.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CActorComponent.h"
#include "game/CModelUpdaterComponent.h"
#include "game/CSceneComponent.h"
#include "game/CSpittailMagmaBallMovementComponent.h"
#include "game/UnkComponent.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU127.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkFilePathStrIdBase;
using base::global::UnkStringData;
using base::global::UnkStringPool;

// As src/base/core/CAsset/UnkIdRecordMapCopy_b45.cpp defines them: a resource context.
struct UnkRecord80Bytes_b45a1 {
    u64 words[0x80 / 8];
};

struct UnkIdRecordMapWithPath_b45a1 {
    UnkRecord80Bytes_b45a1* values;
    UnkStringData** keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::IAllocator* allocator;
    UnkStringData* path;  // 0x28, a file path id
};

void fn_7100016cfc(UnkIdRecordMapWithPath_b45a1* self, const UnkIdRecordMapWithPath_b45a1* other);

// The registry's stack of contexts (src/base/UnkUnit88/UnkQueue_b47.cpp).
struct UnkQueueOwner_b47;
void fn_71000ea3e8(UnkQueueOwner_b47* self);
void fn_71000ea374(UnkQueueOwner_b47* self, void* value);

// The resource registry's current context, at +0x118.
struct UnkResRegistryContext_l171a08 {
    u8 pad_0x0[0x118];
    UnkIdRecordMapWithPath_b45a1 context;
};

// The cache of loaded models, created on first use.
struct UnkResourceCache_ca4ec;
extern UnkResourceCache_ca4ec* g_unk_71727bed88;
void fn_7100eca4ec(UnkResourceCache_ca4ec* self);
UnkModel* fn_7100ecaa8c(UnkResourceCache_ca4ec* self, const CFilePathStrId* path);

struct UnkBoneOwner;
s32 fn_71001c14fc(UnkBoneOwner* self, const u64* id);
bool fn_71001c1c70(UnkBoneOwner* self, u32 index, UnkMatrix44* out);

__attribute__((visibility("hidden"))) extern const UnkVec3f g_unk_710164d0a4;  // the scale of one
void fn_71001435f0(UnkModel* model, const UnkVec3f* scale);

struct UnkLoadedModel_l171a08 {
    void** vtable;
    u8 item[0x130];  // 0x8, the render item
    UnkBoneOwner* bones;  // 0x138
};

struct UnkModelOwner_l171a08 {
    u8 pad_0x0[0x94];
    UnkVec3f scale;  // 0x94
    u8 pad_0xa0[0x300];
    u32 field_0x3a0;
};

struct UnkModelUpdater_l171a08 : UnkBaseComponent {
    u32 layer;  // 0x40
    CFilePathStrId loaded_path;  // 0x48
    UnkLoadedModel_l171a08* model;  // 0x50
    u8 pad_0x58[0x8];
    f32 height;  // 0x60
};

static inline UnkModel* AsModel(UnkLoadedModel_l171a08* model) { return reinterpret_cast<UnkModel*>(model); }

static inline UnkResourceCache_ca4ec* ModelCache() {
    if (!g_unk_71727bed88) {
        auto* cache = static_cast<UnkResourceCache_ca4ec*>(fn_71000839a0(0xd8, 0x3f));
        fn_7100eca4ec(cache);
        g_unk_71727bed88 = cache;
    }
    return g_unk_71727bed88;
}

// A string id's destructor on the pool `global` points to (decrement and test in two steps).
static inline void ReleaseId(UnkStringData*& data, UnkStringPool*& global) {
    if (UnkStringPool* pool = global) {
        if (data && data != &pool->empty) {
            --data->ref_count;
            if (data->ref_count == 0) {
                StringPool_Release(pool, data);
                pool = global;
            }
        }
        data = &pool->empty;
    }
}

// A file path id's copy assignment (as CFilePathStrId's).
static inline void AssignPath(UnkStringData*& data, UnkStringData* d) {
    if (data != d) {
        UnkStringPool* pool = g_unk_7101d47200;
        if (d != &pool->empty)
            ++d->ref_count;
        if (data != &pool->empty) {
            --data->ref_count;
            if (data->ref_count == 0)
                StringPool_Release(pool, data);
        }
        data = d;
    }
}

// A file path id made from a string, released in one step when it goes.
struct UnkPathOf_l171a08 : UnkFilePathStrIdBase {
    explicit UnkPathOf_l171a08(const CRntString& str) { fn_710001a270(this, str, true); }
    ~UnkPathOf_l171a08() {
        if (UnkStringPool* pool = g_unk_7101d47200) {
            if (data && data != &pool->empty && --data->ref_count == 0) {
                StringPool_Release(pool, data);
                pool = g_unk_7101d47200;
            }
            data = &pool->empty;
        }
    }
};

// The context's destructor: its path, its keys, its records, then the storage.
static inline void DestroyContext(UnkIdRecordMapWithPath_b45a1& c) {
    ReleaseId(c.path, g_unk_7101d47200);
    if (c.keys) {
        for (u64 i = 0; i < c.count; ++i)
            ReleaseId(c.keys[i], g_unk_7101d47108);
    }
    if (c.values) {
        for (u64 i = 0; i < c.count; ++i)
            fn_7100017ce8(reinterpret_cast<UnkRecord80*>(&c.values[i]));
    }
    c.count = 0;
    if (c.keys)
        c.allocator->vf_0x48(c.keys);
    if (c.values)
        c.allocator->vf_0x48(c.values);
}

// Takes the model off the render manager's layer and forgets its path.
static inline void ReleaseModel(UnkModelUpdater_l171a08* self) {
    if (self->model) {
        fn_71002199d8(g_unk_7101d47300, reinterpret_cast<UnkItemBase_71011a91c4*>(self->model->item),
                      self->layer);
        ReleaseId(self->loaded_path.data, g_unk_7101d47200);
        self->model = nullptr;
    }
}

// Remembers the path of the model to load and returns the model the cache holds for it.
static inline UnkLoadedModel_l171a08* SetPath(UnkModelUpdater_l171a08* self, const CRntString& path) {
    UnkFilePathStrIdBase id;
    fn_710001a270(&id, path, true);
    self->loaded_path = static_cast<CFilePathStrId&&>(id);
    return reinterpret_cast<UnkLoadedModel_l171a08*>(fn_7100ecaa8c(ModelCache(), &self->loaded_path));
}

// Creates the model at the updater's path on the render manager's layer.
static inline void CreateModel(UnkModelUpdater_l171a08* self, UnkLoadedModel_l171a08*& model, UnkRenderManager* rm,
                               UnkModel*& created, u32 layer) {
    fn_71006cae2c(rm->field_0x218, self->loaded_path, &created, &g_unk_7101d470e0);
    auto* loaded = reinterpret_cast<UnkLoadedModel_l171a08*>(created);
    fn_7100219ae0(rm, loaded ? reinterpret_cast<UnkSceneNode*>(loaded->item) : nullptr, layer);
    model = reinterpret_cast<UnkLoadedModel_l171a08*>(created);
}

inline const CStrId& HeightBoneId_l171a08() {
    static UnkGameManagerIdTable s_table asm("g_unk_7101d552d0");
    return s_table.field_0xd18;
}

static inline s32 FindBone(UnkBoneOwner* bones, const CStrId& id) {
    u64 key = id.data->Key();
    return fn_71001c14fc(bones, &key);
}

static inline void SetIdentity(UnkMatrix44* m) {
    for (int i = 0; i < 16; ++i)
        m->m[i] = 0.0f;
    for (int i = 3; i >= 0; --i)
        m->m[i * 5] = 1.0f;
}

bool fn_7101171a08(UnkModelUpdater_ovb0032* updater, const void* path, const CRntString* extra) {
    auto* self = reinterpret_cast<UnkModelUpdater_l171a08*>(updater);
    UnkLoadedModel_l171a08*& model = self->model;
    auto* owner = static_cast<UnkModelOwner_l171a08*>(self->field_0x30);
    // An owner that keeps its model keeps the one it has.
    if (owner->field_0x3a0 && model)
        return false;
    ReleaseModel(self);
    u32 layer = fn_710024fbc8(self) ? self->layer : (u32)-1;

    UnkLoadedModel_l171a08* cached = SetPath(self, *static_cast<const CRntString*>(path));

    if (extra->data && extra->data[0]) {
        UnkIdRecordMapWithPath_b45a1 context;
        fn_7100016cfc(&context, &reinterpret_cast<UnkResRegistryContext_l171a08*>(g_unk_7101d472f8)->context);
        {
            UnkPathOf_l171a08 extra_path(*extra);
            AssignPath(context.path, extra_path.data);
        }
        UnkModel* created = nullptr;
        UnkRenderManager* rm = g_unk_7101d47300;
        fn_71000ea374(reinterpret_cast<UnkQueueOwner_b47*>(g_unk_7101d472f8), &context);
        CreateModel(self, model, rm, created, layer);
        fn_71000ea3e8(reinterpret_cast<UnkQueueOwner_b47*>(g_unk_7101d472f8));
        DestroyContext(context);
    } else {
        UnkRenderManager* rm = g_unk_7101d47300;
        UnkModel* created = nullptr;
        CreateModel(self, model, rm, created, layer);
    }
    if (cached)
        fn_71002199d8(g_unk_7101d47300, reinterpret_cast<UnkItemBase_71011a91c4*>(cached->item), 0);

    if (model) {
        auto* o = static_cast<UnkModelOwner_l171a08*>(self->field_0x30);
        UnkVec3f scale = o ? o->scale : g_unk_710164d0a4;
        fn_71001435f0(AsModel(model), &scale);
        bool visible = fn_710024fbc8(self);
        fn_710014364c(AsModel(self->model), visible);
        bool flag = fn_71009f1988(self);
        fn_71001436a0(AsModel(self->model), flag);
        UnkLoadedModel_l171a08* m = self->model;
        const CStrId& bone_id = HeightBoneId_l171a08();
        s32 index = FindBone(m->bones, bone_id);
        if (index != -1) {
            UnkMatrix44 bone;
            SetIdentity(&bone);
            fn_71001c1c70(self->model->bones, index, &bone);
            self->height = -bone.m[7];
        }
    }
    return model != nullptr;
}
