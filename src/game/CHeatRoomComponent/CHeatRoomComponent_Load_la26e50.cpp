// CHeatRoomComponent's unit (tools/tu_map.py unit 1538): after the base step (fn_7100a46e78), takes the
// current heat parameters as fn_7100a26758 does, reads the room's stored state (under the owner's name in
// the current scenario's blackboard section, as CHeatRoomComponent_b49.cpp stores it), switches the entity
// at +0x18 to it, and when it is on at full strength hands that strength to every part's effect object
// (slot 0x168); then refreshes the room (fn_7100a26828). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/CCharClassBaseTriggerComponent.h"
#include "game/CHeatRoomComponent.h"
#include "game/CPullOffGrapplePointComponent.h"
#include "game/CScenario.h"
#include "game/UnkBreakableTarget.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkHeatCurrent_ovb0083.h"
#include "game/UnkHeatPair_ovb0083.h"
#include "game/UnkHeatParams_ovb0083.h"
#include "game/UnkSubId_7100fc4b34.h"
#include "game/UnkTU341.h"
#include "game/UnkTU365.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;

// Takes a parameter set; the first value only when the set was not applied before.
inline void ApplyHeat_la26e50(UnkHeatCurrent_ovb0083& current, const UnkHeatParams_ovb0083& params, bool wasSet) {
    if (!wasSet)
        current.field_0x0 = params.field_0x0;
    current.field_0x4 = params.field_0x4;
    current.field_0x8.x = params.field_0x8.x;
    current.field_0x8.y = params.field_0x8.y;
    current.field_0x10 = params.field_0x10;
    current.field_0x14 = params.field_0x18;
}

// The id of `name` within the property's qualified name (the name built on first use), as
// CHeatRoomComponent_b49.cpp's SubId.
static inline UnkSubId_7100fc4b34 SubId(UnkPropertyEx<0x160>& def, const CStrId& name) {
    UnkQualifiedName* qualified = reinterpret_cast<UnkQualifiedName*>(def.field_0x10);
    if (!qualified->str || !qualified->str[0])
        fn_7100ae9d7c(qualified, reinterpret_cast<UnkIdSource*>(&def), base::global::CStrId::Empty());
    return fn_71003327a4(qualified, &name);
}

// Reads the bool stored under `key` in the blackboard section `section` names.
static inline bool ReadBool(CBlackboard* blackboard, const CStrId& section, const UnkSubId_7100fc4b34& key, bool* out) {
    if (CBlackboard::CSection* s = fn_7101047270(blackboard, section))
        return fn_710006d11c(s, &key.id, out);
    return false;
}

struct UnkHeatRoomOwner_la26e50 {
    u8 pad_0x0[0x30];
    CStrId name;  // 0x30
};

struct UnkHeatEffect_la26e50 {
    virtual void vf_0x0();
    // Slot 0x168: takes the room's strength.
    void SetStrength(f32 value) {
        typedef void (*Fn)(UnkHeatEffect_la26e50*, f32);
        (*reinterpret_cast<Fn**>(this))[0x168 / 8](this, value);
    }
};

// A part of the room (0x40 bytes).
struct UnkHeatPart_la26e50 {
    u8 data[0x40];
};

struct UnkHeatRoom_la26e50 {
    u8 pad_0x0[0x18];
    UnkEntityState* entity;  // 0x18
    u8 pad_0x20[0x10];
    UnkHeatRoomOwner_la26e50* owner;  // 0x30
    u8 pad_0x38[0x44];
    f32 strength;  // 0x7c
    u8 pad_0x80[0xb40];
    UnkHeatCurrent_ovb0083 current;  // 0xbc0
    u8 pad_0xbd8[0xc];
    UnkHeatParams_ovb0083 normal;  // 0xbe4
    UnkHeatParams_ovb0083 alt;     // 0xc04
    u32 use_alt;                   // 0xc24
    UnkHeatPart_la26e50* parts;    // 0xc28
    u32 part_count;                // 0xc30
    u8 pad_0xc34[0x4c];
    bool normal_was_set;  // 0xc80
    bool alt_was_set;     // 0xc81
};
static_assert(__builtin_offsetof(UnkHeatRoom_la26e50, strength) == 0x7c, "UnkHeatRoom_la26e50 layout");
static_assert(__builtin_offsetof(UnkHeatRoom_la26e50, use_alt) == 0xc24, "UnkHeatRoom_la26e50 layout");
static_assert(__builtin_offsetof(UnkHeatRoom_la26e50, normal_was_set) == 0xc80, "UnkHeatRoom_la26e50 layout");

void CHeatRoomComponent_LoadHeatRoomEnabledFromBlackboard(UnkHeatRoom_la26e50* self) {
    fn_7100a46e78(reinterpret_cast<CFreezeRoomComponent*>(self));
    UnkGameManager* manager = g_unk_7101d47310;
    u32 useAlt = reinterpret_cast<u8*>(manager)[0x2441];
    self->use_alt = useAlt;
    if (useAlt)
        ApplyHeat_la26e50(self->current, self->alt, self->alt_was_set);
    else
        ApplyHeat_la26e50(self->current, self->normal, self->normal_was_set);

    CBlackboard* blackboard = &manager->field_0x968;
    CStrId section_id = fn_71011a2fbc(manager->field_0x608);
    bool enabled = false;
    if (ReadBool(blackboard, section_id, SubId(g_unk_7172746208, self->owner->name), &enabled))
        fn_7100232864(self->entity, enabled);
    if (enabled && self->strength == 1.0f) {
        for (UnkHeatPart_la26e50 *it = self->parts, *end = it + self->part_count; it != end; ++it) {
            UnkBreakablePart* part = reinterpret_cast<UnkBreakablePart*>(it);
            UnkBreakableTarget* target = *fn_71002a76a4(part);
            if (!target || !target->field_0x10)
                continue;
            target = *fn_71002a76a4(part);
            if (void* effect = fn_710082e510(target ? target->field_0x10 : nullptr))
                static_cast<UnkHeatEffect_la26e50*>(effect)->SetStrength(self->strength);
        }
    }
    fn_7100a26828(reinterpret_cast<UnkHeatRoom_b49*>(self));
}
