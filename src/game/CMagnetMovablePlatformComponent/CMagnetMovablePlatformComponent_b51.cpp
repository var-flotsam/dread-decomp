// CMagnetMovablePlatformComponent (unit 1407).
#include "game/UnkTU811.h"
#include "game_s2/UnkWeakHandle.h"

struct UnkSlidingBlockMovement;

struct UnkMagnetMovablePlatform {
    u8 pad_0x0[0x250];
    UnkWeakHandle* movement;  // 0x250
};

// Keeps a weak reference to the sliding-block movement fn_710054a2cc finds for `source`.
void fn_710091abfc(UnkMagnetMovablePlatform* self, void* source) {
    UnkWeakTarget* target = reinterpret_cast<UnkWeakTarget*>(fn_710054a2cc(source));
    if (!target)
        return;
    AssignRef(&self->movement, GetWeakHandle(target));
}
