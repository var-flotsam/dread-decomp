// CWeightActivatedPlatformSmartObjectComponent unit (tools/tu_map.py, unit 1516): fn_7100a0b154 sends the owner's
// animation the id table's slot 0x1c78 (descriptor chosen by whether slot 0x1db8 plays) unless slot 0x1c78 or
// 0x1c80 plays already, stops the world's effect named at +0x178 while the linked object's user is busy, points
// the player's movement at the owner, gives the owner key g_unk_710164b4d0 and sets the flag at +0x5a8.
// Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CElectricReactionComponent.h"
#include "game/CFXComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU2553.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

using base::global::CStrId;

struct UnkCharacterMovementTarget;
struct UnkWeakTarget;
void CCharacterMovement_SetWeakTarget(UnkCharacterMovementTarget* self, UnkWeakTarget* target);
extern const u64 g_unk_710164b4d0;

struct UnkWapPlayer_la0b154 {
    u8 pad_0x0[0x1f8];
    UnkCharacterMovementTarget* movement;  // 0x1f8
};

struct UnkWapUser_la0b154 {
    u8 pad_0x0[0xf8];
    bool busy;  // 0xf8
};

struct UnkWapLink_la0b154 {
    u8 pad_0x0[0x10];
    UnkSmartObjectActor* object;
};

struct UnkWapOwner_la0b154 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x280 - 0x228];
    UnkRinkaControl_ruc* effects;  // 0x280
};

struct UnkWap_la0b154 {
    u8 pad_0x0[0x30];
    UnkWapOwner_la0b154* owner;
    u8 pad_0x38[0xb0 - 0x38];
    UnkWapLink_la0b154* link;  // 0xb0
    u8 pad_0xb8[0x178 - 0xb8];
    CStrId name;  // 0x178
    u8 pad_0x180[0x5a0 - 0x180];
    u64 kind;  // 0x5a0
    bool field_0x5a8;
};

void fn_7100a0b154(UnkWap_la0b154* self) {
    UnkAnimationComponent* animation = self->owner->animation;
    if (!fn_710117ae38(animation, GameManagerIdTable().field_0x1c78.data->Key(), false) &&
        !fn_710117ae38(animation, GameManagerIdTable().field_0x1c80.data->Key(), false)) {
        u64 hash = GameManagerIdTable().field_0x1c78.data->Key();
        u64 kind = self->kind;
        base::reflection::UnkReflTypeIdArg mode;
        mode.id = 0;
        const UnkDesc_SpitSend* desc = fn_710117ae38(animation, GameManagerIdTable().field_0x1db8.data->Key(), false)
                                           ? &g_unk_7172769794
                                           : &g_unk_7172769740;
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        CAnimationComponent_SendActionToTrack(animation, hash, kind, &mode, &g_unk_710164b958, desc, &mask);

        if (UnkWapLink_la0b154* link = self->link) {
            if (UnkSmartObjectActor* object = link->object) {
                UnkWapUser_la0b154* user = reinterpret_cast<UnkWapUser_la0b154*>(fn_71007cfd8c(object));
                if (user->busy)
                    fn_710114ffb8(self->owner->effects, self->name.data->Key(), false, false);
            }
        }
    }

    UnkWapPlayer_la0b154* player = reinterpret_cast<UnkWapPlayer_la0b154*>(fn_7100dec788(g_unk_7101d47310));
    if (player && player->movement)
        CCharacterMovement_SetWeakTarget(player->movement, reinterpret_cast<UnkWeakTarget*>(self->owner));
    fn_7100d7c828(reinterpret_cast<UnkActor*>(self->owner), &g_unk_710164b4d0, false);
    self->field_0x5a8 = true;
}
