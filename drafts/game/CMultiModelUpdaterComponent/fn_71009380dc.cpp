// CMultiModelUpdaterComponent's unit (tools/tu_map.py unit 1426): the destructor. Every model in the map
// (entries of 0x20 at +0x1e8, string-id keys at +0x1f0, count at +0x1f8) that has an object is taken out of
// the render manager's layer 0, the map is cleared, then the members go last to first (the current name
// at +0x210, the map) and the model updater base (fn_7101170f9c). Names and layouts are placeholders.
#include "base/UnkStrIdPathPair.h"
#include "base/global/CRntSmallDictionary.h"
#include "base/global/CStrId.h"
#include "game/UnkRenderManager.h"
#include "types.h"

using base::global::CStrId;

extern void* g_unk_7101a83e58[];  // CMultiModelUpdaterComponent's vtable

struct UnkModelUpdater_l170f9c;
void fn_7101170f9c(UnkModelUpdater_l170f9c* self);

void CMultiModelUpdaterComponent_DestroyModelMapValue(UnkStrIdPathResource* self);

// The object a model shows: its render item at +0x8.
struct UnkModelObject_l9380dc {
    void* field_0x0;
    u8 item[0x8];  // 0x8
};

struct UnkModelEntry_l9380dc {
    UnkStrIdPathResource resource;
    UnkModelObject_l9380dc* object;  // 0x18

    ~UnkModelEntry_l9380dc() { CMultiModelUpdaterComponent_DestroyModelMapValue(&resource); }
};
static_assert(sizeof(UnkModelEntry_l9380dc) == 0x20, "layout");

using UnkModelMap_l9380dc = base::global::CRntSmallDictionary<CStrId, UnkModelEntry_l9380dc>;

struct UnkMultiModelUpdater_l9380dc {
    void** vtable;
    u8 pad_0x8[0x1e0];
    UnkModelMap_l9380dc models;  // 0x1e8
    CStrId name;                 // 0x210
};
static_assert(__builtin_offsetof(UnkMultiModelUpdater_l9380dc, name) == 0x210, "layout");

void fn_71009380dc(UnkMultiModelUpdater_l9380dc* self) {
    self->vtable = &g_unk_7101a83e58[2];
    UnkModelMap_l9380dc& models = self->models;
    UnkModelEntry_l9380dc* end = models.End();
    for (UnkModelEntry_l9380dc* entry = models.values; entry != end; ++entry) {
        if (entry->object)
            fn_71002199d8(g_unk_7101d47300, reinterpret_cast<UnkItemBase_71011a91c4*>(entry->object->item), 0);
    }
    models.Clear();

    self->name.Reset();
    models.Clear();
    if (models.keys)
        models.allocator->vf_0x48(models.keys);
    if (models.values)
        models.allocator->vf_0x48(models.values);
    fn_7101170f9c(reinterpret_cast<UnkModelUpdater_l170f9c*>(self));
}
