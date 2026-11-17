// CThermalRoomConnectionFX's unit (tools/tu_map.py unit 1334): resolves the two links (+0x80, +0xc0) and
// takes weak references to what they lead to: from the first, its door lock (+0x100) and another object
// (+0x108), with the owner's component at +0x2e8 (+0x118), clearing the flags at +0x121 and +0x123 once both
// of a pair are alive; from the second, its object (+0x110), whose state then shows or hides the owner's
// effect. Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "game/CActorComponent.h"
#include "game/CCollisionMaterialCacheComponent.h"
#include "game/CGameLink.h"
#include "game/UnkTU367.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

UnkWeakTarget* fn_710031cd8c(UnkWeakTarget* target);

struct UnkThermalRoomOwner_l87aa74 {
    u8 pad_0x0[0x2e8];
    UnkWeakTarget* field_0x2e8;
};

struct UnkThermalRoomState_l87aa74 {
    u8 pad_0x0[0x1a8];
    void* field_0x1a8;
    bool field_0x1b0;
    u8 pad_0x1b1[3];
    bool field_0x1b4;
};

struct UnkThermalRoomTarget_l87aa74 : UnkWeakTarget {
    u8 pad_0x8[0x28];
    UnkThermalRoomState_l87aa74* field_0x30;
    u8 pad_0x38[0x38];
    u32 field_0x70;
};
static_assert(__builtin_offsetof(UnkThermalRoomTarget_l87aa74, field_0x70) == 0x70, "layout");

struct UnkThermalRoomConnectionFX_l87aa74 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkThermalRoomOwner_l87aa74* owner;  // 0x30
    u8 pad_0x38[0x48];
    CGameLink<UnkWeakTarget> link_a;  // 0x80
    CGameLink<UnkWeakTarget> link_b;  // 0xc0
    UnkWeakHandle* field_0x100;
    UnkWeakHandle* field_0x108;
    UnkWeakHandle* field_0x110;
    UnkWeakHandle* field_0x118;
    u8 pad_0x120;
    bool field_0x121;
    u8 pad_0x122;
    bool field_0x123;
};
static_assert(__builtin_offsetof(UnkThermalRoomConnectionFX_l87aa74, field_0x123) == 0x123, "layout");

static inline void Resolve(CGameLink<UnkWeakTarget>& link) {
    if (link.state == 1) {
        {
            UnkRefPtr ref = fn_710023b580(&link.callbacks);
            // The reference's pointer is the handle's.
            fn_7100234f78(&link.handle, reinterpret_cast<const base::UnkRefHandle*>(&ref));
        }
        link.state = 2;
    }
}

static inline bool IsAlive(UnkWeakHandle* handle) {
    return handle && handle->target;
}

static inline void SetEffectEnabled(UnkThermalRoomConnectionFX_l87aa74* self, bool enabled) {
    if (UnkStruct_710024fa0c* fx = fn_7100798eec(self->owner))
        fn_710024fa0c(fx, enabled);
}

void fn_710087aa74(UnkThermalRoomConnectionFX_l87aa74* self) {
    Resolve(self->link_a);
    if (UnkWeakTarget* target = self->link_a.Target()) {
        if (UnkDoorLock* lock = fn_71002af860(target))
            AssignRef(&self->field_0x100, GetWeakHandle(reinterpret_cast<UnkWeakTarget*>(lock)));
        if (UnkWeakTarget* component = self->owner->field_0x2e8)
            AssignRef(&self->field_0x118, GetWeakHandle(component));
        if (IsAlive(self->field_0x100) && IsAlive(self->field_0x118))
            self->field_0x121 = false;
        if (UnkWeakTarget* object = fn_710031cd8c(target))
            AssignRef(&self->field_0x108, GetWeakHandle(object));
        if (IsAlive(self->field_0x108) && IsAlive(self->field_0x118))
            self->field_0x123 = false;
    }

    Resolve(self->link_b);
    if (UnkWeakTarget* target = self->link_b.Target()) {
        if (UnkWeakTarget* object = fn_710031cd8c(target))
            AssignRef(&self->field_0x110, GetWeakHandle(object));
        if (UnkWeakHandle* handle = self->field_0x110) {
            if (auto* room = static_cast<UnkThermalRoomTarget_l87aa74*>(handle->target)) {
                UnkThermalRoomState_l87aa74* state = room->field_0x30;
                if (room->field_0x70 == 2 || !state->field_0x1b4 || (state->field_0x1b0 && !state->field_0x1a8))
                    SetEffectEnabled(self, true);
                else
                    SetEffectEnabled(self, false);
            }
        }
    }
}
