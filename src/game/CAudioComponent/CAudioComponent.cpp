// CAudioComponent: a reset (fn_710071f574). Stops its sounds (fn_7100719064 with true, false, true,
// true), then clears the 32 bytes at +0x118. Names and layouts are placeholders.
#include "game/CAudioComponent.h"
#include "types.h"

struct UnkEntitySoundComponent;

struct UnkAudioComponent {
    u8 pad_0x0[0x118];
    u64 field_0x118[4];
};

void fn_710071f574(UnkAudioComponent* self) {
    fn_7100719064(reinterpret_cast<UnkEntitySoundComponent*>(self), true, false, true, true);
    __builtin_memset(self->field_0x118, 0, sizeof(self->field_0x118));
}
