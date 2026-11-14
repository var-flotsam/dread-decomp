// CMultiLockOnPointComponent (unit 1340): a hit with flag 0x4 resolves the point's lazily resolved
// handle (the shape of CTutoExitLogicAction_b47.cpp) and, when it names a block, passes it the
// owner and the hit's flag 0x200, then updates the owner's two timeline keys. Names and layouts
// are placeholders.
#include "base/UnkRefHandle.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU437.h"
#include "game/UnkTimelineComponent.h"

struct UnkCountedLockOn_b51 : base::UnkRefCounted {
    void* field_0x10;
};

using UnkRef = base::UnkRefPtrMember;

struct UnkLockOnBlock;
UnkLockOnBlock* fn_71008846dc(void* object);
void fn_710088341c(UnkLockOnBlock* block, UnkActor* owner, bool flag);

extern u64 g_unk_710164ae78;
extern u64 g_unk_710164ae80;

struct UnkLockOnHit {
    u8 pad_0x0[0xb4];
    u32 flags;
};

struct UnkLockOnPoint {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x5];
    bool field_0x3d;
    u8 pad_0x3e[0xa];
    u8 source[0x28];  // 0x48
    u32 state;        // 0x70
    u32 pad_0x74;
    base::UnkRefHandle handle;  // 0x78
};

bool fn_71008845fc(UnkLockOnPoint* self, void* sender, UnkLockOnHit* hit) {
    if (!(hit->flags & 4) || self->field_0x3d)
        return false;
    if (self->state == 1) {
        {
            UnkRef fresh = fn_7100353be8(self->source);
            fn_710031d208(&self->handle, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
        }
        self->state = 2;
    }
    auto* c = static_cast<UnkCountedLockOn_b51*>(self->handle.ptr);
    if (!c || !c->field_0x10)
        return false;
    // the original reloads the object after the test
    if (UnkLockOnBlock* block = fn_71008846dc(*reinterpret_cast<void* volatile*>(&c->field_0x10))) {
        fn_710088341c(block, self->owner, (hit->flags >> 9) & 1);
        fn_7100d7c94c(self->owner, &g_unk_710164ae78);
        fn_7100d7c828(self->owner, &g_unk_710164ae80, false);
    }
    return false;
}
