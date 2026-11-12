// behaviortree::CSetTimelineAction (tools/tu_map.py): the node's start. After the base routine
// (fn_7100394bd0), when the id name (+0x30) is set and the owner (+0x18) has a timeline (+0x2e8), sets
// the name's hash (+0x30 of its data) to the all-flags mask when the flag at +0x38 is set, else removes
// it. Names and layouts are placeholders.
#include "../../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

using base::global::UnkStrId;

struct UnkSelf;
struct UnkStruct_0x68;
void fn_7100394bd0(UnkStruct_0x68* self);

struct UnkSetTimelineAction_b60a2 {
    u8 pad_0x0[0x18];
    UnkActor* owner;
    u8 pad_0x20[0x10];
    UnkStrId name;
    bool enable;
};

void fn_71003794f8(UnkSetTimelineAction_b60a2* self) {
    fn_7100394bd0(reinterpret_cast<UnkStruct_0x68*>(self));
    if (self->name.data == &g_unk_7101d47108->empty || !self->owner)
        return;
    auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8);
    if (!timeline)
        return;
    if (!self->enable)
        goto remove;
    base::reflection::UnkReflTypeIdArg mask;  // a goto jumps over it: its own slot
    {
        u64 id = *reinterpret_cast<u64*>(reinterpret_cast<u8*>(self->name.data) + 0x30);
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, &id, &mask.id, false, -1.0f);
        return;
    }
remove:
    u64 id = *reinterpret_cast<u64*>(reinterpret_cast<u8*>(self->name.data) + 0x30);
    fn_7100a2b7b8(timeline, &id);
}
