// CWeightActivatedPlatformSmartObjectComponent unit (tools/tu_map.py, unit 1516): fn_7100a0c60c sends the owner's
// animation the id table's slot 0x1c80 with mode 2, resets the linked object's colour (+0x174) when there is one
// and stops the effect named at +0x178 while the linked object's user is busy. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/math/CVector4D.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CElectricReactionComponent.h"
#include "game/CFXComponent.h"
#include "game/CWeightActivatedPlatformSmartObjectComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkSlots.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU253.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector4D;

struct UnkWapSlotsOwner_la0c60c {
    u8 pad_0x0[0x138];
    UnkSlots* slots;
};

struct UnkWapSlotsComp_la0c60c {
    u8 pad_0x0[0x50];
    UnkWapSlotsOwner_la0c60c* owner;
};

struct UnkWapOwner_la0c60c {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    UnkWapSlotsComp_la0c60c* slots;    // 0x228
    u8 pad_0x230[0x280 - 0x230];
    UnkRinkaControl_ruc* effects;  // 0x280
};

struct UnkWapUser_la0c60c {
    u8 pad_0x0[0xf8];
    bool busy;  // 0xf8
};

struct UnkWapLink_la0c60c {
    u8 pad_0x0[0x10];
    UnkSmartObjectActor* object;
};

struct UnkWap_la0c60c {
    u8 pad_0x0[0x30];
    UnkWapOwner_la0c60c* owner;
    u8 pad_0x38[0xb0 - 0x38];
    UnkWapLink_la0c60c* link;  // 0xb0
    u8 pad_0xb8[0x174 - 0xb8];
    u32 linkedSlot;  // 0x174
    CStrId name;     // 0x178
    u8 pad_0x180[0x5a0 - 0x180];
    u64 kind;  // 0x5a0
};

static inline bool UserBusy_la0c60c(UnkWapLink_la0c60c* link) {
    if (link) {
        if (UnkSmartObjectActor* object = link->object)
            return reinterpret_cast<UnkWapUser_la0c60c*>(fn_71007cfd8c(object))->busy;
    }
    return false;
}

void fn_7100a0c60c(UnkWap_la0c60c* self) {
    UnkAnimationComponent* animation = self->owner->animation;
    u64 hash = GameManagerIdTable().field_0x1c80.data->Key();
    u64 kind = self->kind;
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 2;
    CVector4D colour;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7101176690(animation, hash, kind, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);

    if (UnkWapLink_la0c60c* link = self->link) {
        if (UnkSmartObjectActor* object = link->object) {
            fn_71007cfd8c(object);
            UnkWapSlotsOwner_la0c60c* owner = self->owner->slots->owner;
            colour.x = 1.0f;
            colour.y = 1.0f;
            colour.z = 1.0f;
            colour.w = 1.0f;
            fn_71001c287c(owner->slots, self->linkedSlot, 0, &colour, 1);
            colour.w = 0.0f;
            u32 linkedSlot = self->linkedSlot;
            fn_71001c2764(owner->slots, linkedSlot, 0, &colour, 1, true);
        }
    }

    if (UserBusy_la0c60c(self->link))
        fn_710114ffb8(self->owner->effects, self->name.data->Key(), false, false);
}
