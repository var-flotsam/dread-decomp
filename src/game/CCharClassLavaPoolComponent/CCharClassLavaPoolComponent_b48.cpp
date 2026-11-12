// CLavaPoolComponent (unit of CCharClassLavaPoolComponent, tools/tu_map.py): fn_71008fc2fc reads the
// pool's flag (+0xcb1) from the blackboard section of the current scenario (fn_71010472e0 creates it
// when missing), the property's name looked up in the key list at +0xce0 (count +0xce8, values at
// +0xcd8) as in CEventScenarioComponent_b46.cpp. When it was read and is set, the level (+0xcc4) is
// copied to the three heights at +0xc50 and the pool moves there, at once (fn_7100902334, when +0xcb2
// is set) or not (fn_710090248c). fn_71008fc524 is its writer: the owner's timeline (+0x2e8 of +0x30)
// drops two ids (fn_7100a2b7b8), the flag is set and stored in the section unless the blackboard is
// locked (fn_7100332c80, as in CDoorLifeComponent.cpp), then fn_7100901260 runs. Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CBlackboard.h"
#include "game/CCharClassLiquidPoolComponent.h"
#include "game/CScenario.h"
#include "game/UnkGameManager.h"
#include "game/UnkTimelineComponent.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;

extern UnkProperty g_unk_7172733338;

struct UnkLavaPoolValue_b48 {
    union {
        CStrId id;
        UnkPropertyName name;
    };
    u64 field_0x8;
};

// A typed value as the section setter takes it (as in src/game/CDoorLifeComponent.cpp).
struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};
void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);

extern u64 g_unk_710164b1d8;
extern u64 g_unk_710164b1e0;

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

struct UnkLavaPoolOwner_b48 {
    u8 pad_0x0[0x2e8];
    UnkTimelineComponent* timeline;
};

struct UnkLavaPool_b48 {
    u8 pad_0x0[0x30];
    UnkLavaPoolOwner_b48* owner;
    u8 pad_0x38[0xc50 - 0x38];
    f32 height_0xc50;
    f32 height_0xc54;
    f32 height_0xc58;
    u8 pad_0xc5c[0xcb1 - 0xc5c];
    bool filled;
    bool instant;
    u8 pad_0xcb3[0x11];
    f32 level;
    u8 pad_0xcc8[0x10];
    UnkLavaPoolValue_b48* values;
    u64* keys;
    u32 count;
};

void fn_7100902334(UnkLavaPool_b48* self, f32 height);
void fn_710090248c(UnkLavaPool_b48* self, f32 height);

void fn_71008fc2fc(UnkLavaPool_b48* self, CBlackboard* blackboard) {
    CBlackboard::CSection* section = fn_71010472e0(blackboard, fn_71011a2fbc(g_unk_7101d47310->field_0x608));
    if (!section)
        return;
    u64 index = 0;
    u32 count = self->count;
    if (count) {
        u64* keys = self->keys;
        u64 key = (u64)g_unk_7172733338.name.data;
        for (; index != count; ++index) {
            if (keys[index] == key)
                break;
        }
    }
    UnkLavaPoolValue_b48* values = self->values;
    bool* filled = &self->filled;
    if (fn_710006d11c(section, &values[index].id, filled) && *filled) {
        f32 level = self->level;
        self->height_0xc58 = level;
        self->height_0xc54 = level;
        self->height_0xc50 = level;
        if (self->instant)
            fn_7100902334(self, level);
        else
            fn_710090248c(self, level);
    }
}

void fn_71008fc524(UnkLavaPool_b48* self) {
    if (UnkLavaPoolOwner_b48* owner = self->owner) {
        if (UnkTimelineComponent* timeline = owner->timeline) {
            fn_7100a2b7b8(timeline, &g_unk_710164b1d8);
            fn_7100a2b7b8(timeline, &g_unk_710164b1e0);
        }
    }
    UnkGameManager* manager = g_unk_7101d47310;
    CBlackboard* blackboard = &manager->field_0x968;
    CStrId section_id = fn_71011a2fbc(manager->field_0x608);
    self->filled = true;
    u64 index = 0;
    u32 count = self->count;
    if (count) {
        u64* keys = self->keys;
        u64 key = (u64)g_unk_7172733338.name.data;
        for (; index != count; ++index) {
            if (keys[index] == key)
                break;
        }
    }
    UnkLavaPoolValue_b48* values = self->values;
    SetSectionBool(blackboard, section_id, &values[index].name, &self->filled);
    fn_7100901260(self);
}
