// CCutscene unit (tools/tu_map.py; CCutscene::CTunableCutscene by reflection). Names and layouts are
// placeholders; the cutscene is src/game/CCutscene.cpp's UnkCutscene.
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU1134.h"
#include "types.h"

struct UnkTrackObject_71011781f4;
struct UnkObj_7101066878;
void fn_7101066878(UnkObj_7101066878* self, f32 time, bool flag, bool flag2);
struct UnkCutscene;
void fn_710106b2bc(UnkCutscene* self, f32 frame);

struct UnkCutsceneMode_b52 {
    u8 pad_0x0[0x8];
    s32 mode;
};

// An actor entry (0x30 bytes).
struct UnkCutsceneActor_b52 {
    u8 data[0x30];
};

struct UnkCutsceneView_b52 {
    u8 pad_0x0[0x150];
    UnkCutsceneActor_b52* actors;  // 0x150
    u32 actor_count;               // 0x158
    u8 pad_0x15c[0x1c];
    UnkCutsceneMode_b52* mode;     // 0x178
    UnkTrack track;                // 0x180
    u8 pad_0x1b0[0x300];
    s32 field_0x4b0;
    s32 field_0x4b4;
};

// Plays every actor and the cutscene's own track at `time` (a hair earlier in modes 3 and 2), then
// runs the fade track (fn_710106b2bc).
void fn_710106a890(UnkCutsceneView_b52* self, bool flag, f32 time) {
    if (self->field_0x4b4 == 3 || (self->mode && self->mode->mode == 2))
        time += -1e-5f;
    for (UnkCutsceneActor_b52 *it = self->actors, *end = it + self->actor_count; it != end; ++it)
        fn_7101066878(reinterpret_cast<UnkObj_7101066878*>(it), time,
                      flag && self->mode->mode != 1, self->field_0x4b0 != 3);
    if (!fn_710070d500(&self->track))
        return;
    reinterpret_cast<void (*)(UnkTrack*, bool, bool, f32)>(&fn_710070d874)(
        &self->track, flag && self->mode->mode != 1, self->field_0x4b0 != 3, time);
    fn_710106b2bc(reinterpret_cast<UnkCutscene*>(self), time);
}
