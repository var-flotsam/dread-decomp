// CMagnetSlidingBlockComponent's unit (tools/tu_map.py 1412): the slide's end (fn_710092073c). The rail's glow
// goes to full, the timer restarts, the owner's object is told the "finished" id; once the block is used up its
// rail goes dark and the "used" property is set in the current scenario's blackboard section; a running
// vibration fades out. Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "base/UnkStrIdTable.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/CMagnetSlidingBlockComponent.h"
#include "game/CMagnetSlidingBlockRailComponent.h"
#include "game/CNavMeshItemComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkHandle.h"
#include "game/UnkHandleTarget.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU372.h"
#include "game/UnkTU437.h"
#include "game/UnkTU50.h"
#include "game/UnkTUProperty.h"
#include "game/UnkTypedValue.h"
#include "types.h"

using base::global::CStrId;

struct UnkMagnetSlidingBlockRail_ovb0097;
// The rail component of an object.

extern CStrId g_unk_71727375e8 __attribute__((visibility("hidden")));  // "finished"

static inline void* HandleObject(const UnkHandle& handle) {
    if (handle.target && handle.target->field_0x8 == handle.serial && handle.serial != ~0ull)
        return handle.target->field_0x0;
    return nullptr;
}

// What the rail handle refers to: a counted reference to the rail's object.
struct UnkMagnetRailRef_l92073c : base::UnkRefCounted {
    void* object;  // 0x10
};

struct UnkMagnetBlockFlags_l92073c {
    u8 pad_0x0[0x5c];
    bool used_up;  // 0x5c
};

struct UnkMagnetSlidingBlock_l92073c {
    u8 pad_0x0[0x30];
    UnkWeakTarget* owner;  // 0x30
    u8 pad_0x38[0x8];
    UnkMagnetBlockFlags_l92073c* flags;  // 0x40
    u8 pad_0x48[0x350 - 0x48];
    f32 glow;  // 0x350
    u8 pad_0x354[0x408 - 0x354];
    u32 timer;     // 0x408
    u32 duration;  // 0x40c
    u32 field_0x410;
    u8 pad_0x414[0x430 - 0x414];
    u8 rail_source[0x28];  // 0x430
    s32 rail_state;        // 0x458
    u8 pad_0x45c[0x4];
    base::UnkRefHandle rail;  // 0x460
    u8 pad_0x468[0x478 - 0x468];
    UnkStrIdTable properties;  // 0x478
    u8 pad_0x4a0[0x4e0 - 0x4a0];
    UnkHandle vibration;  // 0x4e0
};
static_assert(__builtin_offsetof(UnkMagnetSlidingBlock_l92073c, rail) == 0x460, "layout");
static_assert(__builtin_offsetof(UnkMagnetSlidingBlock_l92073c, properties) == 0x478, "layout");
static_assert(__builtin_offsetof(UnkMagnetSlidingBlock_l92073c, vibration) == 0x4e0, "layout");

// The rail's object, the handle to it fetched on first use.
static inline void* RailObject(UnkMagnetSlidingBlock_l92073c* self) {
    if (self->rail_state == 1) {
        {
            base::UnkRefPtrMember fetched = fn_7100353be8(self->rail_source);
            fn_710031d208(&self->rail, reinterpret_cast<const base::UnkRefHandle*>(&fetched));
        }
        self->rail_state = 2;
    }
    if (base::UnkRefCounted* ref = self->rail.ptr)
        return static_cast<UnkMagnetRailRef_l92073c*>(ref)->object;
    return nullptr;
}

// Stores the bool at `value` under `name` in the blackboard section `section` names (unless the
// blackboard is locked).
static inline void SetSectionBool(CBlackboard* blackboard, const CStrId& section, UnkPropertyName* name, bool* value) {
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = fn_710000d228();
            v.value = value;
            fn_7100332c80(s, name, &v);
        }
    }
}

void fn_710092073c(UnkMagnetSlidingBlock_l92073c* self) {
    self->glow = 1.0f;
    if (void* rail = RailObject(self))
        fn_7100922f28(fn_710091f6bc(rail), self->glow);
    self->field_0x410 = 0;
    self->timer = self->duration;

    if (void* target = fn_71002b3b84(self->owner))
        fn_71009c0f1c(target, g_unk_71727375e8.data->Key());

    if (self->flags->used_up) {
        if (RailObject(self))
            fn_7100922e30(fn_710091f6bc(RailObject(self)), false);

        UnkGameManager* manager = g_unk_7101d47310;
        CBlackboard* blackboard = &manager->field_0x968;
        CStrId section_id = fn_7100de9c30(manager);
        u64 index = 0;
        u32 count = self->properties.count;
        if (count) {
            CStrId* keys = self->properties.keys;
            for (; index != count; ++index) {
                if (keys[index].data == g_unk_7172737530.name.data)
                    break;
            }
        }
        UnkStrIdTableValue* values = self->properties.values;
        bool used = true;
        SetSectionBool(blackboard, section_id, reinterpret_cast<UnkPropertyName*>(&values[index].id), &used);
    }

    if (HandleObject(self->vibration) != HandleObject(g_unk_7101d586c0))
        fn_7100072328(HandleObject(self->vibration), 0.2f, 0.0f);
}
