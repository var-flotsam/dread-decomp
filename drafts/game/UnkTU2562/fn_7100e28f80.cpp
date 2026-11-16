// Dropped-item manager (unit 2562, CLogicDroppedItemManager::CTunableDroppableBillboard): sets up the
// missile billboard, two entries with their ids, colours and counts, and hands them to the billboard.
// Names and layouts are placeholders.
#include "base/color/CColor4F.h"
#include "base/global/CStrId.h"
#include "game/CLogicDroppedItemManager/CTunableDroppableBillboard.h"
#include "game/UnkTU341.h"
#include "types.h"

using base::color::CColor4F;
using base::global::CStrId;

// One billboard entry (0x48 bytes): two ids, two colours and its counts.
struct UnkBillboardEntry {
    CStrId id0;   // 0x0
    CStrId id1;   // 0x8
    CColor4F c0;  // 0x10
    CColor4F c1;  // 0x20
    u32 field_0x30;
    f32 field_0x34;
    u32 field_0x38;
    u32 field_0x3c;
    bool field_0x40;
};
static_assert(sizeof(UnkBillboardEntry) == 0x48, "entry size");

// The billboard's entry table; its constructor is out of line.
struct UnkBillboardSet {
    u32 field_0x0;
    UnkBillboardEntry entries[15];  // 0x8

    UnkBillboardSet() asm("fn_71007bf550");
};
static_assert(sizeof(UnkBillboardSet) == 0x440, "set size");

struct UnkDropEntrySet_ovb0208;
UnkDropEntrySet_ovb0208* fn_7100e2c3f4(UnkDropEntrySet_ovb0208* self, const UnkDropEntrySet_ovb0208* other);

struct UnkDropTarget_ovb0208;
void fn_7100e2c124(f32 value, void* self, UnkDropTarget_ovb0208* target, const CStrId* id, u32 a, u32 b, u32 c, u32 d, u32 e,
                   bool flag);

extern CStrId g_unk_71727c1060;
extern CStrId g_unk_71727c1088[];
void fn_7100e2f994(void* owner);

// The fields of the tunable these billboards read.
struct UnkBillboardTunable {
    u8 pad_0x0[0x50];
    u32 field_0x50;
    u32 field_0x54;
    u8 pad_0x58[0x20];
    u8 field_0x78;
};

struct UnkBillboard;
struct UnkBillboardVtable {
    void* unk_0x0[7];
    void (*refresh)(UnkBillboard* self);  // 0x38
};

struct UnkBillboard {
    UnkBillboardVtable* vtable;
    u8 pad_0x8[0x8];
    void* owner;              // 0x10
    void (*callback)(void*);  // 0x18
    UnkBillboardSet set;      // 0x20
    CStrId id;                // 0x460
    u8 pad_0x468[0x20];
    u32 field_0x488;
};

struct UnkBillboardManager_e28f80 {
    u8 pad_0x0[0x7808];
    u32 field_0x7808;
    u8 pad_0x780c[0xc];
    u32 field_0x7818;
    u8 pad_0x781c[0x7860 - 0x781c];
    UnkBillboard* current;  // 0x7860
    u8 pad_0x7868[0x8760 - 0x7868];
    UnkBillboard billboard;  // 0x8760
};

// The colour assignment, source first (as `dst = src` evaluates it).
static inline void CopyColor(const CColor4F& src, CColor4F& dst) { fn_7100030728(&dst, &src); }

static inline const UnkBillboardTunable& Tunable() {
    return reinterpret_cast<const UnkBillboardTunable&>(g_unk_71727c10c8);
}

static inline void Show(UnkBillboard* billboard, const CStrId& id, void* owner, u32 count, const UnkBillboardSet& set) {
    billboard->id = id;
    billboard->owner = owner;
    billboard->callback = fn_7100e2f994;
    billboard->field_0x488 = count;
    fn_7100e2c3f4(reinterpret_cast<UnkDropEntrySet_ovb0208*>(&billboard->set),
                  reinterpret_cast<const UnkDropEntrySet_ovb0208*>(&set));
    g_unk_717269c4d8 = true;
    billboard->vtable->refresh(billboard);
    g_unk_717269c4d8 = false;
}

void fn_7100e28f80(UnkBillboardManager_e28f80* self) {
    UnkBillboardSet set;
    set.field_0x0 = 2;

    UnkBillboardEntry& second = set.entries[1];
    second.field_0x30 = 0;
    second.field_0x38 = self->field_0x7808;
    {
        CStrId id("Missile0", true);
        second.id0.ReleaseAndTake(id);
    }
    second.id1 = g_unk_71727c1088[Tunable().field_0x78];
    CopyColor(CColor4F(1.0f, 1.0f, 1.0f, 1.0f), second.c0);
    CopyColor(CColor4F(0.0f, 0.0f, 0.0f, 0.0f), second.c1);
    second.field_0x34 = 1.0f;
    fn_7100e2c124(0.5f, nullptr, reinterpret_cast<UnkDropTarget_ovb0208*>(&g_unk_71727c1060), &second.id0, 0x1e, 0x1e, 1, 0, 0, true);

    UnkBillboardEntry& first = set.entries[0];
    first.field_0x30 = 0;
    first.field_0x38 = self->field_0x7818;
    {
        CStrId id("Missile1", true);
        first.id0.ReleaseAndTake(id);
    }
    first.id1 = g_unk_71727c1088[Tunable().field_0x78];
    CopyColor(CColor4F(1.0f, 1.0f, 1.0f, 1.0f), first.c0);
    CopyColor(CColor4F(1.0f, 0.0f, 0.0f, 0.0f), first.c1);
    first.c0.a = 1.0f;
    fn_7100e2c124(0.5f, nullptr, reinterpret_cast<UnkDropTarget_ovb0208*>(&g_unk_71727c1060), &first.id0, 0x1e, 0x1e, 1, 0, 0, true);

    UnkBillboard* billboard = &self->billboard;
    {
        UnkBillboardSet copy = set;
        Show(billboard, g_unk_71727c1060, self, Tunable().field_0x50, copy);
    }
    self->current = billboard;
}
