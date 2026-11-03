// In the CEmergencyLightElectricReactionComponent unit: hands the actor (+0x30) one of two timeline ids
// (fn_7100d7c828), chosen by `flag`. Names and layouts are placeholders.
#include "game/UnkTimelineComponent.h"
#include "types.h"

extern const u64 g_unk_710164ab00;
extern const u64 g_unk_710164ab08;

struct UnkEmergencyLight {
    u8 pad_0x0[0x30];
    UnkActor* actor;
};

void fn_71007d0584(UnkEmergencyLight* self, bool flag) {
    fn_7100d7c828(self->actor, flag ? &g_unk_710164ab08 : &g_unk_710164ab00, false);
}
