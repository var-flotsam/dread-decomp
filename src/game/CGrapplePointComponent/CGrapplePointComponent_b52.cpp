// CGrapplePointComponent (unit 1278): the grapple beam attaching to the point. Names and layouts are
// placeholders.
#include "game/CAbilityEnergyWave.h"
#include "game/UnkTimelineComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkScriptCaller;

extern const u64 g_unk_710164ac10;

struct UnkGrapplePointActor {
    u8 unk_0x0[0x1e0];
    UnkScriptCaller* script;
};

struct UnkGrapplePointComponent {
    u8 unk_0x0[0x30];
    UnkGrapplePointActor* actor;
    u8 unk_0x38[0x8];
    UnkWeakHandle* attached;  // 0x40
};

// Remembers what attached (through its weak handle), tells the script and plays the timeline.
void fn_71008226d0(UnkGrapplePointComponent* self, UnkWeakTarget* grapple) {
    AssignRef(&self->attached, GetWeakHandle(grapple));
    if (self->actor->script) fn_7100305088(self->actor->script, "OnGrappleBeamAttached", "");
    fn_7100d7c828(reinterpret_cast<UnkActor*>(self->actor), &g_unk_710164ac10, false);
}
