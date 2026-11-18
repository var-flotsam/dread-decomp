// CSamusGunComponent's unit (tools/tu_map.py): after the base update, marks the charge full when its time
// (+0x17c) reaches the limit (+0x174) and, unless the owner holds the screw attack, goes on to
// fn_7100859d0c. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CInventoryComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU1501.h"
#include "types.h"

struct UnkInventory;

struct UnkSamusGunOwner_l859c30 {
    u8 pad_0x0[0x208];
    void* field_0x208;        // 0x208
    u8 pad_0x210[0x60];
    UnkInventory* inventory;  // 0x270
    u8 pad_0x278[0x68];
    void* field_0x2e0;        // 0x2e0
};

struct UnkSamusGun_l859c30 {
    u8 pad_0x0[0x30];
    UnkSamusGunOwner_l859c30* owner;  // 0x30
    u8 pad_0x38[0x13c];
    f32 charge_limit;  // 0x174
    bool charging;     // 0x178
    u8 pad_0x179[0x3];
    f32 charge_time;   // 0x17c
    u8 pad_0x180[0x1b0];
    bool field_0x330;  // 0x330
    u8 pad_0x331[0x59];
    bool charged;      // 0x38a
};

void fn_710083e4fc(void* self);
void fn_7100859d0c(UnkSamusGun_l859c30* self);

static inline const UnkQualifiedId& GetQualifiedId_l859c30(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

void fn_7100859c30(UnkSamusGun_l859c30* self) {
    fn_710083e4fc(self);
    UnkSamusGunOwner_l859c30* owner = self->owner;
    if (!owner->field_0x2e0)
        return;
    if (self->charging) {
        self->charged = self->charge_time >= self->charge_limit;
        if (self->charged && owner && !self->field_0x330 && owner->field_0x208 &&
            fn_71009e75e8(static_cast<UnkFlagged*>(owner->field_0x208))) {
            if (UnkInventory* inventory = self->owner->inventory) {
                if (!fn_71011428bc(inventory, GetQualifiedId_l859c30(g_unk_717275a918)))
                    fn_7100859d0c(self);
            }
        }
    } else {
        self->charged = false;
    }
}
