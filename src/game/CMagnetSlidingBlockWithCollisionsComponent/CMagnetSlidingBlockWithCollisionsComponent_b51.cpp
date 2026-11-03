// CMagnetSlidingBlockWithCollisionsComponent (unit 1413): keeps a weak reference to the owner's
// collision object registered under the unit's name id, then runs the base step fn_710091eb20.
// Names and layouts are placeholders.
#include "game/UnkCollisionRecord.h"
#include "game_s2/UnkWeakHandle.h"

// A name id among the unit's statics.
extern base::global::CStrId g_unk_7172737758 __attribute__((visibility("hidden")));

struct UnkSlidingWithCollisionsOwner {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* collision;
};

struct UnkSlidingWithCollisions {
    u8 pad_0x0[0x30];
    UnkSlidingWithCollisionsOwner* owner;
    u8 pad_0x38[0x4f0 - 0x38];
    UnkWeakHandle* collision_object;  // 0x4f0
};

void fn_710091eb20(UnkSlidingWithCollisions* self);

void fn_710092222c(UnkSlidingWithCollisions* self) {
    UnkWeakTarget* target =
        reinterpret_cast<UnkWeakTarget*>(fn_71008640a4(self->owner->collision, &g_unk_7172737758));
    AssignRef(&self->collision_object, GetWeakHandle(target));
    fn_710091eb20(self);
}
