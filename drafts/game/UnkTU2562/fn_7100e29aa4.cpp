// UnkTU2562 (the dropped-item manager's unit): sets up the manager's sixth drop as the power bomb
// item, from a table of 15 entries of which the first two are filled in, and makes it the current one.
// Names and layouts are placeholders.
#include "base/UnkNoArg.h"
#include "base/color/CColor4F.h"
#include "base/global/CStrId.h"
#include "game/CLogicDroppedItemManager/CTunableDroppableBillboard.h"
#include "game/UnkTU341.h"
#include "types.h"

using base::color::CColor4F;
using base::global::CStrId;

extern CStrId g_unk_71727c1070;
extern CStrId g_unk_71727c1088;  // the first of a table of ids

// One entry of the drop's table (0x48 bytes).
struct UnkLifeEntry_e29aa4 {
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
static_assert(sizeof(UnkLifeEntry_e29aa4) == 0x48, "layout");

// The table a drop is set up from: a count and 15 entries (built by fn_71007bf550).
struct UnkLifeTable_e29aa4 {
    u32 count;
    UnkLifeEntry_e29aa4 entries[15];
};
static_assert(sizeof(UnkLifeTable_e29aa4) == 0x440, "layout");

struct UnkTable_71007bf550;
void fn_71007bf550(UnkTable_71007bf550* self);

// The drop (0x4f0 bytes), as this function sees it.
struct UnkDrop_e29aa4 {
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
static_assert(__builtin_offsetof(UnkDrop_e29aa4, name) == 0x460, "layout");
static_assert(__builtin_offsetof(UnkDrop_e29aa4, amount) == 0x488, "layout");
static_assert(sizeof(UnkDrop_e29aa4) == 0x4f0, "layout");

struct UnkDroppedItems_e29aa4 {
    u8 pad_0x0[0x7808];
    u32 field_0x7808;
    u8 pad_0x780c[0xc];
    u32 field_0x7818;
    u8 pad_0x781c[0x54];
    UnkDrop_e29aa4* current;  // 0x7870
    u8 pad_0x7878[0x18];
    UnkDrop_e29aa4 drops[6];  // 0x7890
};
static_assert(__builtin_offsetof(UnkDroppedItems_e29aa4, drops[5]) == 0x9140, "layout");

struct UnkDroppableTunable_e29aa4 {
    u8 pad_0x0[0x58];
    u32 field_0x58;
    u8 pad_0x5c[0x1c];
    u8 field_0x78;  // which id of g_unk_71727c1088's table the second entry takes
};

struct UnkDropTarget_ovb0208;
struct UnkDropEntrySet_ovb0208;
void fn_7100e2c124(f32 value, UnkNoArg, UnkDropTarget_ovb0208* target, const CStrId* id, u32 a, u32 b, u32 c, u32 d,
                   u32 e, bool flag);
UnkDropEntrySet_ovb0208* fn_7100e2c3f4(UnkDropEntrySet_ovb0208* self, const UnkDropEntrySet_ovb0208* other);
void fn_7100e2f99c(void* owner);

void fn_7100e29aa4(UnkDroppedItems_e29aa4* self) {
    alignas(8) u8 storage[sizeof(UnkLifeTable_e29aa4)];
    fn_71007bf550(reinterpret_cast<UnkTable_71007bf550*>(storage));
    UnkLifeTable_e29aa4& table = *reinterpret_cast<UnkLifeTable_e29aa4*>(storage);
    const UnkDroppableTunable_e29aa4* tunable = reinterpret_cast<const UnkDroppableTunable_e29aa4*>(&g_unk_71727c10c8);
    const CStrId* ids = &g_unk_71727c1088;

    UnkLifeEntry_e29aa4& second = table.entries[1];
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
        fn_7100030728(&second.color, &white);
    }
    {
        CColor4F none(0.0f, 0.0f, 0.0f, 0.0f);
        fn_7100030728(&second.field_0x20, &none);
    }
    second.field_0x34 = 1.0f;
    fn_7100e2c124(0.5f, UnkNoArg(), reinterpret_cast<UnkDropTarget_ovb0208*>(&g_unk_71727c1070), &second.id, 0x23,
                  0x23, 1, 0, 1, true);

    UnkLifeEntry_e29aa4& first = table.entries[0];
    first.field_0x30 = 0;
    first.field_0x38 = self->field_0x7818;
    {
        CStrId name("Powerbomb1", true);
        first.id.ReleaseAndTake(name);
    }
    first.field_0x8 = ids[3];
    {
        CColor4F white(1.0f, 1.0f, 1.0f, 1.0f);
        fn_7100030728(&first.color, &white);
    }
    {
        CColor4F red(1.0f, 0.0f, 0.0f, 0.0f);
        fn_7100030728(&first.field_0x20, &red);
    }
    first.color.a = 1.0f;
    fn_7100e2c124(0.5f, UnkNoArg(), reinterpret_cast<UnkDropTarget_ovb0208*>(&g_unk_71727c1070), &first.id, 0x23,
                  0x23, 1, 0, 1, true);

    // The drop is filled in from a copy of the table, then started.
    UnkDrop_e29aa4* drop = &self->drops[5];
    {
        UnkLifeTable_e29aa4 copy = table;
        u32 amount = tunable->field_0x58;
        drop->name = g_unk_71727c1070;
        drop->owner = self;
        drop->callback = fn_7100e2f99c;
        drop->amount = amount;
        fn_7100e2c3f4(reinterpret_cast<UnkDropEntrySet_ovb0208*>(drop->entries),
                      reinterpret_cast<const UnkDropEntrySet_ovb0208*>(&copy));
        g_unk_717269c4d8 = true;
        drop->vf_0x38();
        g_unk_717269c4d8 = false;
    }
    self->current = drop;
    table.~UnkLifeTable_e29aa4();
}
