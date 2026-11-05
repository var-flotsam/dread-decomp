// base::snd::CSoundSystem unit (tools/tu_map.py, the name from reflection): calls fn_71000f9700 on
// the member at +0x8 with (a || b, b, false). Names are placeholders.
#include "types.h"

void fn_71000f9700(void* member, bool a, bool b, bool c);

struct UnkSoundSystem_b65a1 {
    u8 pad_0x0[0x8];
    u8 member[0x8];
};

void fn_7100108030(UnkSoundSystem_b65a1* self, bool a, bool b) {
    fn_71000f9700(self->member, a || b, b, false);
}
