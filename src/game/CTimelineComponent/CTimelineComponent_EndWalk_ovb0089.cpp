// CTimelineComponent unit (tools/tu_map.py): ends a walk over the component's three listener lists (a
// copy of fn_710113831c, src/game/CAttackComponent/CAttackComponent_Targets_ovb0050.cpp): once the walk
// counter is back to zero, the listeners removed meanwhile leave the active list and those added join
// it. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

struct UnkTimelineListenerLists_ovb0089 {
    base::UnkVector<void*> field_0x0;
    base::UnkVector<void*> field_0x28;
    base::UnkVector<void*> field_0x50;
    union {
        s32 lock;  // 0x78
    };
};

void** fn_7100a2e664(base::UnkVector<void*>* self, u32 index);

struct UnkTimelineListenerLock_ovb0089 {
    UnkTimelineListenerLists_ovb0089* lists;
};

void fn_7100a2e4a0(UnkTimelineListenerLock_ovb0089* self) {
    --self->lists->lock;
    UnkTimelineListenerLists_ovb0089* lists = self->lists;
    if (lists->lock > 0)
        return;
    for (u32 i = 0; i < lists->field_0x28.count; ++i)
        lists->field_0x50.Remove(*fn_7100a2e664(&lists->field_0x28, i));
    __builtin_memset(&lists->field_0x28.count, 0, sizeof(u32));
    lists = self->lists;
    for (u32 i = 0; i < lists->field_0x0.count; ++i)
        lists->field_0x50.PushBack(*fn_7100a2e664(&lists->field_0x0, i));
    lists->field_0x0.count = 0;
}
