// CDrop's unit (tools/tu_map.py): the drop's constructor and destructor bodies (15 entries of two ids at
// +0x20, two ids, three id/path records), its amount (scaled per item in game mode 3 and rounded away
// from zero) and the object it is linked to, through its holder or else the game manager's id lookup.
// Names and layouts are placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/CAISmartObjectComponent.h"
#include "game/CDrop.h"
#include "game/CDropLayout.h"
#include "game/CGameObject.h"
#include "game/CLifeComponent.h"
#include "game/CLogicDroppedItemManager/CTunableDroppableBillboard.h"
#include "game/CPlayerLifeComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkPlayerLifeTunable.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2561.h"
#include "game/UnkTU32.h"
#include "game/UnkTU365.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkIdPair;
struct UnkCameraCtrl_ovb0049;
extern char g_unk_7101a71568[];  // CDrop's vtable

void fn_71007bf3f0(CDrop* self) {
    fn_7101043904(self);
    self->vtable = reinterpret_cast<void**>(g_unk_7101a71568) + 2;
    self->owner = nullptr;
    self->callback = nullptr;
    new (&self->table) UnkDropTable;
    new (&self->item) CStrId(g_unk_710162e2ed, true);
    self->field_0x468 = 0;
    new (&self->field_0x470) CStrId(g_unk_710162e2ed, true);
    self->field_0x480 = 0;
    self->field_0x478 = 0;
    self->field_0x48c = 0;
    self->field_0x490 = 0;
    self->field_0x494 = 0;
    self->field_0x498 = 0;
    self->mode = 1;
    self->field_0x49c_2 = 0;
    new (&self->field_0x4a0) UnkStrIdPathResource();
    new (&self->field_0x4b8) UnkStrIdPathResource();
    new (&self->field_0x4d0) UnkStrIdPathResource();
    self->field_0x4e8 = false;
}

void fn_71007bfa70(CDrop* self) {
    self->vtable = reinterpret_cast<void**>(g_unk_7101a71568) + 2;
    fn_7100026f70(reinterpret_cast<UnkIdPair*>(&self->field_0x4d0));
    fn_7100026f70(reinterpret_cast<UnkIdPair*>(&self->field_0x4b8));
    fn_7100026f70(reinterpret_cast<UnkIdPair*>(&self->field_0x4a0));
    self->field_0x470.~CStrId();
    self->item.~CStrId();
    self->table.~UnkDropTable();
    fn_7101043934(reinterpret_cast<UnkCameraCtrl_ovb0049*>(self));
}

struct UnkDroppableTunable;

static inline f32 ScaledAmount(f32 amount, f32 scale) {
    f32 v = amount * scale;
    return v > 0.0f ? __builtin_ceilf(v) : __builtin_floorf(v);
}

// The drop's amount; in game mode 3 scaled by the item's multiplier and rounded away from zero.
f32 fn_71007bffe4(CDrop* self) {
    if (fn_7100dffc58(g_unk_7101d47310) == 3) {
        base::global::UnkStringData* item = self->item.data;
        if (item == g_unk_71727c1048.data)
            return ScaledAmount(self->amount, g_unk_7172a0a0a8.fEasyModeDropAmountFactor_Life);
        if (item == g_unk_71727c1050.data)
            return ScaledAmount(self->amount, g_unk_7172a0a0a8.fEasyModeDropAmountFactor_LifeBig);
        if (item == g_unk_71727c1058.data)
            return ScaledAmount(self->amount, g_unk_7172a0a0a8.fEasyModeDropAmountFactor_Missile);
        if (item == g_unk_71727c1060.data)
            return ScaledAmount(self->amount, g_unk_7172a0a0a8.fEasyModeDropAmountFactor_MissileBig);
        if (item == g_unk_71727c1068.data)
            return ScaledAmount(self->amount, g_unk_7172a0a0a8.fEasyModeDropAmountFactor_PowerBomb);
        if (item == g_unk_71727c1070.data)
            return ScaledAmount(self->amount, g_unk_7172a0a0a8.fEasyModeDropAmountFactor_PowerBombBig);
        if (item == g_unk_71727c1078.data) {
            f32 amount = self->amount;
            f32 limit = *reinterpret_cast<f32*>(reinterpret_cast<char*>(&g_unk_71727c10c8) + 0x60);
            if (amount + 0.001f >= limit && amount - 0.001f <= limit)
                return ScaledAmount(amount, g_unk_7172a0a0a8.fEasyModeDropAmountFactor_MiniXMissile);
            return ScaledAmount(amount, g_unk_7172a0a0a8.fEasyModeDropAmountFactor_MiniXLife);
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
