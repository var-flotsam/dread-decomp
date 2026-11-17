// Dropped-item manager (unit 2562, CLogicDroppedItemManager::CTunableDroppableBillboard): sets up the
// yellow X-Parasite billboard, one entry with its id, colours and counts, and points its three models at
// the tunable's X-Parasite name under each prefix. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkNoArg.h"
#include "base/UnkStrIdPathPair.h"
#include "base/color/CColor4F.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/CLogicDroppedItemManager/CTunableDroppableBillboard.h"
#include "game/UnkTU2561.h"
#include "game/UnkTU2562.h"
#include "game/UnkTU25.h"
#include "game/UnkTU341.h"
#include "types.h"

using base::color::CColor4F;
using base::global::CRntString;
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


struct UnkDroppedItemManager;
struct UnkDroppedItem;
void fn_7100e2fa00(UnkDroppedItemManager* self, UnkDroppedItem* item, void* arg);

// The path prefixes.
__attribute__((visibility("hidden"))) extern CRntString g_unk_71727c13c8[];

// The fields of the tunable these billboards read.
struct UnkBillboardTunable {
    u8 pad_0x0[0x4c];
    u32 field_0x4c;
    u32 field_0x50;
    u32 field_0x54;
    u8 pad_0x58[0x4];
    u32 field_0x5c;
    u8 pad_0x60[0x18];
    u8 field_0x78;
    u8 pad_0x79[0x7];
    f32 field_0x80;
    u8 pad_0x84[0x14];
    CRntString names[10];  // 0x98
};
static_assert(__builtin_offsetof(UnkBillboardTunable, names) == 0x98, "names");

struct UnkBillboard;
struct UnkBillboardVtable {
    void* unk_0x0[7];
    void (*refresh)(UnkBillboard* self);  // 0x38
};

struct UnkBillboard {
    UnkBillboardVtable* vtable;
    u8 pad_0x8[0x8];
    UnkDroppedItemManager* owner;                                                        // 0x10
    void (*callback)(UnkDroppedItemManager* self, UnkDroppedItem* item, void* arg);    // 0x18
    UnkBillboardSet set;                                                                 // 0x20
    CStrId id;                                                                           // 0x460
    u8 pad_0x468[0x20];
    u32 field_0x488;
    u8 pad_0x48c[0x10];
    u8 mode : 2;  // 0x49c
    u8 pad_0x49d[3];
    UnkStrIdPathResource models[3];  // 0x4a0
    bool field_0x4e8;
};
static_assert(__builtin_offsetof(UnkBillboard, models) == 0x4a0, "models");

struct UnkBillboardManager_e2a734 {
    u8 pad_0x0[0x780c];
    u32 field_0x780c;
    u8 pad_0x7810[0x7880 - 0x7810];
    UnkBillboard* current;  // 0x7880
    u8 pad_0x7888[0x9b20 - 0x7888];
    UnkBillboard billboard;  // 0x9b20
};

// The string id and file-path id fn_710001f074 builds in place from a path, destroyed in reverse order.
struct UnkBuiltPathIds_e21b8c {
    struct Ids {
        CStrId id;
        base::UnkStrIdMemberOf<g_unk_7101d47200> path;
    };
    alignas(Ids) u8 storage[sizeof(Ids)];

    explicit UnkBuiltPathIds_e21b8c(const CRntString& text) {
        fn_710001f074(reinterpret_cast<UnkPathIds*>(storage), reinterpret_cast<const UnkInlineString*>(&text));
    }
    const UnkStrIdPathPair& Pair() const { return *reinterpret_cast<const UnkStrIdPathPair*>(storage); }
    ~UnkBuiltPathIds_e21b8c() { reinterpret_cast<Ids*>(storage)->~Ids(); }
};

static inline CRntString operator+(const CRntString& a, const CRntString& b) { return fn_710007eee0(&a, &b); }

// Points a path resource at the ids parsed from a path (the path is built first).
static inline void LoadPath(const CRntString& path, UnkStrIdPathResource* resource) {
    UnkBuiltPathIds_e21b8c ids(path);
    fn_71000267f8(resource, ids.Pair());
}

// The colour assignment, source first (as `dst = src` evaluates it).
static inline void CopyColor(const CColor4F& src, CColor4F& dst) { fn_7100030728(&dst, &src); }

static inline const UnkBillboardTunable& Tunable() {
    return reinterpret_cast<const UnkBillboardTunable&>(g_unk_71727c10c8);
}

static inline void Show(UnkBillboard* billboard, const CStrId& id, UnkDroppedItemManager* owner, u32 count,
                        const UnkBillboardSet& set) {
    billboard->id = id;
    billboard->owner = owner;
    billboard->callback = fn_7100e2fa00;
    billboard->field_0x488 = count;
    fn_7100e2c3f4(reinterpret_cast<UnkDropEntrySet_ovb0208*>(&billboard->set),
                  reinterpret_cast<const UnkDropEntrySet_ovb0208*>(&set));
    g_unk_717269c4d8 = true;
    billboard->vtable->refresh(billboard);
    g_unk_717269c4d8 = false;
}

void fn_7100e2a734(UnkBillboardManager_e2a734* self) {
    UnkBillboardSet set;
    set.field_0x0 = 1;

    UnkBillboardEntry& entry = set.entries[0];
    entry.field_0x30 = 0;
    entry.field_0x38 = self->field_0x780c;
    {
        CStrId id("X-Parasite Yellow", true);
        entry.id0.ReleaseAndTake(id);
    }
    entry.id1 = g_unk_71727c1088[6];
    entry.field_0x3c = 0xffffffff;
    entry.field_0x34 = Tunable().field_0x80;
    CopyColor(CColor4F(1.0f, 1.0f, 0.0f, 1.0f), entry.c0);
    CopyColor(CColor4F(0.0f, 0.0f, 0.0f, 0.0f), entry.c1);
    fn_7100e2c124(0.5f, UnkNoArg(), reinterpret_cast<UnkDropTarget_ovb0208*>(&g_unk_71727c1078), &entry.id0, 0x10, 0x10, 0x10,
                  0, 0, true);

    UnkBillboard* billboard = &self->billboard;
    {
        UnkBillboardSet copy = set;
        Show(billboard, g_unk_71727c1078, reinterpret_cast<UnkDroppedItemManager*>(self), Tunable().field_0x5c, copy);
    }
    billboard->mode = 2;
    LoadPath(g_unk_71727c13c8[0] + Tunable().names[3] + g_unk_71727c13c8[2], &billboard->models[0]);
    LoadPath(g_unk_71727c13c8[0] + Tunable().names[3] + g_unk_71727c13c8[3], &billboard->models[1]);
    LoadPath(g_unk_71727c13c8[0] + Tunable().names[3] + g_unk_71727c13c8[4], &billboard->models[2]);
    billboard->field_0x4e8 = true;
    self->current = billboard;
}
