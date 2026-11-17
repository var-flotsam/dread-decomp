// CCharClassEventScenarioComponent's unit (1251): appends an empty list of string ids to a vector of
// them and returns it; growing moves each list over (its ids copied, the old list emptied with
// fn_71007e572c). Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CStrId;

// As src/game/CCharClassEventScenarioComponent/CCharClassEventScenarioComponent_Scenario_ovb0097.cpp
// defines it.
struct __attribute__((packed)) UnkScenarioIdList_ovb0097 {
    u32 count;
    CStrId ids[4];  // 0x4
};
static_assert(sizeof(UnkScenarioIdList_ovb0097) == 0x24, "UnkScenarioIdList_ovb0097 layout");

void fn_71007e572c(UnkScenarioIdList_ovb0097* self, u32 n);

UnkScenarioIdList_ovb0097* fn_71007e4df4(CRntVector<UnkScenarioIdList_ovb0097>* self) {
    u32 n = self->count + 1;
    if (self->capacity < n) {
        UnkScenarioIdList_ovb0097* lists = static_cast<UnkScenarioIdList_ovb0097*>(
            self->allocator->vf_0x40((u64)n * sizeof(UnkScenarioIdList_ovb0097), 8, self->flags));
        if (UnkScenarioIdList_ovb0097* old = self->data) {
            UnkScenarioIdList_ovb0097* dst = lists;
            for (u32 i = 0; i < self->count; ++i, ++dst, ++old) {
                for (u64 j = 0; j < old->count; ++j)
                    new (&dst->ids[j]) CStrId(old->ids[j]);
                dst->count = old->count;
                fn_71007e572c(old, 0);
            }
            self->allocator->vf_0x48(self->data);
        }
        self->data = lists;
        self->capacity = n;
    }
    for (u32 i = self->count; i < n; ++i)
        self->data[i].count = 0;
    self->count = n;
    UnkScenarioIdList_ovb0097* end = &self->data[n];
    return n ? &end[-1] : end;
}
