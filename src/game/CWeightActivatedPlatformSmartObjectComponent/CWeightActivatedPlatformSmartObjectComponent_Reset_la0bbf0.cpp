// CWeightActivatedPlatformSmartObjectComponent unit (tools/tu_map.py, unit 1516): fn_7100a0bbf0 sends the owner's
// animation the id table's slot 0x1db8 (descriptor chosen by whether slot 0x1c78 plays) unless slot 0x1db8 or
// 0x1dc0 plays already, tells the world the name at +0x178 while the linked object's user is busy, and clears
// the counter at +0x14c. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CElectricReactionComponent.h"
#include "game/CFXComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU1730.h"
#include "types.h"

using base::global::CStrId;

struct UnkWorld;

struct UnkWapUser_la0bbf0 {
    u8 pad_0x0[0xf8];
    bool busy;  // 0xf8
};

struct UnkWapLink_la0bbf0 {
    u8 pad_0x0[0x10];
    UnkSmartObjectActor* object;
};

struct UnkWapOwner_la0bbf0 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x280 - 0x228];
    UnkWorld* world;  // 0x280
};

struct UnkWap_la0bbf0 {
    u8 pad_0x0[0x30];
    UnkWapOwner_la0bbf0* owner;
    u8 pad_0x38[0xb0 - 0x38];
    UnkWapLink_la0bbf0* link;  // 0xb0
    u8 pad_0xb8[0x14c - 0xb8];
    u32 field_0x14c;
    u8 pad_0x150[0x178 - 0x150];
    CStrId name;  // 0x178
    u8 pad_0x180[0x5a0 - 0x180];
    u64 kind;  // 0x5a0
};

void fn_7100a0bbf0(UnkWap_la0bbf0* self) {
    UnkAnimationComponent* animation = self->owner->animation;
    if (!fn_710117ae38(animation, GameManagerIdTable().field_0x1db8.data->Key(), false) &&
        !fn_710117ae38(animation, GameManagerIdTable().field_0x1dc0.data->Key(), false)) {
        u64 hash = GameManagerIdTable().field_0x1db8.data->Key();
        u64 kind = self->kind;
        base::reflection::UnkReflTypeIdArg mode;
        mode.id = 0;
        const UnkDesc_SpitSend* desc = fn_710117ae38(animation, GameManagerIdTable().field_0x1c78.data->Key(), false)
                                           ? &g_unk_7172769794
                                           : &g_unk_7172769740;
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7101176690(animation, hash, kind, &mode, &g_unk_710164b958, desc, &mask);
    }

    if (UnkWapLink_la0bbf0* link = self->link) {
        if (UnkSmartObjectActor* object = link->object) {
            UnkWapUser_la0bbf0* user = reinterpret_cast<UnkWapUser_la0bbf0*>(fn_71007cfd8c(object));
            if (user->busy)
                fn_710114fe8c(self->owner->world, self->name.data->Key(), 1, 1);
        }
    }
    self->field_0x14c = 0;
}
