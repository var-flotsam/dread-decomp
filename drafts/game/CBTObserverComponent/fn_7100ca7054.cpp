// CBTObserverComponent unit: removes the observer a numeric id names from the manager's map of parallel
// arrays at +0x90 (the last one takes its place), drops it from every group's list, then releases it.
// Returns whether there was one. Names and layouts are placeholders.
#include "base/UnkParallelFind.h"
#include "types.h"

struct UnkObserverToRemove {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
};

struct UnkObserverGroup_7054 {
    u8 pad_0x0[0x8];
    UnkObserverToRemove** data;  // 0x8
    u32 count;                   // 0x10
    u8 pad_0x14[0x1c];
    UnkObserverGroup_7054* next;  // 0x30
};

struct UnkObserverRemoveManager {
    u8 pad_0x0[0x48];
    UnkObserverGroup_7054* groups;  // 0x48
    u8 pad_0x50[0x40];
    UnkObserverToRemove** values;  // 0x90
    u32* keys;                     // 0x98
    u32 count;                     // 0xa0
};

struct UnkBreakableTileGroupData;
void fn_7100ca07a8(UnkBreakableTileGroupData* self);

bool fn_7100ca7054(UnkObserverRemoveManager* self, u32 id) {
    UnkObserverToRemove** slot = UnkParallelFindSlotByValue(self, id);
    if (slot == &self->values[self->count])
        return false;

    UnkObserverToRemove* observer = *slot;
    u64 index = slot - self->values;
    if (self->count - 1 != (u32)index) {
        self->values[(u32)index] = self->values[self->count - 1];
        self->keys[(u32)index] = self->keys[self->count - 1];
    }
    self->count = self->count - 1;

    for (UnkObserverGroup_7054* group = self->groups; group; group = group->next) {
        UnkObserverToRemove** it = group->data;
        for (u32 left = group->count; left; --left, ++it) {
            if (*it == observer) {
                __builtin_memcpy(it, it + 1, (u64)(left - 1) * sizeof(*it));
                --group->count;
                break;
            }
        }
    }
    fn_7100ca07a8(reinterpret_cast<UnkBreakableTileGroupData*>(observer));
    observer->vf_0x10();
    return true;
}
