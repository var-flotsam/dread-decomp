// CWeightActivatedPlatformSmartObjectComponent unit (tools/tu_map.py, unit 1516): fn_7100a0c27c sends the owner's
// animation the id table's slot 0x1dc0 with mode 0x81, resets the linked object's colour (+0x174) while its user
// is busy, gives the owner key g_unk_710164b4c8 if the flag at +0x5a8 is set (and clears it), starts the effect
// named at +0x178 while the user is busy and restarts the owner's part at +0x2a0. Names and layouts are
// placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/math/CVector4D.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CCollisionComponent.h"
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

struct UnkCollisionContainer_b42;

struct UnkWapSlotsOwner_la0c27c {
    u8 pad_0x0[0x138];
    UnkSlots* slots;
};

struct UnkWapSlotsComp_la0c27c {
    u8 pad_0x0[0x50];
    UnkWapSlotsOwner_la0c27c* owner;
};

struct UnkWapOwner_la0c27c {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    UnkWapSlotsComp_la0c27c* slots;    // 0x228
    u8 pad_0x230[0x280 - 0x230];
    UnkWorld* effects;  // 0x280
    u8 pad_0x288[0x2a0 - 0x288];
    void* part;  // 0x2a0
};

struct UnkWapUser_la0c27c {
    u8 pad_0x0[0xf8];
    bool busy;  // 0xf8
};

struct UnkWapLink_la0c27c {
    u8 pad_0x0[0x10];
    UnkSmartObjectActor* object;
};

struct UnkWap_la0c27c {
    u8 pad_0x0[0x30];
    UnkWapOwner_la0c27c* owner;
    u8 pad_0x38[0xb0 - 0x38];
    UnkWapLink_la0c27c* link;  // 0xb0
    u8 pad_0xb8[0x174 - 0xb8];
    u32 linkedSlot;  // 0x174
    CStrId name;     // 0x178
    u8 pad_0x180[0x5a0 - 0x180];
    u64 kind;  // 0x5a0
    bool field_0x5a8;
};

static inline bool UserBusy_la0c27c(UnkWapLink_la0c27c* link) {
    if (link) {
        if (UnkSmartObjectActor* object = link->object)
            return reinterpret_cast<UnkWapUser_la0c27c*>(fn_71007cfd8c(object))->busy;
    }
    return false;
}

void fn_7100a0c27c(UnkWap_la0c27c* self) {
    UnkAnimationComponent* animation = self->owner->animation;
    u64 hash = GameManagerIdTable().field_0x1dc0.data->Key();
    u64 kind = self->kind;
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 0x81;
    CVector4D colour;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    CAnimationComponent_SendActionToTrack(animation, hash, kind, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);

    if (UserBusy_la0c27c(self->link)) {
        UnkWapSlotsOwner_la0c27c* owner = self->owner->slots->owner;
        colour.x = 1.0f;
        colour.y = 1.0f;
        colour.z = 1.0f;
        colour.w = 1.0f;
        fn_71001c287c(owner->slots, self->linkedSlot, 0, &colour, 1);
        colour.w = 1.0f;
        u32 linkedSlot = self->linkedSlot;
        fn_71001c2764(owner->slots, linkedSlot, 0, &colour, 1, true);
    }

    if (self->field_0x5a8) {
        fn_7100d7c828(reinterpret_cast<UnkActor*>(self->owner), &g_unk_710164b4c8, false);
        self->field_0x5a8 = false;
    }
    if (UserBusy_la0c27c(self->link))
        fn_710114fe8c(self->owner->effects, self->name.data->Key(), true, true);
    if (void* part = self->owner->part) {
        fn_7100861fc8(part, false);
        fn_7100861710(static_cast<UnkCollisionContainer_b42*>(part), true);
    }
}
