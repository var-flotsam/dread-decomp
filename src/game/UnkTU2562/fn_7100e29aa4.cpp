// UnkTU2562 (the dropped-item manager's unit): sets up the manager's sixth drop as the power bomb
// item, from a table of 15 entries of which the first two are filled in, and makes it the current one.
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


struct UnkDroppedItems_e29aa4 {
    u8 pad_0x0[0x7808];
    u32 field_0x7808;
    u8 pad_0x780c[0xc];
    u32 field_0x7818;
    u8 pad_0x781c[0x54];
    CDrop* current;  // 0x7870
    u8 pad_0x7878[0x18];
    CDrop drops[6];  // 0x7890
};
static_assert(__builtin_offsetof(UnkDroppedItems_e29aa4, drops[5]) == 0x9140, "layout");

struct UnkDroppableTunable_e29aa4 {
    u8 pad_0x0[0x58];
    f32 fPowerBombBigAmount;  // 0x58
    u8 pad_0x5c[0x1c];
    u8 field_0x78;  // which id of g_unk_71727c1088's table the second entry takes
};

void fn_7100e29aa4(UnkDroppedItems_e29aa4* self) {
    UnkDropTable table;
    const UnkDroppableTunable_e29aa4* tunable = reinterpret_cast<const UnkDroppableTunable_e29aa4*>(&g_unk_71727c10c8);
    const CStrId* ids = g_unk_71727c1088;

    UnkDropTableEntry& second = table.entries[1];
    second.field_0x30 = 0;
    table.count = 2;
    second.field_0x38 = self->field_0x7808;
    {
        CStrId name("PowerBomb0", true);
        second.id.ReleaseAndTake(name);
    }
    second.field_0x8 = ids[tunable->field_0x78];
    {
        CColor4F white(1.0f, 1.0f, 1.0f, 1.0f);
        second.color = white;
    }
    {
        CColor4F none(0.0f, 0.0f, 0.0f, 0.0f);
        second.field_0x20 = none;
    }
    second.field_0x34 = 1.0f;
    fn_7100e2c124(0.5f, UnkNoArg(), reinterpret_cast<UnkDropTarget_ovb0208*>(&g_unk_71727c1070), &second.id, 0x23,
                  0x23, 1, 0, 1, true);

    UnkDropTableEntry& first = table.entries[0];
    first.field_0x30 = 0;
    first.field_0x38 = self->field_0x7818;
    {
        CStrId name("Powerbomb1", true);
        first.id.ReleaseAndTake(name);
    }
    first.field_0x8 = ids[3];
    {
        CColor4F white(1.0f, 1.0f, 1.0f, 1.0f);
        first.color = white;
    }
    {
        CColor4F red(1.0f, 0.0f, 0.0f, 0.0f);
        first.field_0x20 = red;
    }
    first.color.a = 1.0f;
    fn_7100e2c124(0.5f, UnkNoArg(), reinterpret_cast<UnkDropTarget_ovb0208*>(&g_unk_71727c1070), &first.id, 0x23,
                  0x23, 1, 0, 1, true);

    // The drop is filled in from a copy of the table, then started.
    CDrop* drop = &self->drops[5];
    {
        UnkDropTable copy = table;
        f32 amount = tunable->fPowerBombBigAmount;
        drop->item = g_unk_71727c1070;
        drop->owner = self;
        drop->callback = fn_7100e2f99c;
        drop->amount = amount;
        drop->table = copy;
        g_unk_717269c4d8 = true;
        drop->Start();
        g_unk_717269c4d8 = false;
    }
    self->current = drop;
}
