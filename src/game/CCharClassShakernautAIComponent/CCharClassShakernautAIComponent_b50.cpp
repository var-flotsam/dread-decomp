// CCharClassShakernautAIComponent unit (tools/tu_map.py). fn_71006acd1c and its identical copy
// fn_71006acdc0 pick one of four path ids (the unit's statics g_unk_71727062a8..c0) at random
// (fn_71012395a0) and by whether the actor's animation (+0x220) plays a CCharClassSetAnimStateTrack
// (fn_710117b08c with fn_7100c4c1b8). Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU2772.h"
#include "types.h"

using base::global::CFilePathStrId;

// Four ids, interleaved: [0] and [2] of each (0x...62a8 and 0x...62b8; 0x...62b0 and 0x...62c0).
extern CFilePathStrId g_unk_71727062a8[3] __attribute__((visibility("hidden")));
extern CFilePathStrId g_unk_71727062b0[3] __attribute__((visibility("hidden")));

struct UnkShakernautActor_b50 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;
};

static inline const CFilePathStrId& PickPath_b50(UnkShakernautActor_b50* actor) {
    if (fn_71012395a0() & 1) {
        UnkTrackQuery query = {fn_7100c4c1b8(), nullptr, -1.0f};
        fn_710117b08c(actor->animation, &query);
        return query.result ? g_unk_71727062b0[0] : g_unk_71727062b0[2];
    } else {
        UnkTrackQuery query = {fn_7100c4c1b8(), nullptr, -1.0f};
        fn_710117b08c(actor->animation, &query);
        return query.result ? g_unk_71727062a8[2] : g_unk_71727062a8[0];
    }
}

const CFilePathStrId& fn_71006acd1c(void* self, UnkShakernautActor_b50* actor) {
    return PickPath_b50(actor);
}

const CFilePathStrId& fn_71006acdc0(void* self, UnkShakernautActor_b50* actor) {
    return PickPath_b50(actor);
}
