// CCharClassBillboardComponent unit (tools/tu_map.py unit 1154): fn_710072fd94 hands the key at +0x30 of
// the object at +0xd0 to the billboard manager g_unk_71726da048 (created on first use, as in
// src/game/CBillboardComponent/CBillboardComponent_b39.cpp; fn_7100e1ceb0), removes every item of the two
// lists at +0xe0 and +0x108 from the render manager (fn_71002199d8) and empties both lists. Names and
// layouts are placeholders.
#include "base/UnkVector.h"
#include "game/CDizzeanSwarmControllerComponent.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU2560.h"
#include "types.h"

void fn_7100e1ceb0(void* manager, const u64* key);

struct UnkKeyed_bb {
    u8 pad_0x0[0x30];
    u64 key;
};

struct UnkItemList_bb {
    UnkItemBase_71011a91c4** data;
    u32 count;
    u8 pad_0xc[0x1c];
};

struct UnkCharClassBillboard_bb {
    u8 pad_0x0[0xd0];
    UnkKeyed_bb* field_0xd0;
    u8 pad_0xd8[0x8];
    UnkItemList_bb items[2];  // 0xe0, 0x108
};

void fn_710072fd94(UnkCharClassBillboard_bb* self) {
    void* manager = g_unk_71726da048;
    if (!manager) {
        manager = fn_71000839a0(0xe0, 0x3f);
        fn_7100e1bca4(manager);
        g_unk_71726da048 = manager;
    }
    u64 key = self->field_0xd0->key;
    fn_7100e1ceb0(manager, &key);
    for (UnkItemList_bb* list = self->items; list != self->items + 2; ++list)
        for (UnkItemBase_71011a91c4 **it = list->data, **end = list->data + list->count; it != end;) {
            UnkItemBase_71011a91c4* item = *it++;
            fn_71002199d8(g_unk_7101d47300, item, 0);
        }
    self->items[0].count = 0;
    self->items[1].count = 0;
}
