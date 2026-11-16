// CMagnetSurfaceHuskComponent unit (tools/tu_map.py unit 1417): fn_7100925348 sets the husk's "Breakable"
// flag (+0x51): it swaps the two model flags, re-arms the timeline and collider when breakable, and stores the
// flag in the blackboard section of the current scenario. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/CChozoCommanderSentenceSphereAttack.h"
#include "game/CMagnetSurfaceHuskComponent.h"
#include "game/CModelUpdaterComponent.h"
#include "game/CScenario.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkGameManager.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "game/UnkTUProperty.h"
#include "game/UnkTypedValue.h"
#include "types.h"

using base::global::CStrId;

extern u64 g_unk_710164b258;
extern u64 g_unk_710164b260;

struct UnkMagnetSurfaceHusk_b48;

struct UnkHuskCollider_SetBreakable {
    void** vtable;
    void vf_0x60(u32 flags) {
        reinterpret_cast<void (*)(UnkHuskCollider_SetBreakable*, u32)>(vtable[0x60 / 8])(this, flags);
    }
};

struct UnkHuskModelFlag_SetBreakable {
    u8 pad_0x0[0x30];
    u64 id;
};

struct UnkHuskModelFlags_SetBreakable {
    u8 pad_0x0[0x58];
    UnkHuskModelFlag_SetBreakable* on;
    UnkHuskModelFlag_SetBreakable* off;
};

struct UnkHuskOwner_SetBreakable {
    u8 pad_0x0[0x228];
    UnkFlagTarget* model;
    u8 pad_0x230[0x2e8 - 0x230];
    UnkTimelineComponent* timeline;
};

struct UnkHuskValue_SetBreakable {
    UnkPropertyName name;
    u64 field_0x8;
};

struct UnkMagnetSurfaceHusk_SetBreakable {
    u8 pad_0x0[0x18];
    void* field_0x18;
    u8 pad_0x20[0x30 - 0x20];
    UnkHuskOwner_SetBreakable* owner;
    u8 pad_0x38[0x50 - 0x38];
    bool field_0x50;
    bool breakable;
    u8 pad_0x52[0x68 - 0x52];
    UnkHuskValue_SetBreakable* values;
    u64* keys;
    u32 count;
    u8 pad_0x7c[0x90 - 0x7c];
    UnkHuskModelFlags_SetBreakable* flags;
};

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

void fn_7100925348(UnkMagnetSurfaceHusk_b48* husk, bool breakable) {
    auto* self = reinterpret_cast<UnkMagnetSurfaceHusk_SetBreakable*>(husk);
    self->breakable = breakable;
    UnkFlagTarget* model = self->owner->model;
    if (breakable) {
        {
            u64 id = self->flags->on->id;
            fn_7101172780(model, reinterpret_cast<const char*>(&id));
            id = self->flags->off->id;
            fn_7101172764(model, reinterpret_cast<const char*>(&id));
        }
        self->field_0x50 = false;
        UnkTimelineComponent* timeline = self->owner->timeline;
        fn_7100a2b7b8(timeline, &g_unk_710164b258);
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, &g_unk_710164b260, &mask.id, false, -1.0f);
        auto* collider = reinterpret_cast<UnkHuskCollider_SetBreakable*>(fn_71008640a4(
            static_cast<UnkCollisionContainer*>(fn_710043acb4(self->field_0x18)), &g_unk_7172737ef8));
        if (collider)
            collider->vf_0x60(0x20800);
    } else {
        u64 id = self->flags->off->id;
        fn_7101172780(model, reinterpret_cast<const char*>(&id));
        id = self->flags->on->id;
        fn_7101172764(model, reinterpret_cast<const char*>(&id));
    }
    UnkGameManager* manager = g_unk_7101d47310;
    CBlackboard* blackboard = &manager->field_0x968;
    CStrId section_id = fn_71011a2fbc(manager->field_0x608);
    u64 index = 0;
    u32 count = self->count;
    if (count) {
        u64* keys = self->keys;
        u64 key = (u64)g_unk_7172737e70.name.data;
        for (; index != count; ++index) {
            if (keys[index] == key)
                break;
        }
    }
    UnkHuskValue_SetBreakable* values = self->values;
    SetSectionBool(blackboard, section_id, &values[index].name, &self->breakable);
}
