#include "types.h"

struct UnkSlidingBlockMovement;
UnkSlidingBlockMovement* fn_710054a2cc(void* entity);  // the entity's typed component lookup
void fn_710092103c(UnkSlidingBlockMovement* movement, f32 speed);

struct UnkSlidingBlockParams {
    u8 unk_0x0[0x68];
    f32 speed;
};

struct CHangableGrappleMagnetSlidingBlockComponent {
    u8 unk_0x0[0x30];
    void* entity;
    u8 unk_0x38[0xa0 - 0x38];
    UnkSlidingBlockParams* params;
};

// Sets the block's movement speed: 1 when `full`, else the configured speed.
void fn_7100824528(CHangableGrappleMagnetSlidingBlockComponent* self, bool full) {
    UnkSlidingBlockMovement* movement = fn_710054a2cc(self->entity);
    if (!full)
        fn_710092103c(movement, self->params->speed);
    else
        fn_710092103c(movement, 1.0f);
}
