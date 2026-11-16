// UnkTU2562 (the dropped-item manager's unit): sets up the manager's first drop as the "Life2" item,
// from a table of 15 entries of which only the first is filled in, and makes it the current drop.
// Names and layouts are placeholders.
#include "base/UnkNoArg.h"
#include "base/color/CColor4F.h"
#include "base/global/CStrId.h"
#include "game/CLogicDroppedItemManager/CTunableDroppableBillboard.h"
#include "game/UnkTU341.h"
#include "types.h"

using base::color::CColor4F;
using base::global::CStrId;

extern CStrId g_unk_71727c1048;
extern CStrId g_unk_71727c1088;
extern const CColor4F g_unk_710164be00;

// One entry of the drop's table (0x48 bytes).
struct UnkLifeEntry_e2813c {
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
static_assert(sizeof(UnkLifeEntry_e2813c) == 0x48, "layout");

// The table a drop is set up from: a count and 15 entries (built by fn_71007bf550).
struct UnkLifeTable_e2813c {
    u32 count;
    UnkLifeEntry_e2813c entries[15];
};
static_assert(sizeof(UnkLifeTable_e2813c) == 0x440, "layout");

struct UnkTable_71007bf550;
void fn_71007bf550(UnkTable_71007bf550* self);

// The drop (0x4f0 bytes), as this function sees it.
struct UnkDrop_e2813c {
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
};
static_assert(__builtin_offsetof(UnkDrop_e2813c, name) == 0x460, "layout");
static_assert(__builtin_offsetof(UnkDrop_e2813c, amount) == 0x488, "layout");

struct UnkDroppedItems_e2813c {
    u8 pad_0x0[0x7810];
    u32 field_0x7810;
    u8 pad_0x7814[0x34];
    UnkDrop_e2813c* current;  // 0x7848
    u8 pad_0x7850[0x40];
    UnkDrop_e2813c drop;  // 0x7890
};
static_assert(__builtin_offsetof(UnkDroppedItems_e2813c, drop) == 0x7890, "layout");

struct UnkDroppableTunable_e2813c {
    u8 pad_0x0[0x44];
    u32 field_0x44;
};

void fn_7100e2c124(UnkNoArg, const CStrId* name, CStrId* id, u32 a, u32 b, u32 c, u32 d, u32 e, f32 scale,
                   bool flag);
void fn_7100e2c3f4(void* entries, const UnkLifeTable_e2813c* table);
void fn_7100e2f98c(void* owner);

// Fills the drop in from the table and lets it start.
inline void SetupDrop(UnkDroppedItems_e2813c* self, UnkLifeTable_e2813c table) {
    UnkDrop_e2813c* drop = &self->drop;
    u32 amount = reinterpret_cast<const UnkDroppableTunable_e2813c*>(&g_unk_71727c10c8)->field_0x44;
    drop->name = g_unk_71727c1048;
    drop->owner = self;
    drop->callback = fn_7100e2f98c;
    drop->amount = amount;
    fn_7100e2c3f4(drop->entries, &table);
    g_unk_717269c4d8 = true;
    drop->vf_0x38();
    g_unk_717269c4d8 = false;
}

void fn_7100e2813c(UnkDroppedItems_e2813c* self) {
    alignas(8) u8 storage[sizeof(UnkLifeTable_e2813c)];
    fn_71007bf550(reinterpret_cast<UnkTable_71007bf550*>(storage));
    UnkLifeTable_e2813c& table = *reinterpret_cast<UnkLifeTable_e2813c*>(storage);

    UnkLifeEntry_e2813c& entry = table.entries[0];
    entry.field_0x30 = 0;
    table.count = 1;
    entry.field_0x38 = self->field_0x7810;
    {
        CStrId name("Life2", true);
        entry.id = static_cast<CStrId&&>(name);
    }
    entry.field_0x8 = g_unk_71727c1088;
    fn_7100030728(&entry.color, &g_unk_710164be00);
    entry.color.a = 1.0f;
    {
        CColor4F red(1.0f, 0.0f, 0.0f, 0.0f);
        fn_7100030728(&entry.field_0x20, &red);
    }
    entry.field_0x3c = -1;
    fn_7100e2c124(UnkNoArg(), &g_unk_71727c1048, &entry.id, 0x19, 0x19, 0x40, 0, 0, 1.0f, true);

    SetupDrop(self, table);
    self->current = &self->drop;
    table.~UnkLifeTable_e2813c();
}
