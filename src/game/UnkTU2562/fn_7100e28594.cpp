// UnkTU2562 (the dropped-item manager's unit): sets up the manager's second drop as the "LifeBig2"
// item, from a table of 15 entries of which only the first is filled in, and makes it the current one.
// Names and layouts are placeholders.
#include "base/UnkNoArg.h"
#include "base/color/CColor4F.h"
#include "base/global/CStrId.h"
#include "game/CDrop.h"
#include "game/CLogicDroppedItemManager/CTunableDroppableBillboard.h"
#include "game/UnkTU2561.h"
#include "game/UnkTU2562.h"
#include "game/UnkTU341.h"
#include "types.h"

using base::color::CColor4F;
using base::global::CStrId;

extern const CColor4F g_unk_710164be10;

// One entry of the drop's table (0x48 bytes).
struct UnkLifeEntry_e28594 {
    CStrId id;
    CStrId field_0x8;
    CColor4F color;
    CColor4F field_0x20;
    u32 field_0x30;
    f32 field_0x34;
    s32 field_0x38;
    s32 field_0x3c;
    bool field_0x40;
};
static_assert(sizeof(UnkLifeEntry_e28594) == 0x48, "layout");

// The table a drop is set up from: a count and 15 entries (built by fn_71007bf550).
struct UnkLifeTable_e28594 {
    u32 count;
    UnkLifeEntry_e28594 entries[15];
};
static_assert(sizeof(UnkLifeTable_e28594) == 0x440, "layout");


// The drop (0x4f0 bytes), as this function sees it.
struct UnkDrop_e28594 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();

    u64 field_0x8;
    void* owner;              // 0x10
    void (*callback)(void*);  // 0x18
    u8 entries[0x438];        // 0x20
    u8 pad_0x458[8];
    CStrId name;  // 0x460
    u8 pad_0x468[0x20];
    u32 amount;  // 0x488
    u8 pad_0x48c[0x64];
};
static_assert(__builtin_offsetof(UnkDrop_e28594, name) == 0x460, "layout");
static_assert(__builtin_offsetof(UnkDrop_e28594, amount) == 0x488, "layout");
static_assert(sizeof(UnkDrop_e28594) == 0x4f0, "layout");

struct UnkDroppedItems_e28594 {
    u8 pad_0x0[0x7810];
    u32 field_0x7810;
    u8 pad_0x7814[0x3c];
    UnkDrop_e28594* current;  // 0x7850
    u8 pad_0x7858[0x38];
    UnkDrop_e28594 drops[2];  // 0x7890
};
static_assert(__builtin_offsetof(UnkDroppedItems_e28594, drops[1]) == 0x7d80, "layout");

struct UnkDroppableTunable_e28594 {
    u8 pad_0x0[0x48];
    u32 field_0x48;
};

void fn_7100e2f98c(void* owner);

void fn_7100e28594(UnkDroppedItems_e28594* self) {
    alignas(8) u8 storage[sizeof(UnkLifeTable_e28594)];
    fn_71007bf550(reinterpret_cast<UnkTable_71007bf550*>(storage));
    UnkLifeTable_e28594& table = *reinterpret_cast<UnkLifeTable_e28594*>(storage);

    UnkLifeEntry_e28594& entry = table.entries[0];
    entry.field_0x30 = 0;
    table.count = 1;
    entry.field_0x38 = self->field_0x7810;
    {
        CStrId name("LifeBig2", true);
        entry.id.ReleaseAndTake(name);
    }
    entry.field_0x8 = g_unk_71727c1088[0];
    fn_7100030728(&entry.color, &g_unk_710164be10);
    {
        CColor4F red(1.0f, 0.0f, 0.0f, 0.0f);
        fn_7100030728(&entry.field_0x20, &red);
    }
    entry.field_0x3c = -1;
    entry.color.a = 1.0f;
    fn_7100e2c124(1.0f, UnkNoArg(), reinterpret_cast<UnkDropTarget_ovb0208*>(&g_unk_71727c1050), &entry.id, 0x28,
                  0x28, 0x40, 0, 0, true);

    // The drop is filled in from a copy of the table, then started.
    UnkDrop_e28594* drop = &self->drops[1];
    {
        UnkLifeTable_e28594 copy = table;
        u32 amount = reinterpret_cast<const UnkDroppableTunable_e28594*>(&g_unk_71727c10c8)->field_0x48;
        drop->name = g_unk_71727c1050;
        drop->owner = self;
        drop->callback = fn_7100e2f98c;
        drop->amount = amount;
        fn_7100e2c3f4(reinterpret_cast<UnkDropEntrySet_ovb0208*>(drop->entries),
                      reinterpret_cast<const UnkDropEntrySet_ovb0208*>(&copy));
        g_unk_717269c4d8 = true;
        drop->vf_0x38();
        g_unk_717269c4d8 = false;
    }
    self->current = drop;
    table.~UnkLifeTable_e28594();
}
