// CActionSwitcherOnPullGrapplePointComponent's unit (tools/tu_map.py): fn_7100826634 passes its
// arguments to fn_710082cdb0, then for state 0 (or 1) sends the owner's (+0x30) sub-object at
// +0x220 the hash of the string id at +0x90 (or +0x88) of the object at +0x90 with mode 0 (as
// UnkSpitSend, src/game_s2/UnkSpitSend.h), unless that id is the pool's empty one. Names are
// placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "types.h"

struct UnkGrappleSwitch_b56;
void fn_710082cdb0(UnkGrappleSwitch_b56* self, void* arg, s32 state);

struct UnkGrappleOwner_b56 {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkGrappleIdHolder_b56 {
    u8 pad_0x0[0x88];
    base::global::UnkStringData* field_0x88;  // CStrId
    base::global::UnkStringData* field_0x90;  // CStrId
};

struct UnkGrappleSwitch_b56 {
    u8 pad_0x0[0x30];
    UnkGrappleOwner_b56* owner;
    u8 pad_0x38[0x58];
    UnkGrappleIdHolder_b56* field_0x90;
};

void fn_7100826634(UnkGrappleSwitch_b56* self, void* arg, s32 state) {
    fn_710082cdb0(self, arg, state);
    base::global::UnkStringData** slot;
    if (state == 1)
        slot = &self->field_0x90->field_0x88;
    else if (state == 0)
        slot = &self->field_0x90->field_0x90;
    else
        return;
    base::global::UnkStringData* id = *slot;
    if (id != &g_unk_7101d47108->empty)
        UnkSpitSend(self->owner->field_0x220, *reinterpret_cast<u64*>(reinterpret_cast<char*>(id) + 0x30),
                    0xf4061ff25a2163d6);
}
