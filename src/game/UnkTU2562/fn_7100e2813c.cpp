// UnkTU2562 (the dropped-item manager's unit): sets up the manager's first drop as the "Life2"
// item, from a table of 15 entries of which only the first is filled in, and makes it the current one.
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

extern const CColor4F g_unk_710164be00;

struct UnkDroppedItems_e2813c {
    u8 pad_0x0[0x7810];
    u32 field_0x7810;
    u8 pad_0x7814[0x34];
    CDrop* current;  // 0x7848
    u8 pad_0x7850[0x40];
    CDrop drops[1];  // 0x7890
};
static_assert(__builtin_offsetof(UnkDroppedItems_e2813c, drops[0]) == 0x7890, "layout");

void fn_7100e2813c(UnkDroppedItems_e2813c* self) {
    UnkDropTable table;

    UnkDropTableEntry& entry = table.entries[0];
    entry.field_0x30 = 0;
    table.count = 1;
    entry.field_0x38 = self->field_0x7810;
    {
        CStrId name("Life2", true);
        entry.id.ReleaseAndTake(name);
    }
    entry.field_0x8 = g_unk_71727c1088[0];
    entry.color = g_unk_710164be00;
    entry.color.a = 1.0f;
    {
        CColor4F red(1.0f, 0.0f, 0.0f, 0.0f);
        entry.field_0x20 = red;
    }
    entry.field_0x3c = -1;
    fn_7100e2c124(1.0f, UnkNoArg(), reinterpret_cast<UnkDropTarget_ovb0208*>(&g_unk_71727c1048), &entry.id, 0x19,
                  0x19, 0x40, 0, 0, true);

    // The drop is filled in from a copy of the table, then started.
    CDrop* drop = &self->drops[0];
    {
        UnkDropTable copy = table;
        f32 amount = g_unk_71727c10c8.fLifeAmount;
        drop->item = g_unk_71727c1048;
        drop->owner = self;
        drop->callback = fn_7100e2f98c;
        drop->amount = amount;
        drop->table = copy;
        g_unk_717269c4d8 = true;
        drop->Start();
        g_unk_717269c4d8 = false;
    }
    self->current = drop;
}
