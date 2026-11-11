// The first entry the component's action sets (+0x48, count at +0x50) hold under a key and each
// id of the component's list at +0x4d0 in turn (fn_71002a6bdc of the entry's word), then under the
// key and the id -1; null when none does. CAnimationComponent's unit; the action set's lookup is
// src/game/CActionSet.cpp's. Names and layouts are placeholders.
#include "game/CActionSet.h"
#include "types.h"

struct UnkActionSetMap;
u64 fn_71002a6bdc(u32 value);

struct UnkAnimIdNode {
    u32 value;
    UnkAnimIdNode* next;
};

struct UnkAnimActionSetsOwner {
    u8 pad_0x0[0x48];
    UnkActionSetMap** sets;
    u32 count;
    u8 pad_0x54[0x47c];
    UnkAnimIdNode* ids;

    // Each set in turn until one holds the pair; the last result.
    __attribute__((always_inline)) void* Lookup(u64 key1, u64 key2) {
        void* r = nullptr;
        UnkActionSetMap** it = sets;
        UnkActionSetMap** end = it + count;
        if (it != end) {
            do {
                r = fn_7100b8c61c(*it, &key1, &key2);
                ++it;
            } while (it != end && !r);
        }
        return r;
    }
};
static_assert(__builtin_offsetof(UnkAnimActionSetsOwner, ids) == 0x4d0, "UnkAnimActionSetsOwner layout");

void* fn_7101176d34(UnkAnimActionSetsOwner* self, u64 key) {
    for (UnkAnimIdNode* n = self->ids; n; n = n->next)
        if (void* r = self->Lookup(key, fn_71002a6bdc(n->value)))
            return r;
    return self->Lookup(key, ~0ull);
}
