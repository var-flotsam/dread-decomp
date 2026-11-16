// CDemolitionBlockSonarTargetComponent unit (tools/tu_map.py unit 1520). fn_7100a0f7b4: the
// non-deleting destructor next to the deleting one of
// CDemolitionBlockSonarTargetComponent_Dtor_b52.cpp (vtable, the 0x20-byte polymorphic entries at
// +0x48, the counted reference at +0x40, then the base destructor fn_7100a1082c). fn_7100a0feac: the
// render pool's creator for a 0x160-byte model (fn_710014569c constructs it with its name), loaded
// from `path` by fn_7100a0fff0 unless the path is empty (a failed load deletes it), then added to the
// pool's chained hash set keyed by its scene node (+8) and returned as that node; `out` gets the model.
// Names and layouts are placeholders.
#include <new>

#include "base/global/CRntDictionary.h"
#include "base/global/CRntVector.h"
#include "base/global/CFilePathStrId.h"
#include "game/CDemolitionBlockSonarTargetComponent.h"
#include "game/CSonarTargetComponent.h"
#include "game/UnkModelManager.h"
#include "game/UnkRenderManager.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CFilePathStrId;

struct UnkSonarTargetEntry_ovb0054 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual ~UnkSonarTargetEntry_ovb0054();  // 0x30
    u8 pad_0x8[0x18];
};

struct UnkSonarTarget_ovb0054 {
    void** vtable;
    u8 pad_0x8[0x38];
    UnkRefObject* ref;                                     // 0x40
    base::global::CRntVector<UnkSonarTargetEntry_ovb0054> entries;  // 0x48
};

// By reference, like the deleting form.
void fn_7100a0f7b4(UnkSonarTarget_ovb0054& self) {
    self.vtable = reinterpret_cast<void**>(g_unk_7101a8b8f0 + 0x10);
    self.entries.Free();
    if (UnkRefObject* p = self.ref) {
        if (--p->ref_count == 0)
            p->vf_0x10();
    }
    fn_7100a1082c(reinterpret_cast<char*>(&self));
}

struct UnkDemolitionModelHolder;

// Constructs the model named `name`.
void fn_710014569c(void* self, base::global::CStrId* name);
bool fn_7100a0fff0(UnkDemolitionModelHolder* self, const CFilePathStrId& path);

struct UnkSonarModel_ovb0054 {
    virtual void vf_0x0();
    virtual void vf_0x8();
};

using UnkSonarSet_ovb0054 = base::global::CRntDictionary<u64, u64>;

// Returns the node (src/game/UnkModelManager.cpp declares it with a void return; the mangled name is the same).
UnkSceneNode* fn_7100a0feac(UnkRenderPool* pool, CFilePathStrId* path, UnkOverlayModel** out, base::global::CStrId* id) {
    UnkSonarSet_ovb0054* set = reinterpret_cast<UnkSonarSet_ovb0054*>(pool);
    auto* model = static_cast<UnkOverlayModel*>(fn_71000839a0(0x160, 0x38));
    fn_710014569c(model, id);
    UnkSceneNode* result = nullptr;
    if (model) {
        if (path->data != &g_unk_7101d47200->empty &&
            !fn_7100a0fff0(reinterpret_cast<UnkDemolitionModelHolder*>(model), *path)) {
            reinterpret_cast<UnkSonarModel_ovb0054*>(model)->vf_0x8();
            return nullptr;
        }
        set->Insert((u64)&model->node, (u64)model);
        result = &model->node;
    }
    if (out)
        *out = model;
    return result;
}
