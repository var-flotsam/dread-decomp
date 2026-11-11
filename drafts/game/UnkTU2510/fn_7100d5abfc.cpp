#include "../../../src/game_s7/UnkIter.h"
#include "types.h"
#include <algorithm>

struct UnkKeyed { void* vt; u32 refCount; u32 pad; void* key; };
struct UnkTracker_7100863a8c;
struct UnkCollisionObject;
struct UnkSet {
    void* vt;
    UnkKeyed** slots;
    u32 count;
    UnkIter<UnkKeyed*> begin() { return UnkIter<UnkKeyed*>{slots}; }
    UnkIter<UnkKeyed*> end() { return UnkIter<UnkKeyed*>{slots + count}; }
};
void fn_7100d5a9e4(UnkSet* self, UnkKeyed** it);

void fn_7100d5abfc(UnkTracker_7100863a8c* tracker, UnkCollisionObject* obj) {
    UnkSet* self = reinterpret_cast<UnkSet*>(tracker);
    void* key = obj;
    auto it = std::find_if(self->begin(), self->end(), [key](UnkKeyed* e) { return e && e->key == key; });
    fn_7100d5a9e4(self, it.p);
}
