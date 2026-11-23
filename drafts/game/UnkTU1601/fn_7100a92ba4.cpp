// CThermalDeviceComponent's unit (tools/tu_map.py unit 1601): after fn_7100aa3234, shows the thermal door
// pop-up message, moves the heat level from the previous slot to the current one (clearing the old slot's
// value and setting the two render parameters each time), and stores true under the usable property in the
// current scenario's blackboard section. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/math/CVector4D.h"
#include "base/reflection/ReflectedAttributes.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/CScenario.h"
#include "game/CUsableComponent.h"
#include "game/GUI/CPopUpComposition.h"
#include "game/UnkGameManager.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU315.h"
#include "game/UnkTUProperty.h"
#include "game/UnkTypedValue.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector4D;

extern const u64 g_unk_710164b5f0 __attribute__((visibility("hidden")));
extern const u64 g_unk_710164b5f8 __attribute__((visibility("hidden")));
extern const CVector4D g_unk_7172751c18;
extern const CVector4D g_unk_7101a92ec0;
// The value of each of the eight heat levels.
extern f32* const g_unk_7101a93290[8] __attribute__((visibility("hidden")));

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

struct UnkThermalValue_la92ba4 {
    UnkPropertyName name;
    u64 field_0x8;
};

struct UnkThermalDevice_la92ba4 {
    u8 pad_0x0[0x4d0];
    UnkThermalValue_la92ba4* values;  // 0x4d0
    u64* keys;                        // 0x4d8
    u32 key_count;                    // 0x4e0
    u8 pad_0x4e4[0x53c - 0x4e4];
    s32 previous_level;  // 0x53c
    s32 level;           // 0x540
    u8 pad_0x544[0x598 - 0x544];
    f32 field_0x598;
    u8 pad_0x59c;
    bool field_0x59d;
};
static_assert(__builtin_offsetof(UnkThermalDevice_la92ba4, field_0x59d) == 0x59d, "layout");

struct UnkPopUpEntry_la92ba4 {
    u8 pad_0x0[0x10];
    void* composition;
};

void fn_7100aa3234(UnkThermalDevice_la92ba4* self);

// Sets heat level `level`'s value (its number plus one when it is the active one, 0 otherwise) and the two
// render parameters (levels 0 to 7 only).
static inline void SetHeatLevel_la92ba4(s32 level, bool active) {
    if ((u32)level < 8) {
        *g_unk_7101a93290[level] = active ? level + 1.0f : 0.0f;
        fn_710021a0b4(g_unk_7101d47300, &g_unk_710164b5f0, &g_unk_7172751c18);
        fn_710021a0b4(g_unk_7101d47300, &g_unk_710164b5f8, &g_unk_7101a92ec0);
    }
}

void fn_7100a92ba4(UnkThermalDevice_la92ba4* self) {
    fn_7100aa3234(self);
    auto* entry = static_cast<UnkPopUpEntry_la92ba4*>(g_unk_7101d47310->field_0x2258);
    void* composition;
    if (entry && (composition = entry->composition)) {
        static CStrId s_message("#CAVE_THERMAL_DOOR_MESSAGE", true);
        fn_7100b5fd6c(composition, &s_message.data, &g_unk_7101d470e0, true, false);
    }
    s32 previous = self->previous_level;
    if (previous == self->level) {
        SetHeatLevel_la92ba4(previous, true);
    } else {
        SetHeatLevel_la92ba4(previous, false);
        SetHeatLevel_la92ba4(self->level, true);
    }
    self->field_0x598 = 0.5f;

    {
        UnkGameManager* manager = g_unk_7101d47310;
        CBlackboard* blackboard = &manager->field_0x968;
        CStrId section_id = fn_71011a2fbc(manager->field_0x608);
        u64 index = 0;
        u32 count = self->key_count;
        if (count) {
            u64* keys = self->keys;
            u64 key = (u64)reinterpret_cast<UnkProperty*>(g_unk_7172753148)->name.data;
            for (; index != count; ++index) {
                if (keys[index] == key)
                    break;
            }
        }
        UnkThermalValue_la92ba4* values = self->values;
        bool value = true;
        SetSectionBool(blackboard, section_id, &values[index].name, &value);
    }
    self->field_0x59d = true;
}
