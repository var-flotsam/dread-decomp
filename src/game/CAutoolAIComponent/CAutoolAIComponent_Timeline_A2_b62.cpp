// CAutoolAIComponent's unit (tools/tu_map.py): sets a key on the timeline of the part reached through +0x14768
// (+0x10, +0x30; fn_71003c6c3c) to the all-flags mask, unless it is already known. Kinds 0-2 pick one of three
// name hashes (their names are not recovered; the switch becomes a lookup table); any other kind uses the hash of
// the empty name. Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CName.h"
#include "game/CAutoolAIComponent.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkEvacPart_b61;
struct UnkEvacTimeline_b61;

struct UnkAutoolInner_b62a2 {
    u8 pad_0x0[0x30];
    UnkEvacPart_b61* part;
};
struct UnkAutoolLink_b62a2 {
    u8 pad_0x0[0x10];
    UnkAutoolInner_b62a2* inner;
};
struct UnkAutoolAI_b62a2 {
    u8 pad_0x0[0x14768];
    UnkAutoolLink_b62a2* link;
};

void fn_71003c4e64(UnkAutoolAI_b62a2* self, s32 kind) {
    if (!self->link || !self->link->inner)
        return;
    auto* timeline = reinterpret_cast<UnkTimelineComponent*>(fn_71003c6c3c(self->link->inner->part));
    if (!timeline)
        return;
    base::reflection::UnkReflTypeIdArg mask;
    base::global::CName key("");
    switch (kind) {
    case 0: key.hash = 0x7010fabc65b1b421ull; break;
    case 1: key.hash = 0x5d7fad51064ad162ull; break;
    case 2: key.hash = 0x72c2675c0fc379f0ull; break;
    }
    if (key.hash == static_cast<u64>(-1) || fn_7100a2be94(timeline, &key.hash))
        return;
    mask.id = UnkAll_SpitSend().value;
    fn_7100a2cb7c(timeline, &key.hash, &mask.id, false, -1.0f);
}
