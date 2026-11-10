// CCharClassTrainUsableComponent's unit (tools/tu_map.py): a constructor (the base constructor
// fn_7100a7b494, the vtable, then twelve copies of the empty string id g_unk_7101d470e0 at +0x1b8).
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

struct UnkShipRechargeDef;
void fn_7100a7b494(UnkShipRechargeDef* self);
extern void* g_unk_7101a93690[];  // the class's vtable

using UnkTrainIdData_ovb0127 = base::global::UnkStringData;

struct UnkTrainUsableDef_ovb0127 {
    void** vtable;
    u8 pad_0x8[0x1b0];
    UnkTrainIdData_ovb0127* ids[12];  // 0x1b8: CStrId
};

void fn_7100a983c8(UnkTrainUsableDef_ovb0127* self) {
    fn_7100a7b494(reinterpret_cast<UnkShipRechargeDef*>(self));
    self->vtable = g_unk_7101a93690 + 2;
    // Copies of the empty id: the id and the pool's empty one are read once.
    UnkTrainIdData_ovb0127* id = g_unk_7101d470e0.data;
    self->ids[0] = id;
    bool counted = &g_unk_7101d47108->empty != id;
    if (counted)
        ++id->ref_count;
    for (int i = 1; i < 12; ++i) {
        self->ids[i] = id;
        if (counted)
            ++id->ref_count;
    }
}
