// CMultiModelUpdaterComponent's unit (tools/tu_map.py unit 1426): shows the model of a model map
// entry. With a model already shown, its resource is swapped for the entry's (nothing when the
// path is the same); otherwise a model is created on the component's layer, scaled and placed by
// one of its bones. Names and layouts are placeholders.
// Needs UnkGameManagerIdTable::field_0xd18 (a CStrId at 0xd18; tested with build/lean/scratch/UnkGameManagerIdTable.h).
#include "game/UnkGameManagerIdTable.h"
#include "base/UnkCloneCalls.h"
#include "base/UnkCountedResource.h"
#include "base/UnkMatrix44.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CActorComponent.h"
#include "game/CMultiModelUpdaterComponent.h"
#include "game/CSceneComponent.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU127.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkBoneOwner;
struct UnkPoolMap_b0252;
struct UnkResourceHolder_b0252;

struct UnkClone_fn_71001c3314;
extern template void base::refl_SetBool<UnkClone_fn_71001c3314, 0x360>(char*, bool);

void fn_71001434b8(UnkModel* model, UnkCountedResource* resource);
void fn_71001435f0(UnkModel* model, const UnkVec3f* scale);
void* fn_7100141f1c(UnkPoolMap_b0252* map, UnkCountedResource* resource, UnkResourceHolder_b0252** out,
                   UnkTaggedSubArg* name);
s32 fn_71001c14fc(UnkBoneOwner* self, const u64& key);
bool fn_71001c1c70(UnkBoneOwner* self, u32 index, UnkMatrix44* out);

// The scale a model gets when the component has no settings object.
__attribute__((visibility("hidden"))) extern const f32 g_unk_710164b290;

struct UnkModelScene_l938fcc {
    u8 pad_0x0[0x29b0];
    u8 dirty;  // 0x29b0
};

struct UnkModelSkeleton_l938fcc {
    u8 pad_0x0[0x78];
    UnkModelScene_l938fcc* scene;  // 0x78
};

// A model as the render pool creates it: its scene node at +8, its resource at +0x10.
struct UnkModelInstance_l938fcc {
    void* vtable;
    u8 node[0x8];                       // 0x8, UnkSceneNode
    UnkReflObject* resource;            // 0x10
    u8 pad_0x18[0x138 - 0x18];
    UnkModelSkeleton_l938fcc* skeleton;  // 0x138
};

// A resource with its file path at +0x10.
struct UnkPathRes_l938fcc : UnkReflObject {
    u8 pad_0x8[0x8];
    CFilePathStrId path;  // 0x10
};

struct UnkModelSettings_l938fcc {
    u8 pad_0x0[0x94];
    f32 scale;  // 0x94
};

struct UnkModelEntryView_l938fcc {
    CStrId name;
    CFilePathStrId path;
    UnkCountedResource* resource;  // 0x10
    u64 field_0x18;
};

struct UnkMultiModelView_l938fcc {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkModelSettings_l938fcc* settings;  // 0x30
    u8 pad_0x38[0x8];
    s32 layer;  // 0x40
    u8 pad_0x44[0xc];
    UnkModelInstance_l938fcc* model;  // 0x50
    u8 pad_0x58[0x8];
    f32 height;  // 0x60
    u8 pad_0x64[0x218 - 0x64];
    bool flag;  // 0x218

    void vf_0x190(UnkModelEntry_A5_b67* entry) {
        reinterpret_cast<void (*)(UnkMultiModelView_l938fcc*, UnkModelEntry_A5_b67*)>(vtable[0x190 / 8])(this,
                                                                                                     entry);
    }
};
static_assert(__builtin_offsetof(UnkMultiModelView_l938fcc, flag) == 0x218, "component layout");

static inline UnkPathRes_l938fcc* AsPathRes(UnkReflObject* res) {
    return fn_710009d50c(res->vf_0x0(), fn_71000e8a5c()) ? static_cast<UnkPathRes_l938fcc*>(res) : nullptr;
}

static inline UnkCountedResource* AsRenderRes(UnkCountedResource* res) {
    if (!res)
        return nullptr;
    UnkReflObject* object = reinterpret_cast<UnkReflObject*>(res);
    return fn_710009d50c(object->vf_0x0(), fn_71001b122c()) ? res : nullptr;
}

static inline UnkModel* AsModel(UnkModelInstance_l938fcc* model) {
    return reinterpret_cast<UnkModel*>(model);
}

static inline UnkBoneOwner* Bones(UnkModelInstance_l938fcc* model) {
    return reinterpret_cast<UnkBoneOwner*>(model->skeleton);
}

static inline s32 FindBone(UnkModelInstance_l938fcc* model, const u64& key) {
    return fn_71001c14fc(Bones(model), key);
}

static inline void SetFlag(UnkModelInstance_l938fcc* model, bool flag) {
    base::refl_SetBool<UnkClone_fn_71001c3314, 0x360>(reinterpret_cast<char*>(model->skeleton), flag);
}

static inline void SetIdentity(UnkMatrix44* m) {
    for (int i = 0; i < 16; ++i)
        m->m[i] = 0.0f;
    for (int i = 3; i >= 0; --i)
        m->m[i * 5] = 1.0f;
}

bool fn_7100938fcc(UnkMultiModel_A5_b67* component, UnkModelEntry_A5_b67* modelEntry) {
    UnkMultiModelView_l938fcc* self = reinterpret_cast<UnkMultiModelView_l938fcc*>(component);
    UnkModelEntryView_l938fcc* entry = reinterpret_cast<UnkModelEntryView_l938fcc*>(modelEntry);
    if (!entry->resource)
        return false;

    UnkModelInstance_l938fcc* model = self->model;
    if (model) {
        if (AsPathRes(model->resource)->path.data == entry->path.data)
            return false;
        UnkCountedResource* resource = entry->resource;
        if (!resource)
            return true;
        model = self->model;
        if (model) {
            fn_71001434b8(AsModel(model), AsRenderRes(resource));
            self->model->skeleton->scene->dirty = 1;
        }
    }
    if (!model) {
        s32 layer = fn_710024fbc8(self) ? self->layer : -1;
        UnkRenderManager* manager = g_unk_7101d47300;
        {
            UnkCountedResource* resource = AsRenderRes(entry->resource);
            UnkModelInstance_l938fcc* created = nullptr;
            fn_7100141f1c(reinterpret_cast<UnkPoolMap_b0252*>(manager->field_0x218), resource,
                          reinterpret_cast<UnkResourceHolder_b0252**>(&created),
                          reinterpret_cast<UnkTaggedSubArg*>(&g_unk_7101d470e0));
            fn_7100219ae0(manager, created ? reinterpret_cast<UnkSceneNode*>(created->node) : nullptr, layer);
            self->model = created;
            created->skeleton->scene->dirty = 1;
        }
        {
            f32 s = *(self->settings ? &self->settings->scale : &g_unk_710164b290);
            UnkVec3f scale(s, s, s);
            fn_71001435f0(AsModel(self->model), &scale);
        }
        fn_710014364c(AsModel(self->model), fn_710024fbc8(self));
        fn_71001436a0(AsModel(self->model), fn_71009f1988(self));
        s32 bone = FindBone(self->model, GameManagerIdTable().field_0xd18.data->Key());
        if (bone != -1) {
            UnkMatrix44 matrix;
            SetIdentity(&matrix);
            fn_71001c1c70(Bones(self->model), bone, &matrix);
            self->height = -matrix.m[7];
        }
    }
    if (self->model)
        SetFlag(self->model, self->flag);
    self->vf_0x190(modelEntry);
    return true;
}
