// Dropped-item manager (unit 2562, hint CLogicDroppedItemManager::CTunableDroppableBillboard): sets up
// the "X-Parasite Green" billboard (slot 6): one drop entry, its pickup callback and its three paths.
// Names and layouts are placeholders.
#include "base/UnkPathIdsFromText.h"
#include "base/UnkStrIdPathPair.h"
#include "base/UnkDestructors.h"
#include "base/UnkNoArg.h"
#include "base/color/CColor4F.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/CDropLayout.h"
#include "game/CLogicDroppedItemManager/CTunableDroppableBillboard.h"
#include "game/UnkTU2561.h"
#include "game/UnkTU2562.h"
#include "game/UnkTU25.h"
#include "game/UnkTU341.h"
#include "types.h"

using base::color::CColor4F;
using base::global::CRntString;
using base::global::CStrId;

struct UnkDroppedItemManager;
struct UnkDroppedItem;
struct UnkDropTarget_ovb0208;

// The billboards' path prefix and suffixes.
__attribute__((visibility("hidden"))) extern CRntString g_unk_71727c13c8[];

void fn_7100e2f9a4(UnkDroppedItemManager* self, UnkDroppedItem* item, void* arg);

struct UnkDroppableTunable_e2a034 {
    u8 pad_0x0[0x60];
    u32 field_0x60;
    u8 pad_0x64[0x7c - 0x64];
    u32 field_0x7c;
    u8 pad_0x80[0x128 - 0x80];
    CRntString field_0x128;
};

static inline UnkDroppableTunable_e2a034& Tunable() {
    return reinterpret_cast<UnkDroppableTunable_e2a034&>(g_unk_71727c10c8);
}

struct UnkDropEntry_e2a034 {
    CStrId id0;       // 0x0
    CStrId id1;       // 0x8
    CColor4F color0;  // 0x10
    CColor4F color1;  // 0x20
    u32 field_0x30;
    u32 field_0x34;
    u32 field_0x38;
    s32 field_0x3c;
    bool field_0x40;
};
static_assert(sizeof(UnkDropEntry_e2a034) == 0x48, "entry size");

struct UnkDropEntrySet_e2a034 {
    u32 count;
    UnkDropEntry_e2a034 entries[15];

    UnkDropEntrySet_e2a034();  // fn_71007bf550
    inline UnkDropEntrySet_e2a034(const UnkDropEntrySet_e2a034& other) = default;
    UnkDropEntrySet_e2a034& operator=(const UnkDropEntrySet_e2a034& other) {
        reinterpret_cast<UnkDropTable&>(*this) = reinterpret_cast<const UnkDropTable&>(other);  // fn_7100e2c3f4
        return *this;
    }
};
static_assert(sizeof(UnkDropEntrySet_e2a034) == 0x440, "set size");

struct UnkDropBillboard_e2a034;
struct UnkDropBillboardVtable_e2a034 {
    void* slots[7];
    void (*refresh)(UnkDropBillboard_e2a034* self);  // 0x38
};

struct UnkDropBillboard_e2a034 {
    UnkDropBillboardVtable_e2a034* vtable;  // 0x0
    u8 pad_0x8[8];
    UnkDroppedItemManager* owner;  // 0x10
    void (*on_pickup)(UnkDroppedItemManager* self, UnkDroppedItem* item, void* arg);  // 0x18
    UnkDropEntrySet_e2a034 entries;  // 0x20
    CStrId name;                     // 0x460
    u8 pad_0x468[0x488 - 0x468];
    u32 value;  // 0x488
    u8 pad_0x48c[0x49c - 0x48c];
    u8 flag0 : 1;  // 0x49c
    u8 flag1 : 1;
    u8 pad_0x49d[3];
    UnkStrIdPathResource paths[3];  // 0x4a0
    bool active;                    // 0x4e8
};
static_assert(__builtin_offsetof(UnkDropBillboard_e2a034, paths) == 0x4a0, "paths");
static_assert(__builtin_offsetof(UnkDropBillboard_e2a034, active) == 0x4e8, "active");

struct UnkDropManager_e2a034 {
    u8 pad_0x0[0x780c];
    u32 field_0x780c;
    u8 pad_0x7810[0x7848 - 0x7810];
    UnkDropBillboard_e2a034* billboards[8];  // 0x7848
    u8 pad_0x7888[0x9630 - 0x7888];
    UnkDropBillboard_e2a034 billboard;  // 0x9630
};

static inline CRntString operator+(const CRntString& a, const CRntString& b) { return fn_710007eee0(&a, &b); }

// Points a path resource at the ids parsed from a path.
static inline void SetPath(UnkStrIdPathResource* resource, const CRntString& path) {
    fn_71000267f8(resource, UnkPathIdsFromText(path).ids);
}

static inline void SetColor(CColor4F& color, const CColor4F& value) { color = value; }

static inline void Configure(UnkDropBillboard_e2a034* billboard, UnkDropManager_e2a034* owner,
                             const UnkDropEntrySet_e2a034& entries, u32 value) {
    billboard->name = g_unk_71727c1078;
    billboard->owner = reinterpret_cast<UnkDroppedItemManager*>(owner);
    billboard->on_pickup = &fn_7100e2f9a4;
    billboard->value = value;
    billboard->entries = entries;
    g_unk_717269c4d8 = true;
    billboard->vtable->refresh(billboard);
    g_unk_717269c4d8 = false;
}

void fn_7100e2a034(UnkDropManager_e2a034* self) {
    UnkDropEntrySet_e2a034 entries;
    UnkDropEntry_e2a034& entry = entries.entries[0];
    entry.field_0x30 = 0;
    entries.count = 1;
    entry.field_0x38 = self->field_0x780c;
    entry.id0 = CStrId("X-Parasite Green", true);
    entry.id1 = g_unk_71727c1088[6];
    entry.field_0x3c = -1;
    entry.field_0x34 = Tunable().field_0x7c;
    SetColor(entry.color0, CColor4F(0.1f, 1.0f, 0.0f, 1.0f));
    SetColor(entry.color1, CColor4F(0.0f, 0.0f, 0.0f, 0.0f));
    fn_7100e2c124(0.5f, UnkNoArg(), reinterpret_cast<UnkDropTarget_ovb0208*>(&g_unk_71727c1078), &entry.id0, 0x10, 0x10, 0x10, 0,
                  0, true);

    UnkDropBillboard_e2a034* billboard = &self->billboard;
    Configure(billboard, self, UnkDropEntrySet_e2a034(entries), Tunable().field_0x60);
    billboard->flag0 = false;
    billboard->flag1 = true;
    const CRntString& name = Tunable().field_0x128;
    SetPath(&billboard->paths[0], g_unk_71727c13c8[0] + name + g_unk_71727c13c8[2]);
    SetPath(&billboard->paths[1], g_unk_71727c13c8[0] + name + g_unk_71727c13c8[3]);
    SetPath(&billboard->paths[2], g_unk_71727c13c8[0] + name + g_unk_71727c13c8[4]);
    billboard->active = true;
    self->billboards[6] = billboard;
}
