// From units 296 and 297 of tools/tu_map.py (0x710020f450-0x710020ffbc): copies the active list to a
// snapshot, then updates every item of the snapshot with the frame time (fifth virtual). Unit 296's
// table also re-sorts its second list when it was marked dirty. Names and layouts are placeholders.
#include <string.h>
#include "types.h"

struct UnkUnit296Updatable {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void Update(float dt);
};

template <typename Table>
__attribute__((always_inline)) static inline void UpdateSnapshot(Table* self, float dt) {
    memcpy(self->snapshot, self->active, self->active_count * sizeof(UnkUnit296Updatable*));
    self->snapshot_count = self->active_count;
    for (u64 i = 0; i < self->snapshot_count; ++i)
        self->snapshot[i]->Update(dt);
}

struct UnkUnit296UpdateTable {
    u8 pad_0x0[0x4008];
    void* sorted[0x400];  // 0x4008
    UnkUnit296Updatable* active[0x400];  // 0x6008
    UnkUnit296Updatable* snapshot[0x400];  // 0x8008
    u8 pad_0xa008[0x8];
    u32 sorted_count;  // 0xa010
    u32 active_count;  // 0xa014
    u32 snapshot_count;  // 0xa018
    u8 pad_0xa01c[0x10];
    bool dirty;  // 0xa02c
};

void fn_710020f450(s32 lo, s32 hi, void** items);

void fn_710020f6c8(UnkUnit296UpdateTable* self, float dt) {
    UpdateSnapshot(self, dt);
    if (self->dirty) {
        fn_710020f450(0, self->sorted_count - 1, self->sorted);
        self->dirty = false;
    }
}

struct UnkUnit297UpdateTable {
    u8 pad_0x0[0x28008];
    UnkUnit296Updatable* active[0x2800];  // 0x28008
    UnkUnit296Updatable* snapshot[0x2800];  // 0x3c008
    u8 pad_0x50008[0x8];
    u32 active_count;  // 0x50010
    u32 snapshot_count;  // 0x50014
};

void fn_710020fc80(UnkUnit297UpdateTable* self, float dt) {
    UpdateSnapshot(self, dt);
}
