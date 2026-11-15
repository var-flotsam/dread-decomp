// CEnemyMovement unit: switches the movement to state `id` unless it is current, through the state
// machine at +0x700, then tells the owner's component at +0x240 (virtual 0x510). Names and layouts are
// placeholders.
#include "../UnkSmRequest_A5_b73.h"
#include "types.h"

struct UnkEnemyMovementPeer_l0cc214 {
    void** vtable;
    void vf_0x510(u32 id) { reinterpret_cast<void (*)(UnkEnemyMovementPeer_l0cc214*, u32)>(vtable[0x510 / 8])(this, id); }
};

struct UnkEnemyMovementOwner_l0cc214 {
    u8 pad_0x0[0x240];
    UnkEnemyMovementPeer_l0cc214* peer;
};

struct UnkEnemyMovement_l0cc214 {
    u8 pad_0x0[0x30];
    UnkEnemyMovementOwner_l0cc214* owner;
    u8 pad_0x38[0x700 - 0x38];
    UnkSmMachine_A5_b73 machine;  // 0x700
};

void fn_71010cc214(UnkEnemyMovement_l0cc214* self, u32 id) {
    UnkSmState_A5_b73* current = self->machine.current;
    if ((current ? current->id : 0x7fffffffu) != id) {
        RequestState_A5_b73<6, 0x7fffffff>(&self->machine, id);
        if (UnkEnemyMovementPeer_l0cc214* peer = self->owner->peer)
            peer->vf_0x510(id);
    }
}
