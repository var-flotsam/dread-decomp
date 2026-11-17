// Dropped-item manager (unit 2562, CLogicDroppedItemManager::CTunableDroppableBillboard): sets up the
// missile billboard, two entries with their ids, colours and counts, and hands them to the billboard.
// Names and layouts are placeholders.
#include "base/UnkNoArg.h"
#include "base/color/CColor4F.h"
#include "base/global/CStrId.h"
#include "game/CDropLayout.h"
#include "game/CLogicDroppedItemManager/CTunableDroppableBillboard.h"
#include "game/UnkDroppableTunable.h"
#include "game/UnkTU2561.h"
#include "game/UnkTU2562.h"
#include "game/UnkTU341.h"
#include "types.h"

using base::color::CColor4F;
using base::global::CStrId;


struct UnkBillboardManager_e28f80 {
    u8 pad_0x0[0x7808];
    u32 field_0x7808;
    u8 pad_0x780c[0xc];
    u32 field_0x7818;
    u8 pad_0x781c[0x7860 - 0x781c];
    CDrop* current;  // 0x7860
    u8 pad_0x7868[0x8760 - 0x7868];
    CDrop billboard;  // 0x8760
};

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

void fn_7100e28f80(UnkBillboardManager_e28f80* self) {
    UnkDropTable set;
    set.count = 2;

    UnkDropTableEntry& second = set.entries[1];
    second.field_0x30 = 0;
    second.field_0x38 = self->field_0x7808;
    {
        CStrId id("Missile0", true);
        second.id.ReleaseAndTake(id);
    }
    second.field_0x8 = g_unk_71727c1088[g_unk_71727c10c8.bDropsDoRelaxMovement];
    second.color = CColor4F(1.0f, 1.0f, 1.0f, 1.0f);
    second.field_0x20 = CColor4F(0.0f, 0.0f, 0.0f, 0.0f);
    second.field_0x34 = 1.0f;
    fn_7100e2c124(0.5f, UnkNoArg(), reinterpret_cast<UnkDropTarget_ovb0208*>(&g_unk_71727c1060), &second.id, 0x1e, 0x1e, 1, 0, 0, true);

    UnkDropTableEntry& first = set.entries[0];
    first.field_0x30 = 0;
    first.field_0x38 = self->field_0x7818;
    {
        CStrId id("Missile1", true);
        first.id.ReleaseAndTake(id);
    }
    first.field_0x8 = g_unk_71727c1088[g_unk_71727c10c8.bDropsDoRelaxMovement];
    first.color = CColor4F(1.0f, 1.0f, 1.0f, 1.0f);
    first.field_0x20 = CColor4F(1.0f, 0.0f, 0.0f, 0.0f);
    first.color.a = 1.0f;
    fn_7100e2c124(0.5f, UnkNoArg(), reinterpret_cast<UnkDropTarget_ovb0208*>(&g_unk_71727c1060), &first.id, 0x1e, 0x1e, 1, 0, 0, true);

    CDrop* billboard = &self->billboard;
    {
        UnkDropTable copy = set;
        Show(billboard, g_unk_71727c1060, self, g_unk_71727c10c8.fMissileBigAmount, copy);
    }
    self->current = billboard;
}
