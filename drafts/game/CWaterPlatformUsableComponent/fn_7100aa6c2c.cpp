// CWaterPlatformUsableComponent unit (tools/tu_map.py): after the base method, reads the usable's
// "Completed" flag from its blackboard section; a completed platform is marked used, told so (mode 2),
// raised, runs its "OnUseCompleted" script and lights the guide; otherwise the guide light is off.
// Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/UnkTimedCurve.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/math/CVector4D.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CBlackboard.h"
#include "game/CUsableComponent.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU315.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::math::CVector4D;

struct UnkWaterPlatformOwner_laa6c2c {
    u8 pad_0x0[0x1e0];
    UnkScriptCaller* script;  // 0x1e0
    u8 pad_0x1e8[0x220 - 0x1e8];
    void* field_0x220;
};

struct UnkWaterPlatformValue_laa6c2c {
    CStrId id;
    u64 field_0x8;
};

struct UnkWaterPlatform_laa6c2c {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkWaterPlatformOwner_laa6c2c* owner;  // 0x30
    u8 pad_0x38[0x4d0 - 0x38];
    UnkWaterPlatformValue_laa6c2c* values;  // 0x4d0
    u64* keys;                              // 0x4d8
    u32 count;                              // 0x4e0
    u8 pad_0x4e4[0x568 - 0x4e4];
    UnkTimedCurve curve;  // 0x568
    u8 pad_0x5b8[0x5c5 - 0x5b8];
    bool used;  // 0x5c5

    // The blackboard section the usable's flags are stored in.
    CStrId SectionId() {
        return reinterpret_cast<CStrId (*)(UnkWaterPlatform_laa6c2c*)>(vtable[0x2d8 / 8])(this);
    }

    // The value stored under `property`'s name (one past the end when there is none).
    UnkWaterPlatformValue_laa6c2c* Find(const UnkProperty& property) {
        u64 index = 0;
        u32 n = count;
        if (n) {
            u64* ids = keys;
            u64 key = (u64)property.name.data;
            for (; index != n; ++index) {
                if (ids[index] == key)
                    break;
            }
        }
        return &values[index];
    }
};
static_assert(__builtin_offsetof(UnkWaterPlatform_laa6c2c, curve) == 0x568, "layout");
static_assert(__builtin_offsetof(UnkWaterPlatform_laa6c2c, used) == 0x5c5, "layout");

void fn_7100a9eff4(UnkWaterPlatform_laa6c2c* self, CBlackboard* blackboard);  // the base method
void fn_7100aa6a2c(f32 value, UnkWaterPlatform_laa6c2c* self);

// Starts the platform's curve towards `target`.
static inline void StartCurve(UnkWaterPlatform_laa6c2c* self, f32 target) {
    CRntString modifiers(g_unk_710162e2ed);
    fn_7100075f78(0.0f, target, 0.0f, &self->curve, &modifiers);
}

void fn_7100aa6c2c(UnkWaterPlatform_laa6c2c* self, CBlackboard* blackboard) {
    u64 light;  // the guide light's name hash
    fn_7100a9eff4(self, blackboard);
    CBlackboard::CSection* section = fn_71010472e0(blackboard, self->SectionId());
    if (!section)
        return;
    fn_7100aa6a2c(0.0f, self);
    StartCurve(self, 0.2f);
    bool completed = false;
    bool found = fn_710006d11c(section, &self->Find(*reinterpret_cast<UnkProperty*>(g_unk_7172753170))->id, &completed);
    if (completed && found) {
        self->used = true;
        void* target = self->owner->field_0x220;
        // The mode is a plain 4-byte value here (the original's slot has the flag above right after it).
        u32 mode = 2;
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7101176690(target, 0x9923746b468b51e5, 0x94b6e812cbf3fb75, reinterpret_cast<base::reflection::UnkReflTypeIdArg*>(&mode), &g_unk_710164b968, &g_unk_7172769740,
                      &mask);
        StartCurve(self, 0.5f);
        fn_7100aa6a2c(1.0f, self);
        if (UnkScriptCaller* script = self->owner->script)
            fn_7100305088(script, "OnUseCompleted", g_unk_710162e2ed);
        light = fn_7100001570("GUIDE_LGT_00", 12);
        CVector4D color = {15.0f, 15.0f, 15.0f, 1.0f};
        fn_710021a0b4(g_unk_7101d47300, &light, &color);
    } else {
        light = fn_7100001570("GUIDE_LGT_00", 12);
        CVector4D color = {0.0f, 0.0f, 0.0f, 1.0f};
        fn_710021a0b4(g_unk_7101d47300, &light, &color);
    }
}
