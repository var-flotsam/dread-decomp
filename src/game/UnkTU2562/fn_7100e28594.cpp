// UnkTU2562 (the dropped-item manager's unit): sets up the manager's second drop as the "LifeBig2"
// item, from a table of 15 entries of which only the first is filled in, and makes it the current one.
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

extern const CColor4F g_unk_710164be10;

struct UnkDroppedItems_e28594 {
    u8 pad_0x0[0x7810];
    u32 field_0x7810;
    u8 pad_0x7814[0x3c];
    CDrop* current;  // 0x7850
    u8 pad_0x7858[0x38];
    CDrop drops[2];  // 0x7890
};
static_assert(__builtin_offsetof(UnkDroppedItems_e28594, drops[1]) == 0x7d80, "layout");

struct UnkDroppableTunable_e28594 {
    u8 pad_0x0[0x48];
    f32 fLifeBigAmount;  // 0x48
};

void fn_7100e28594(UnkDroppedItems_e28594* self) {
    UnkDropTable table;

    UnkDropTableEntry& entry = table.entries[0];
    entry.field_0x30 = 0;
    table.count = 1;
    entry.field_0x38 = self->field_0x7810;
    {
        CStrId name("LifeBig2", true);
        entry.id.ReleaseAndTake(name);
    }
    entry.field_0x8 = g_unk_71727c1088[0];
    entry.color = g_unk_710164be10;
    {
        CColor4F red(1.0f, 0.0f, 0.0f, 0.0f);
        entry.field_0x20 = red;
    }
    entry.field_0x3c = -1;
    entry.color.a = 1.0f;
    fn_7100e2c124(1.0f, UnkNoArg(), reinterpret_cast<UnkDropTarget_ovb0208*>(&g_unk_71727c1050), &entry.id, 0x28,
                  0x28, 0x40, 0, 0, true);

    // The drop is filled in from a copy of the table, then started.
    CDrop* drop = &self->drops[1];
    {
        UnkDropTable copy = table;
        f32 amount = reinterpret_cast<const UnkDroppableTunable_e28594*>(&g_unk_71727c10c8)->fLifeBigAmount;
        drop->item = g_unk_71727c1050;
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
