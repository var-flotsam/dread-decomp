// CShockWavePoolComponent unit. fn_71009f6050: a release step; after the component base's (fn_71010c4230)
// it hands the owner's +0x30 back to the active player's actor (fn_710116a170) when it had taken it, then
// removes its two render items from the render manager. Names and layouts are placeholders.
#include "game/CComponent.h"
#include "game/UnkRenderManager.h"
#include "types.h"

struct UnkShockWavePoolTarget {
    u8 pad_0x0[0x1c0];
    void* field_0x1c0;
};

struct UnkShockWavePoolActor {
    u8 pad_0x0[0x30];
    UnkShockWavePoolTarget* field_0x30;
};

struct UnkShockWavePoolSlot {
    u8 pad_0x0[0x10];
    UnkShockWavePoolActor** field_0x10;
};

struct UnkShockWavePoolOwner {
    u8 pad_0x0[0x30];
    u8 field_0x30[0x8];
};

struct UnkShockWavePoolRenderItem {
    u8 pad_0x0[0x8];
    u8 item[0x8];
};

struct UnkShockWavePoolRelease {
    u8 pad_0x0[0x30];
    UnkShockWavePoolOwner* field_0x30;
    u8 pad_0x38[0x20c];
    bool field_0x244;
    u8 pad_0x245[0xd3];
    UnkShockWavePoolRenderItem* field_0x318;
    u32 field_0x320;
    bool field_0x324;
    UnkShockWavePoolRenderItem* field_0x328;
};

UnkShockWavePoolSlot* fn_7101139134(u32 index);
void fn_710116a170(void* self, void* item);

// The first player's actor's target, or null when there is none.
inline UnkShockWavePoolTarget* PlayerTarget() {
    UnkShockWavePoolSlot* slot = fn_7101139134(1);
    if (!slot->field_0x10) return nullptr;
    UnkShockWavePoolActor* actor = *slot->field_0x10;
    if (!actor) return nullptr;
    return actor->field_0x30;
}

static void RemoveRenderItem(UnkShockWavePoolRenderItem*& item) {
    if (item) {
        fn_71002199d8(g_unk_7101d47300, reinterpret_cast<UnkItemBase_71011a91c4*>(item->item), 0);
        item = nullptr;
    }
}

void fn_71009f6050(UnkShockWavePoolRelease* self) {
    fn_71010c4230(reinterpret_cast<UnkHandleOwner_71010c4230*>(self));
    if (self->field_0x244) {
        bool& taken = self->field_0x324;
        UnkShockWavePoolTarget* target = PlayerTarget();
        if (((target != nullptr) & taken) && target->field_0x1c0) {
            fn_710116a170(target->field_0x1c0, self->field_0x30->field_0x30);
            self->field_0x320 = 0;
        }
        taken = false;
    }
    RemoveRenderItem(self->field_0x328);
    RemoveRenderItem(self->field_0x318);
}
