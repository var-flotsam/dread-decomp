// CWeightActivatedPlatformSmartObjectComponent unit (tools/tu_map.py, unit 1516): fn_7100a0b86c sends the owner's
// animation the id table's slot 0x1c80 with mode 0x82, sets the slot colour (+0x170) from +0x160, resets the
// linked object's colour (+0x174) when there is one, gives the owner key g_unk_710164b4c0, sets the flag at +0x5a8
// and restarts the owner's part at +0x2a0. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/math/CVector4D.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CElectricReactionComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkSlots.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU253.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

using base::math::CVector4D;

extern UnkDesc_SpitSend g_unk_710164b968;  // reached through the GOT
extern const u64 g_unk_710164b4c0;
void fn_7100861fc8(void* part, bool flag);
struct UnkCollisionContainer_b42;
void fn_7100861710(UnkCollisionContainer_b42* self, bool force);

struct UnkWapSlotsOwner_la0b86c {
    u8 pad_0x0[0x138];
    UnkSlots* slots;
};

struct UnkWapSlotsComp_la0b86c {
    u8 pad_0x0[0x50];
    UnkWapSlotsOwner_la0b86c* owner;
};

struct UnkWapOwner_la0b86c {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    UnkWapSlotsComp_la0b86c* slots;    // 0x228
    u8 pad_0x230[0x2a0 - 0x230];
    void* part;  // 0x2a0
};

struct UnkWapLink_la0b86c {
    u8 pad_0x0[0x10];
    UnkSmartObjectActor* object;
};

struct UnkWap_la0b86c {
    u8 pad_0x0[0x30];
    UnkWapOwner_la0b86c* owner;
    u8 pad_0x38[0xb0 - 0x38];
    UnkWapLink_la0b86c* link;  // 0xb0
    u8 pad_0xb8[0x160 - 0xb8];
    CVector4D colour;  // 0x160
    u32 slot;          // 0x170
    u32 linkedSlot;    // 0x174
    u8 pad_0x178[0x5a0 - 0x178];
    u64 kind;  // 0x5a0
    bool field_0x5a8;
};

void fn_7100a0b86c(UnkWap_la0b86c* self) {
    UnkAnimationComponent* animation = self->owner->animation;
    u64 hash = GameManagerIdTable().field_0x1c80.data->Key();
    u64 kind = self->kind;
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 0x82;
    CVector4D colour;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7101176690(animation, hash, kind, &mode, &g_unk_710164b968, &g_unk_7172769740, &mask);

    UnkWapSlotsOwner_la0b86c* slotsOwner = self->owner->slots->owner;
    u32 slot = self->slot;
    fn_71001c2764(slotsOwner->slots, slot, 1, &self->colour, 1, true);

    if (UnkWapLink_la0b86c* link = self->link) {
        if (UnkSmartObjectActor* object = link->object) {
            fn_71007cfd8c(object);
            UnkWapSlotsOwner_la0b86c* owner = self->owner->slots->owner;
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

    fn_7100d7c828(reinterpret_cast<UnkActor*>(self->owner), &g_unk_710164b4c0, false);
    self->field_0x5a8 = true;
    if (void* part = self->owner->part) {
        fn_7100861fc8(part, false);
        fn_7100861710(static_cast<UnkCollisionContainer_b42*>(part), true);
    }
}
