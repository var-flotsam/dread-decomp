// CCooldownXBossWindTunnelAttack's unit (tools/tu_map.py): when the owner's (+0x8) AI component (+0x240) is a
// CCooldownXBossAIComponent, follows the handle that fn_71003b0c4c gives for its +0x15690 to an entity; when that
// entity is enabled (+0x1b4) and, if it needs one (+0x1b0), has its +0x1a8, sets the id 0x370180b673bdeb7f on the
// entity's timeline (+0x2e8) to the all-flags mask unless it is already set, or switches the entity off
// (fn_7100232864) when its timeline does not know the id. Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

using base::reflection::CClass;

struct UnkEntityState;
void fn_7100232864(UnkEntityState* entity, bool value);  // as src/game/CCentralUnitComponent/CCentralUnitComponent_b46.cpp

struct UnkWindTunnelEntity_b61a2 {
    u8 pad_0x0[0x1a8];
    void* field_0x1a8;
    u8 field_0x1b0;
    u8 pad_0x1b1[3];
    u8 field_0x1b4;
    u8 pad_0x1b5[0x133];
    void* field_0x2e8;
};

struct UnkWindTunnelRef_b61a2 {
    u8 pad_0x0[0x10];
    UnkWindTunnelEntity_b61a2* entity;
};

struct UnkWindTunnelHandle_b61a2 {
    UnkWindTunnelRef_b61a2* ref;
};
// Typed as CHydrogigaZiplineRailComponent_b49.cpp declares it (one signature per symbol).
struct UnkZiplineLink_b49;
struct UnkZiplineLinkTarget_b49;
UnkZiplineLinkTarget_b49** fn_71003b0c4c(UnkZiplineLink_b49* source);

struct UnkWindTunnelAI_b61a2 {
    virtual const CClass* vf_0x0();
};

struct UnkWindTunnelFan_b61a2 {
    u8 pad_0x0[0x8];
    UnkActor* owner;
};

void fn_71004cbaa8(UnkWindTunnelFan_b61a2* self) {
    auto* ai = static_cast<UnkWindTunnelAI_b61a2*>(self->owner->field_0x240);
    if (!ai || !fn_710009d50c(ai->vf_0x0(), fn_71004cf318()))
        return;
    UnkWindTunnelRef_b61a2* ref = reinterpret_cast<UnkWindTunnelHandle_b61a2*>(fn_71003b0c4c(reinterpret_cast<UnkZiplineLink_b49*>(reinterpret_cast<u8*>(ai) + 0x15690)))->ref;
    if (!ref)
        return;
    UnkWindTunnelEntity_b61a2* entity = ref->entity;
    if (!entity || !entity->field_0x1b4)
        return;
    if (entity->field_0x1b0 && !entity->field_0x1a8)
        return;
    base::reflection::UnkReflTypeIdArg mask;
    u64 id = 0x370180b673bdeb7full;
    auto* timeline = static_cast<UnkTimelineComponent*>(entity->field_0x2e8);
    if (timeline && fn_7100a2c13c(timeline, &id)) {
        if (fn_7100a2be94(timeline, &id))
            return;
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, &id, &mask.id, false, -1.0f);
    } else {
        fn_7100232864(reinterpret_cast<UnkEntityState*>(entity), false);
    }
}
