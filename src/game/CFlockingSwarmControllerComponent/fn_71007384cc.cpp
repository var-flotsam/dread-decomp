// CFlockingSwarmControllerComponent unit: removes the swarm's tile blockers from the owner's collision
// container (fn_7100863e04) and empties the list, then clears a counter in each swarm entry.
// Names and layouts are placeholders.
#include "game/CCollisionComponent.h"
#include "types.h"

struct UnkSwarmOwner_l7384cc {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* field_0x2a0;
};

// A fixed-capacity list whose elements live in raw storage after the count.
struct UnkTileBlockers_l7384cc {
    u32 count;
    u8 storage[16 * sizeof(UnkCollisionObject*)];

    UnkCollisionObject** Data() { return reinterpret_cast<UnkCollisionObject**>(storage); }
    UnkCollisionObject** begin() { return Data(); }
    UnkCollisionObject** end() { return Data() + count; }
};

struct UnkSwarmEntry_l7384cc {
    u8 pad_0x0[8];
    u32 field_0x8;
    u8 pad_0xc[0x28 - 0xc];
};

struct UnkSwarmEntries_l7384cc {
    UnkSwarmEntry_l7384cc* data;
    u32 count;

    UnkSwarmEntry_l7384cc* begin() { return data; }
    UnkSwarmEntry_l7384cc* end() { return &data[count]; }
};

struct UnkFlockingSwarm_l7384cc {
    u8 pad_0x0[0x30];
    UnkSwarmOwner_l7384cc* field_0x30;
    u8 pad_0x38[0x140 - 0x38];
    UnkSwarmEntries_l7384cc entries;
    u8 pad_0x150[0x3e0 - 0x150];
    UnkTileBlockers_l7384cc blockers;
};
static_assert(__builtin_offsetof(UnkFlockingSwarm_l7384cc, blockers) == 0x3e0, "layout");

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_71007384cc(UnkFlockingSwarm_l7384cc* self) {
    if (self->field_0x30->field_0x2a0) {
        for (UnkCollisionObject* blocker : self->blockers)
            fn_7100863e04(self->field_0x30->field_0x2a0, blocker);
        self->blockers.count = 0;
    }
    UnkSwarmEntries_l7384cc& entries = self->entries;
    for (UnkSwarmEntry_l7384cc* it = entries.begin(); it != entries.end(); ++it)
        it->field_0x8 = 0;
}
