// Dropped-item manager (unit 2562, CLogicDroppedItemManager::CTunableDroppableBillboard): sets up the
// missile billboard, two entries with their ids, colours and counts, and hands them to the billboard.
// Names and layouts are placeholders.
#include "base/UnkNoArg.h"
#include "base/color/CColor4F.h"
#include "base/global/CStrId.h"
#include "game/CDropLayout.h"
#include "game/CLogicDroppedItemManager/CTunableDroppableBillboard.h"
#include "game/UnkTU2561.h"
#include "game/UnkTU2562.h"
#include "game/UnkTU341.h"
#include "types.h"

using base::color::CColor4F;
using base::global::CStrId;


// The fields of the tunable these billboards read.
struct UnkBillboardTunable_e289ec {
    u8 pad_0x0[0x4c];
    f32 fMissileAmount;  // 0x4c
    f32 fMissileBigAmount;  // 0x50
    f32 fPowerBombAmount;  // 0x54
    u8 pad_0x58[0x20];
    u8 field_0x78;
};

struct UnkBillboardManager_e289ec {
    u8 pad_0x0[0x7808];
    u32 field_0x7808;
    u8 pad_0x780c[0xc];
    u32 field_0x7818;
    u8 pad_0x781c[0x7858 - 0x781c];
    CDrop* current;  // 0x7858
    u8 pad_0x7860[0x8270 - 0x7860];
    CDrop billboard;  // 0x8270
};

static inline const UnkBillboardTunable_e289ec& Tunable() {
    return reinterpret_cast<const UnkBillboardTunable_e289ec&>(g_unk_71727c10c8);
}

static inline void Show(CDrop* billboard, const CStrId& id, void* owner, f32 count, const UnkDropTable& set) {
    billboard->item = id;
    billboard->owner = owner;
    billboard->callback = fn_7100e2f994;
    billboard->amount = count;
    billboard->table = set;
    g_unk_717269c4d8 = true;
    billboard->Start();
    g_unk_717269c4d8 = false;
}

void fn_7100e289ec(UnkBillboardManager_e289ec* self) {
    UnkDropTable set;
    set.count = 2;

    UnkDropTableEntry& second = set.entries[1];
    second.field_0x30 = 0;
    second.field_0x38 = self->field_0x7808;
    {
        CStrId id("Missile0", true);
        second.id.ReleaseAndTake(id);
    }
    second.field_0x8 = g_unk_71727c1088[Tunable().field_0x78];
    second.color = CColor4F(1.0f, 1.0f, 1.0f, 1.0f);
    second.field_0x20 = CColor4F(0.0f, 0.0f, 0.0f, 0.0f);
    second.field_0x34 = 1.0f;
    fn_7100e2c124(0.5f, UnkNoArg(), reinterpret_cast<UnkDropTarget_ovb0208*>(&g_unk_71727c1058), &second.id, 0x1e, 0x1e, 1, 0, 0, true);

    UnkDropTableEntry& first = set.entries[0];
    first.field_0x30 = 0;
    first.field_0x38 = self->field_0x7818;
    {
        CStrId id("Missile1", true);
        first.id.ReleaseAndTake(id);
    }
    first.field_0x8 = g_unk_71727c1088[Tunable().field_0x78];
    first.color = CColor4F(1.0f, 1.0f, 1.0f, 1.0f);
    first.field_0x20 = CColor4F(1.0f, 0.0f, 0.0f, 0.0f);
    first.color.a = 1.0f;
    fn_7100e2c124(0.5f, UnkNoArg(), reinterpret_cast<UnkDropTarget_ovb0208*>(&g_unk_71727c1058), &first.id, 0x1e, 0x1e, 1, 0, 0, true);

    CDrop* billboard = &self->billboard;
    {
        UnkDropTable copy = set;
        Show(billboard, g_unk_71727c1058, self, Tunable().fMissileAmount, copy);
    }
    self->current = billboard;
}
