#include "../../game_s7/UnkIter.h"
#include "game/UnkTU2510.h"
#include "types.h"
#include <algorithm>

// A tracked object's record: the object it stands for is `key`.
struct UnkTrackedEntry { void* vt; u32 refCount; u32 pad; void* key; };
struct UnkSet {
    void* vt;
    UnkTrackedEntry** slots;
    u32 count;
    UnkIter<UnkTrackedEntry*> begin() { return UnkIter<UnkTrackedEntry*>{slots}; }
    UnkIter<UnkTrackedEntry*> end() { return UnkIter<UnkTrackedEntry*>{slots + count}; }
};
void fn_7100d5a9e4(UnkSet* self, UnkTrackedEntry** it);

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_7100d5abfc(void* owner, UnkStruct_7100861148_Collider* collider) {
    UnkSet* self = static_cast<UnkSet*>(owner);
    void* key = collider;
    auto it = std::find_if(self->begin(), self->end(), [key](UnkTrackedEntry* e) { return e && e->key == key; });
    fn_7100d5a9e4(self, it.p);
}
