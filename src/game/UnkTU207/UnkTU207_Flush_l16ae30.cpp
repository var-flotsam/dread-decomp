// Unit 207: walks the controller's (target, name id) entries (each one reached through the growing
// accessor), resetting flag 0x10 on every material of the targets the registry g_unk_716e34c2e0 still
// holds when the model asks for it, then releases the ids and empties the list. Names and layouts are
// placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkController_u207.h"
#include "game/UnkModel_u207.h"
#include "game/UnkTU204.h"
#include "game/UnkTU207.h"
#include "game/UnkTU225.h"
#include "game/msapi/material/SAlpha.h"
#include "types.h"

using base::global::CStrId;

struct UnkKeySetOwner;

struct UnkEntry_u207 {
    void* target;
    CStrId id;
    UnkEntry_u207(const UnkEntry_u207& other) : target(other.target), id(other.id) {}
};

// A target with materials: their count at +0x2c and the array at +0x48.
struct UnkMaterialTarget_l16ae30 {
    u8 pad_0x0[0x2c];
    u32 material_count;  // 0x2c
    u8 pad_0x30[0x18];
    UnkMaterial** materials;  // 0x48
};

// The entry at `index`, growing the list to hold it (new entries get no target and the empty id) when
// it may grow.
static inline UnkEntry_u207& GrowAt(base::global::CRntVector<UnkEntry_u207>& v, u32 index) {
    if (v.field_0x14 && v.count <= index) {
        u32 cap = v.capacity;
        if (cap <= index && v.step != 0)
            cap = v.GrownCapacity(index);
        fn_710016b21c(&v, cap);
        fn_710016b21c(&v, index + 1);
        for (u32 i = v.count; i < index + 1; ++i) {
            UnkEntry_u207* entry = &v.data[i];
            entry->target = nullptr;
            new (&entry->id) CStrId(g_unk_710162e2ed, true);
        }
        v.count = index + 1;
    }
    return v.data[index];
}

void fn_710016ae30(void* self_) {
    UnkController_u207* self = static_cast<UnkController_u207*>(self_);
    UnkModel_u207* model = self->model;
    bool reset = model->field_0xa9 && model->field_0xb0 != 0;
    u32 n = self->entries.count;
    for (u32 i = 0; i < n; ++i) {
        auto* target = static_cast<UnkMaterialTarget_l16ae30*>(GrowAt(self->entries, i).target);
        if (reset && fn_710019e4fc(static_cast<UnkKeySetOwner*>(g_unk_716e34c2e0), reinterpret_cast<u64>(target))) {
            u32 count = target->material_count;
            for (u64 j = 0; j < count; ++j)
                fn_71001a6ec8(target->materials[j], 0x10);
        }
    }
    // Releases every id, then empties the list.
    for (u32 i = 0; i < self->entries.count; ++i)
        self->entries.data[i].id.Reset();
    self->entries.count = 0;
}
