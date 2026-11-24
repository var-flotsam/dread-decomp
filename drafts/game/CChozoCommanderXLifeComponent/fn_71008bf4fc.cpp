// CChozoCommanderXLifeComponent's unit (tools/tu_map.py unit 1369): on entering state 2, registers the
// component (fn_7101199818), sends the owner's and the player's animations a request and listens to the
// owner's, then places the "ev_commanderx_rock01" actor at the owner's position, enables it and sends its
// animation a request. Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/CInputComponent.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2730.h"
#include "game/UnkTU341.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

void fn_71008c0268();
extern __attribute__((visibility("hidden"))) char g_unk_71727302a0[];

// A listener on an animation's weak handle, calling `callback` on `object`.
struct UnkCommanderXListener_l8bf4fc {
    void** vtable;
    void* object;          // 0x8
    void (*callback)();    // 0x10
    UnkWeakHandle* link;   // 0x18

    // Drops the link, unregistering from its target first.
    void Unlisten() {
        if (UnkWeakHandle* l = link) {
            bool held = true;
            if (UnkWeakTarget* target = l->target) {
                fn_710117b3f0(target, this);
                l = link;
                held = l != nullptr;
            }
            if (held) {
                if (--l->ref_count == 0)
                    l->vf_0x10();
                link = nullptr;
            }
        }
    }
    void Listen(UnkWeakTarget* target) {
        Unlisten();
        CAbilityGhostAura_AssignWeakHandleToRef(target, &link);
        fn_710117b3e8(link ? link->target : nullptr, this);
    }
};

// The rock actor: an actor whose vf_0x118 sets its transform.
struct UnkCommanderXRock_l8bf4fc {
    UnkActor actor;
    u8 pad_0x380[0x30];
    bool field_0x3b0;

    void vf_0x118(void* transform, const u32* flags) {
        reinterpret_cast<void (*)(UnkCommanderXRock_l8bf4fc*, void*, const u32*)>(actor.vtable[0x118 / 8])(
            this, transform, flags);
    }
};
static_assert(__builtin_offsetof(UnkCommanderXRock_l8bf4fc, field_0x3b0) == 0x3b0, "layout");

struct UnkCommanderXLife_l8bf4fc {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x210];
    UnkCommanderXListener_l8bf4fc listener;  // 0x248
};
static_assert(__builtin_offsetof(UnkCommanderXLife_l8bf4fc, listener) == 0x248, "layout");

using base::reflection::UnkReflTypeIdArg;

// Sends `target` the request `hash` with `mode` under the all-flags mask; the arguments go in `request`
// and `mask`.
static inline void Send(void* target, u64 hash, UnkReflTypeIdArg* request, u32 mode, UnkReflTypeIdArg* mask) {
    request->id = mode;
    mask->id = UnkAll_SpitSend().value;
    CAnimationComponent_SendActionToTrack(target, hash, 0xf4061ff25a2163d6, request, &g_unk_710164b958, &g_unk_7172769740, mask);
}

// The actor named `name`.
static inline UnkCommanderXRock_l8bf4fc* FindActor(const CStrId& name) {
    u64 key = name.data->Key();
    u32 kind = g_unk_71727bebb8;
    return reinterpret_cast<UnkCommanderXRock_l8bf4fc*>(fn_7100debbc8(g_unk_7101d47310, &key, &kind));
}

void fn_71008bf4fc(UnkCommanderXLife_l8bf4fc* self, s32 previous, s32 state) {
    // The arguments of the three requests below.
    UnkReflTypeIdArg owner_request, owner_mask;
    UnkReflTypeIdArg player_request, player_mask;
    UnkReflTypeIdArg rock_request, rock_mask;

    if (state == 2) {
        fn_7101199818(g_unk_7101d47330, self);
        if (void* animation = self->owner->field_0x220) {
            Send(animation, 0x87987b4938241bde, &owner_request, 2, &owner_mask);
            self->listener.object = self;
            self->listener.callback = fn_71008c0268;
            self->listener.Listen(static_cast<UnkWeakTarget*>(animation));
        }

        if (UnkActor* player = reinterpret_cast<UnkActor*>(fn_7100dec788(g_unk_7101d47310))) {
            if (player->field_0x1d8)
                fn_7100872460(static_cast<UnkInputComponent*>(player->field_0x1d8), true, false, g_unk_71727302a0);
            if (void* animation = player->field_0x220)
                Send(animation, 0x53ec1c21635fc6d7, &player_request, 2, &player_mask);
        }

        static CStrId s_rock("ev_commanderx_rock01", true);
        if (UnkCommanderXRock_l8bf4fc* rock = FindActor(s_rock)) {
            UnkActor* owner = self->owner;
            rock->actor.field_0x44 = owner->field_0x44;
            rock->actor.field_0x54 = owner->field_0x54;
            rock->actor.field_0x64 = owner->field_0x64;
            u32 flags = 1;
            rock->vf_0x118(rock->actor.field_0x38, &flags);
            rock->field_0x3b0 = true;
            fn_7100232864(reinterpret_cast<UnkEntityState*>(rock), true);
            if (void* animation = rock->actor.field_0x220)
                Send(animation, 0x0582ae157e30b675, &rock_request, 0, &rock_mask);
        }
    }
}
