// Plays a cutscene actor track on the owner's character class and shows or hides the owner to
// match it. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/CActorComponent.h"
#include "game/CAnimationComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU1134.h"
#include "game/UnkTU1728.h"
#include "game/UnkTU341.h"
#include "types.h"

struct UnkTrackObject_71011781f4;
struct UnkCutsceneTimeline;
struct UnkAnimTiming;
struct UnkStruct_71002334fc;
struct UnkStruct_710024fa0c;

struct UnkActor_7101066878 {
    u8 pad_0x0[0x1b3];
    u8 field_0x1b3;
    u8 pad_0x1b4[0x6c];
    void* field_0x220;
    u8 pad_0x228[0x58];
    UnkStruct_710024fa0c* field_0x280;
};

struct UnkObj_7101066878 {
    u8 pad_0x0[0x18];
    UnkActor_7101066878* field_0x18;
};

static const u64 kActorTrack = 0xf4061ff25a2163d6;

void fn_7101066878(UnkObj_7101066878* self, f32 time, bool flag, bool flag2) {
    UnkTrack* track = fn_71011771e0(self->field_0x18->field_0x220, kActorTrack);
    if (fn_710070d4e8(track))
        reinterpret_cast<void (*)(UnkTrackObject_71011781f4*, bool, bool, f32)>(&fn_710070d874)(
            reinterpret_cast<UnkTrackObject_71011781f4*>(track), flag, flag2, time);
    UnkTrack* track2 = fn_71011771e0(self->field_0x18->field_0x220, kActorTrack);
    fn_710070d4e8(track2);
    bool visible = fn_710117aaac(self->field_0x18->field_0x220, fn_7100cf3f28(),
                                 fn_7100b8aff0(time, reinterpret_cast<const UnkAnimTiming*>(fn_710070d4e8(track2))));
    UnkActor_7101066878* actor = self->field_0x18;
    if (visible != (actor->field_0x1b3 != 0)) {
        fn_71002334fc(reinterpret_cast<UnkStruct_71002334fc*>(actor), visible);
        if (UnkStruct_710024fa0c* comp = self->field_0x18->field_0x280)
            fn_710024fa0c(comp, visible);
    }
}
