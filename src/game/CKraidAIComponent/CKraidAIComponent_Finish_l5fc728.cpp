// The CKraidAIComponent unit. fn_71005fc728 runs the base step (fn_71010ff2b8), unlinks the listener at
// +0x14e08 from the target of the weak handle at +0x14e20 and drops the handle, sends the game manager's
// second loaded object the id hash of the singleton's part at +0x1d78 with mode 1, re-enables the player's
// input and unregisters the component (fn_7101199818). Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CInputComponent.h"
#include "game/UnkFlagListS7.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2702.h"
#include "game/UnkTU2730.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkPlayer;
struct UnkInputComponent;

using unk_flag_list::UnkFlagListKey;

extern UnkFlagListKey g_unk_71726f37a0 __attribute__((visibility("hidden")));

struct UnkKraidFinishId_l5fc728 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkKraidFinishScene_l5fc728 {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkKraidFinishRef_l5fc728 {
    u8 pad_0x0[0x10];
    UnkKraidFinishScene_l5fc728* target;
};

struct UnkKraidFinishManager_l5fc728 {
    u8 pad_0x0[0x29a0];
    UnkKraidFinishRef_l5fc728* field_0x29a0;
};

struct UnkKraidFinishPlayer_l5fc728 {
    u8 pad_0x0[0x1d8];
    UnkInputComponent* input;
};

struct UnkKraidAI_l5fc728 {
    u8 pad_0x0[0x14e08];
    u8 field_0x14e08[0x18];
    UnkWeakHandle* field_0x14e20;
};

static inline u64 PartHash_l5fc728(u64 offset) {
    return (*reinterpret_cast<UnkKraidFinishId_l5fc728**>(
                reinterpret_cast<char*>(&base::reflection::refl_GetSingleton()) + offset))
        ->hash;
}

// Sends a request under the two ids with the given mode (as UnkSpitSend does with mode 0).
inline void SendWithMode_l5fc728(void* target, u64 hash, u64 kind, u32 m) {
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = m;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    CAnimationComponent_SendActionToTrack(target, hash, kind, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
}

void fn_71005fc728(UnkKraidAI_l5fc728* self) {
    fn_71010ff2b8(reinterpret_cast<char*>(self));
    if (UnkWeakHandle* handle = self->field_0x14e20) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, self->field_0x14e08);
        ResetRef(&self->field_0x14e20);
    }
    UnkKraidFinishManager_l5fc728* manager = reinterpret_cast<UnkKraidFinishManager_l5fc728*>(g_unk_7101d47310);
    if (void* target = manager->field_0x29a0->target->field_0x220)
        SendWithMode_l5fc728(target, PartHash_l5fc728(0x1d78), 0xf4061ff25a2163d6, 1);
    UnkKraidFinishPlayer_l5fc728* player =
        reinterpret_cast<UnkKraidFinishPlayer_l5fc728*>(fn_7100dec788(g_unk_7101d47310));
    if (player && player->input)
        fn_7100872460(player->input, false, false, &g_unk_71726f37a0);
    fn_7101199818(g_unk_7101d47330, self);
}
