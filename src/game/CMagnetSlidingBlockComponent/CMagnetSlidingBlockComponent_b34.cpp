#include "types.h"

bool fn_710002fb70(void* timer);

struct CMagnetSlidingBlockComponent;
void fn_71009210d4(CMagnetSlidingBlockComponent* self, f32 speed);

struct CMagnetSlidingBlockComponent {
    u8 unk_0x0[0x358];
    u8 timer[0xbc];  // 0x358
    f32 speed;       // 0x414
};

// Sets the speed, zeroed while the timer at 0x358 says so, and applies it.
void fn_710092103c(CMagnetSlidingBlockComponent* self, f32 speed) {
    self->speed = speed;
    if (fn_710002fb70(self->timer))
        self->speed = 0.0f;
    fn_71009210d4(self, self->speed);
}
