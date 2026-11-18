// CMultiModelUpdaterComponent's unit (tools/tu_map.py unit 1426): adds a model under a name unless the
// model map (entries of 0x20 at +0x1e8, keys at +0x1f0) already has one: loads the model resource at the
// path, builds the entry (fn_7100938ba0), creates the render model on layer 0 and appends the entry under
// the name. Always returns true. Names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "base/UnkPooledStrId.h"
#include "base/UnkResKey.h"
#include "base/UnkStrIdPathPair.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntSmallDictionary.h"
#include "base/global/CStrId.h"
#include "game/CMultiModelUpdaterComponent.h"
#include "game/CSpittailMagmaBallMovementComponent.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU127.h"
#include "types.h"

#include <new>

using base::global::CFilePathStrId;
using base::global::CStrId;

// As its other users declare it (CMultiModelUpdaterComponent_Models_ovb0119.cpp).
struct UnkModelValue_b0119 {
    u8 data[0x20];
};
using UnkModelMap_b0119 = base::global::CRntSmallDictionary<base::UnkPooledString, UnkModelValue_b0119>;
void fn_7100939678(UnkModelMap_b0119* self, u32 capacity);
void fn_7100938ba0(UnkModelValue_b0119* self, const CStrId* name, const CFilePathStrId* path,
                   UnkCountedResource* resource);

// A map entry, built in place by fn_7100938ba0: the resource reference and the render model made for it.
struct UnkModelEntry_l93852c {
    u8 resource[sizeof(UnkStrIdPathResource)];
    UnkModel* model;  // 0x18

    UnkStrIdPathResource* Resource() { return reinterpret_cast<UnkStrIdPathResource*>(resource); }
    UnkModelValue_b0119* Value() { return reinterpret_cast<UnkModelValue_b0119*>(this); }
};
static_assert(sizeof(UnkModelEntry_l93852c) == sizeof(UnkModelValue_b0119), "entry size");

struct UnkMultiModel_l93852c {
    u8 pad_0x0[0x1e8];
    UnkModelMap_b0119 models;  // 0x1e8
};

// The value under `name`, or End().
static inline UnkModelValue_b0119* Find_l93852c(UnkModelMap_b0119& models, const CStrId& name) {
    for (u64 i = 0; i < models.count; ++i)
        if (models.keys[i].data == name.data)
            return &models.values[i];
    return models.End();
}

// A render model's scene node (+0x8), or none.
static inline UnkSceneNode* Node_l93852c(UnkModel* model) {
    return model ? reinterpret_cast<UnkSceneNode*>(reinterpret_cast<char*>(model) + 8) : nullptr;
}

bool fn_710093852c(UnkMultiModel_l93852c* self, const CStrId* name, const CFilePathStrId* path) {
    UnkModelMap_b0119& models = self->models;
    if (Find_l93852c(models, *name) != models.End())
        return true;

    UnkResKey key(g_unk_716e453ff8.name, CFilePathStrId(*path));
    UnkModel* model;  // set once the entry is built
    UnkResRef resource = fn_71000ead80(g_unk_7101d472f8, &key, 1, nullptr);
    UnkModelEntry_l93852c entry;
    fn_7100938ba0(entry.Value(), name, path, resource.ptr);

    UnkRenderManager* manager = g_unk_7101d47300;
    model = nullptr;
    fn_71006cae2c(manager->field_0x218, *path, &model, &g_unk_7101d470e0);
    fn_7100219ae0(manager, Node_l93852c(model), 0);
    entry.model = model;
    fn_710014364c(entry.model, 0);
    fn_71001436a0(entry.model, 0);

    if (models.count == models.capacity)
        fn_7100939678(&models, models.count + models.step);
    // The key is a copy of the name.
    new (&models.keys[models.count]) CStrId(*name);
    fn_7100939804(&models.values[models.count], entry.Value());
    ++models.count;
    fn_7100938dec(entry.Resource());
    return true;
}
