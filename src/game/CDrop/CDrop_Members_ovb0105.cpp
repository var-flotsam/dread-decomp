// CDrop's unit (tools/tu_map.py): the drop's constructor and destructor bodies (15 entries of two ids at
// +0x20, two ids, three id/path records), its amount (scaled per item in game mode 3 and rounded away
// from zero) and the object it is linked to, through its holder or else the game manager's id lookup.
// Names and layouts are placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/CAISmartObjectComponent.h"
#include "game/CDrop.h"
#include "game/CGameObject.h"
#include "game/CLifeComponent.h"
#include "game/CLogicDroppedItemManager/CTunableDroppableBillboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU32.h"
#include "game/UnkTU365.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkIdPair;
struct UnkCameraCtrl_ovb0049;
extern char g_unk_7101a71568[];  // CDrop's vtable

// An id released in two steps (CStrId::Reset) when its owner is destroyed.
struct UnkDropId_b0105 {
    union {
        CStrId id;
    };
    ~UnkDropId_b0105() { id.Reset(); }
};

// One of the drop's 15 entries (0x48 bytes).
struct UnkDropEntry_b0105 {
    u64 field_0x0;
    UnkDropId_b0105 field_0x8;
    UnkDropId_b0105 field_0x10;
    u8 pad_0x18[0x30];
};
static_assert(sizeof(UnkDropEntry_b0105) == 0x48, "UnkDropEntry_b0105 layout");

// An id/path resource record (0x18 bytes), released with fn_7100026f70.
struct UnkDropResource_b0105 {
    CStrId id;
    CFilePathStrId path;
    u64 field_0x10;
    UnkDropResource_b0105() : id(g_unk_7101d470e0), path(g_unk_7101d470e8), field_0x10(0) {}
};

struct UnkDrop_b0105 {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
    UnkDropEntry_b0105 entries[15];  // 0x20
    u8 pad_0x458[8];
    UnkDropId_b0105 field_0x460;
    u32 field_0x468;
    UnkDropId_b0105 field_0x470;
    u64 field_0x478;
    u64 field_0x480;
    u32 field_0x488;
    u32 field_0x48c;
    u32 field_0x490;
    u32 field_0x494;
    u32 field_0x498;
    bool field_0x49c;
    UnkDropResource_b0105 field_0x4a0;
    UnkDropResource_b0105 field_0x4b8;
    UnkDropResource_b0105 field_0x4d0;
    bool field_0x4e8;
};
static_assert(__builtin_offsetof(UnkDrop_b0105, field_0x4d0) == 0x4d0, "UnkDrop_b0105 layout");

void fn_71007bf550(UnkDropEntry_b0105* entries);  // constructs the entries

void fn_71007bf3f0(UnkDrop_b0105* self) {
    fn_7101043904(self);
    self->vtable = reinterpret_cast<void**>(g_unk_7101a71568) + 2;
    self->field_0x10 = 0;
    self->field_0x18 = 0;
    fn_71007bf550(self->entries);
    new (&self->field_0x460.id) CStrId(g_unk_710162e2ed, true);
    self->field_0x468 = 0;
    new (&self->field_0x470.id) CStrId(g_unk_710162e2ed, true);
    self->field_0x480 = 0;
    self->field_0x478 = 0;
    self->field_0x48c = 0;
    self->field_0x490 = 0;
    self->field_0x494 = 0;
    self->field_0x498 = 0;
    self->field_0x49c = true;
    new (&self->field_0x4a0) UnkDropResource_b0105();
    new (&self->field_0x4b8) UnkDropResource_b0105();
    new (&self->field_0x4d0) UnkDropResource_b0105();
    self->field_0x4e8 = false;
}

void fn_71007bfa70(UnkDrop_b0105* self) {
    self->vtable = reinterpret_cast<void**>(g_unk_7101a71568) + 2;
    fn_7100026f70(reinterpret_cast<UnkIdPair*>(&self->field_0x4d0));
    fn_7100026f70(reinterpret_cast<UnkIdPair*>(&self->field_0x4b8));
    fn_7100026f70(reinterpret_cast<UnkIdPair*>(&self->field_0x4a0));
    self->field_0x470.~UnkDropId_b0105();
    self->field_0x460.~UnkDropId_b0105();
    for (u32 i = 15; i-- > 0;)
        self->entries[i].~UnkDropEntry_b0105();
    fn_7101043934(reinterpret_cast<UnkCameraCtrl_ovb0049*>(self));
}

struct UnkDroppableTunable;

// The drop amount multipliers of the tunable g_unk_7172a0a0a8, by item id.
struct UnkDropScaleTunable_b0105 {
    u8 pad_0x0[0x90];
    f32 field_0x90;
    f32 field_0x94;
    f32 field_0x98;
    f32 field_0x9c;
    f32 field_0xa0;
    f32 field_0xa4;
    f32 field_0xa8;
    f32 field_0xac;
};
extern UnkDropScaleTunable_b0105 g_unk_7172a0a0a8;

extern CStrId g_unk_71727c1048;
extern CStrId g_unk_71727c1050;
extern CStrId g_unk_71727c1058;
extern CStrId g_unk_71727c1060;
extern CStrId g_unk_71727c1068;
extern CStrId g_unk_71727c1070;
extern CStrId g_unk_71727c1078;

struct UnkDropAmount_b0105 {
    u8 pad_0x0[0x460];
    CStrId item;  // 0x460
    u8 pad_0x468[0x20];
    f32 amount;  // 0x488
};

static inline f32 ScaledAmount(f32 amount, f32 scale) {
    f32 v = amount * scale;
    return v > 0.0f ? __builtin_ceilf(v) : __builtin_floorf(v);
}

// The drop's amount; in game mode 3 scaled by the item's multiplier and rounded away from zero.
f32 fn_71007bffe4(UnkDropAmount_b0105* self) {
    if (fn_7100dffc58(g_unk_7101d47310) == 3) {
        base::global::UnkStringData* item = self->item.data;
        if (item == g_unk_71727c1048.data)
            return ScaledAmount(self->amount, g_unk_7172a0a0a8.field_0x90);
        if (item == g_unk_71727c1050.data)
            return ScaledAmount(self->amount, g_unk_7172a0a0a8.field_0x94);
        if (item == g_unk_71727c1058.data)
            return ScaledAmount(self->amount, g_unk_7172a0a0a8.field_0x98);
        if (item == g_unk_71727c1060.data)
            return ScaledAmount(self->amount, g_unk_7172a0a0a8.field_0x9c);
        if (item == g_unk_71727c1068.data)
            return ScaledAmount(self->amount, g_unk_7172a0a0a8.field_0xa0);
        if (item == g_unk_71727c1070.data)
            return ScaledAmount(self->amount, g_unk_7172a0a0a8.field_0xa4);
        if (item == g_unk_71727c1078.data) {
            f32 amount = self->amount;
            f32 limit = *reinterpret_cast<f32*>(reinterpret_cast<char*>(&g_unk_71727c10c8) + 0x60);
            if (amount + 0.001f >= limit && amount - 0.001f <= limit)
                return ScaledAmount(amount, g_unk_7172a0a0a8.field_0xac);
            return ScaledAmount(amount, g_unk_7172a0a0a8.field_0xa8);
        }
    }
    return self->amount;
}

struct UnkHolder;
struct UnkSpawnedActor_ovb0065;
struct UnkSpawnRegistered_ovb0065;
struct UnkBreakablePart;
struct UnkBreakableTarget;

struct UnkDropTargetData_b0105 {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkDropLink_b0105 {
    u8 pad_0x0[0x10];
    char* object;  // 0x10
};

struct UnkDropHolder_b0105 {
    u8 pad_0x0[0x48];
    UnkDropLink_b0105* link;  // 0x48
};

struct UnkDropEntity_b0105 {
    u8 pad_0x0[0x1b8];
    UnkDropHolder_b0105* holder;  // 0x1b8
};

struct UnkDropIdData_b0105 {
    u8 pad_0x0[0x30];
    u64 id;  // 0x30
};

static inline char* Linked(UnkDropHolder_b0105* holder) {
    return holder->link ? holder->link->object : nullptr;
}

static char* FindObject(UnkDropEntity_b0105* entity) {
    if (UnkDropHolder_b0105* holder = entity->holder) {
        if (char* object = Linked(holder))
            return object;
        fn_710115fe20(reinterpret_cast<UnkHolder*>(holder));
        if (char* object = Linked(holder))
            return object;
    }
    const CStrId* name = fn_7100d7ba84(reinterpret_cast<UnkSpawnedActor_ovb0065*>(entity));
    if (name->data == &g_unk_7101d47108->empty)
        return nullptr;
    UnkGameManager* manager = g_unk_7101d47310;
    u64 id;
    alignas(8) u32 flags;
    u32 flags_value = g_unk_71727bebb8;
    id = reinterpret_cast<UnkDropIdData_b0105*>(name->data)->id;
    flags = flags_value;
    void* found = fn_7100deba8c(manager, &id, &flags);
    if (!found)
        return nullptr;
    return reinterpret_cast<char*>(fn_7100360044(found));
}

void* fn_71007c01a0(void*, UnkDropEntity_b0105* entity) {
    char* object = FindObject(entity);
    if (!object)
        return nullptr;
    auto* target = reinterpret_cast<UnkDropTargetData_b0105*>(
        *fn_71002a76a4(reinterpret_cast<UnkBreakablePart*>(object + 0x60)));
    if (!target)
        return nullptr;
    if (!target->field_0x10)
        return nullptr;
    return fn_71007c04f0(target->field_0x10);
}
