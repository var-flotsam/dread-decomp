// CTeleporterUsableComponent unit (tools/tu_map.py): fn_7100a8f594 runs the base step (fn_7100a9fa34), sends
// the floating prop found for the actor (+0x30, fn_71007e4924) the id hash of the singleton's
// (fn_7100031894) part at +0xb10 with mode 0 (UnkSpitSend, src/game_s2/UnkSpitSend.h), then forwards a
// disabling input request (fn_7100872460) to the player's (fn_7100dec788) input component at +0x1d8.
// fn_7100a8eff8, for an event of kind 1, sends the part at +0xb08's hash the same way, then unlinks the
// listener at +0x568 from the target of the weak handle at +0x580 (fn_710117b3f0) and drops the handle.
// Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CCharClassEventScenarioComponent.h"
#include "game/CInputComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkPlayer;
struct UnkInputComponent;

extern __attribute__((visibility("hidden"))) char g_unk_71727519c0[];

struct UnkTeleporterId_b57 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkTeleporterPlayer_b57 {
    u8 pad_0x0[0x1d8];
    UnkInputComponent* input;
};

struct UnkTeleporterUsable_b57 {
    u8 pad_0x0[0x30];
    void* actor;
    u8 pad_0x38[0x568 - 0x38];
    u8 field_0x568[0x18];
    UnkWeakHandle* field_0x580;
};

void fn_7100a9fa34(UnkTeleporterUsable_b57* self);

static inline u64 PartHash_b57(u64 offset) {
    return (*reinterpret_cast<UnkTeleporterId_b57**>(reinterpret_cast<char*>(&base::reflection::refl_GetSingleton()) +
                                                     offset))
        ->hash;
}

void fn_7100a8f594(UnkTeleporterUsable_b57* self) {
    fn_7100a9fa34(self);
    if (self->actor) {
        if (UnkWeakTarget* target = fn_71007e4924(self->actor))
            UnkSpitSend(target, PartHash_b57(0xb10), 0xf4061ff25a2163d6);
    }
    UnkTeleporterPlayer_b57* player = reinterpret_cast<UnkTeleporterPlayer_b57*>(fn_7100dec788(g_unk_7101d47310));
    if (player && player->input)
        fn_7100872460(player->input, false, false, g_unk_71727519c0);
}

bool fn_7100a8eff8(UnkTeleporterUsable_b57* self, const s32* event) {
    if (*event == 1) {
        if (self->actor) {
            if (UnkWeakTarget* target = fn_71007e4924(self->actor))
                UnkSpitSend(target, PartHash_b57(0xb08), 0xf4061ff25a2163d6);
        }
        if (UnkWeakHandle* handle = self->field_0x580) {
            if (UnkWeakTarget* target = handle->target)
                fn_710117b3f0(target, self->field_0x568);
            ResetRef(&self->field_0x580);
        }
    }
    return true;
}
