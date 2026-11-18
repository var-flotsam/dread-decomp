// CWeightActivatedPlatformSmartObjectComponent unit (tools/tu_map.py, unit 1516): fn_7100a0ad9c, whether the
// platform's smart object can be used now. Hands the player back (fn_7100df31b8) when it holds the game manager's
// handle and the platform has an action id; uses the linked object when its animation allows it, otherwise
// blocks the player's input for the track's length. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CElectricReactionComponent.h"
#include "game/CInputComponent.h"
#include "game/CWeightActivatedPlatformSmartObjectComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1428.h"
#include "game/UnkTU2553.h"
#include "types.h"

using base::global::CStrId;

struct UnkAnimationComponent;
struct UnkUsable_A5_b73;
struct UnkWeakTarget;
struct UnkPlatformUpdate_A5_b76;
bool fn_710117ad58(UnkAnimationComponent* self);
f32 fn_7101178134(UnkAnimationComponent* self, u64 key);
bool fn_7100aa1098(UnkUsable_A5_b73* self, UnkWeakTarget* target);

struct UnkWapHandle_la0ad9c {
    u8 pad_0x0[0x10];
    void* target;
};
// The game manager's second handle (+0x29a0, of the four at +0x2998).
struct UnkWapManager_la0ad9c {
    u8 pad_0x0[0x29a0];
    UnkWapHandle_la0ad9c* handle;
};

struct UnkWapPlayer_la0ad9c {
    u8 pad_0x0[0x1d8];
    UnkInputComponent* input;
    u8 pad_0x1e0[0x210 - 0x1e0];
    void* field_0x210;
    u8 pad_0x218[0x220 - 0x218];
    UnkAnimationComponent* animation;
};

struct UnkWapLink_la0ad9c {
    u8 pad_0x0[0x10];
    UnkSmartObjectActor* object;
};

struct UnkWapCheck_la0ad9c {
    u8 pad_0x0[0x68];
    CStrId action;
    u8 pad_0x70[0xb0 - 0x70];
    UnkWapLink_la0ad9c* link;
    u8 pad_0xb8[0x5a0 - 0xb8];
    u64 track;
};
static_assert(__builtin_offsetof(UnkWapCheck_la0ad9c, track) == 0x5a0, "layout");

static inline void* HandleTarget_la0ad9c(UnkGameManager* manager) {
    UnkWapHandle_la0ad9c* handle = reinterpret_cast<UnkWapManager_la0ad9c*>(manager)->handle;
    return handle ? handle->target : nullptr;
}

// The platform (UnkPlatformUpdate_A5_b76, whose layout does not yet name +0x68 and +0xb0) seen through this
// file's view of it.
bool fn_7100a0ad9c(UnkPlatformUpdate_A5_b76* platform) {
    auto* self = reinterpret_cast<UnkWapCheck_la0ad9c*>(platform);
    void* holder = HandleTarget_la0ad9c(g_unk_7101d47310);
    UnkPlayer* current = fn_7100dec788(g_unk_7101d47310);
    if ((holder == current) & (self->action.data != &g_unk_7101d47108->empty))
        fn_7100df31b8(g_unk_7101d47310, true, false, 2, CStrId(base::global::CStrId::Empty()), false);

    auto* player = reinterpret_cast<UnkWapPlayer_la0ad9c*>(fn_7100dec788(g_unk_7101d47310));
    UnkSmartObjectUserA* user = nullptr;
    if (self->link && self->link->object)
        user = fn_71007cfd8c(self->link->object);
    UnkAnimationComponent* animation = player ? player->animation : nullptr;
    if (holder != current && user && animation && fn_710117ad58(animation))
        return fn_7100aa1098(reinterpret_cast<UnkUsable_A5_b73*>(user), reinterpret_cast<UnkWeakTarget*>(player));

    if (!fn_710087238c(player->input)) {
        UnkInputComponent* input = player->input;
        f32 time = fn_7101178134(animation, self->track) + 0.1f;
        fn_71008724b4(time, input, &g_unk_7172743570, false);
        if (player->field_0x210)
            fn_710094b508(player->field_0x210);
    }
    return false;
}
