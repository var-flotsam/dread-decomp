// CCharClassStopCameraFXPresetEvent (unit 2302 of tools/tu_map.py): when the event names a preset (the string id
// at +0x10 is not the empty string), hands it to the game manager's fn_7100e03a78. Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "types.h"

void fn_7100e03a78(UnkGameManager*, void*);

struct UnkStopCameraFXPresetEvent {
    u8 pad_0x0[0x10];
    base::global::UnkStringData* preset;
};

void fn_7100ce2228(UnkStopCameraFXPresetEvent* self) {
    if (self->preset != &g_unk_7101d47108->empty)
        fn_7100e03a78(g_unk_7101d47310, &self->preset);
}
