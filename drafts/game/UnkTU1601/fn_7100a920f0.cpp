// CThermalDeviceComponent's unit (tools/tu_map.py unit 1601): refreshes the device after its state changed.
// Reads the usable flag from the scenario's blackboard section (as the freeze function does), runs the usable
// or the unusable handler, tells the owner's character class about it, and sets the render parameters of the
// current and the previous state. Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/math/CVector4D.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CBlackboard.h"
#include "game/CScenario.h"
#include "game/CUsableComponent.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU1601.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU315.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;

struct UnkThermalDevice_A5_b73;
struct UnkThermalDevice_b0279;
void fn_7100a92424(UnkThermalDevice_b0279* self);

__attribute__((visibility("hidden"))) extern f32* g_unk_7101a93290[8];

struct UnkThermalConfig_la920f0 {
    u8 pad_0x0[0x30];
    u64 hash;  // 0x30
};

struct UnkThermalConfigHolder_la920f0 {
    UnkThermalConfig_la920f0* config;
};

struct UnkThermalValue_la920f0 {
    UnkPropertyName name;
    u64 field_0x8;
};

struct UnkThermalRefresh_la920f0 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x498];
    UnkThermalValue_la920f0* values;  // 0x4d0
    u64* keys;                        // 0x4d8
    u32 key_count;                    // 0x4e0
    u8 pad_0x4e4[0x58];
    s32 stateNow;   // 0x53c
    s32 statePrev;  // 0x540
    u8 pad_0x544[0x59d - 0x544];
    bool usable;  // 0x59d

    UnkThermalConfigHolder_la920f0* vf_0x218() {
        return reinterpret_cast<UnkThermalConfigHolder_la920f0* (*)(UnkThermalRefresh_la920f0*)>(vtable[0x218 / 8])(
            this);
    }
};

static inline void SetRender() {
    fn_710021a0b4(g_unk_7101d47300, &g_unk_710164b5f0, &g_unk_7172751c18);
    fn_710021a0b4(g_unk_7101d47300, &g_unk_710164b5f8, &g_unk_7101a92ec0);
}

void fn_7100a920f0(UnkThermalRefresh_la920f0* self) {
    fn_7100a9f300(reinterpret_cast<UnkTeleporterSelf_b0233*>(self));
    bool usable = false;
    UnkGameManager* manager = g_unk_7101d47310;
    CBlackboard* blackboard = &manager->field_0x968;
    base::reflection::UnkReflTypeIdArg mode;
    base::reflection::UnkReflTypeIdArg mask;
    {
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
        UnkThermalValue_la920f0* values = self->values;
        if (CBlackboard::CSection* s = fn_7101047270(blackboard, section_id))
            fn_710006d11c(s, reinterpret_cast<const CStrId*>(&values[index].name), &usable);
    }
    self->usable = usable;
    if (usable) {
        fn_7100a925a0(reinterpret_cast<UnkThermalDevice_A5_b73*>(self));
        void* cls = self->owner->field_0x220;
        if (cls) {
            u64 hash = self->vf_0x218()->config->hash;
            mode.id = 0;
            mask.id = UnkAll_SpitSend().value;
            fn_7101176690(cls, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
        }
    } else {
        fn_7100a92424(reinterpret_cast<UnkThermalDevice_b0279*>(self));
    }

    s32 now = self->stateNow;
    s32 prev = self->statePrev;
    if (now == prev) {
        if ((u32)now <= 7) {
            *g_unk_7101a93290[now] = usable ? (f32)now + 1.0f : 0.0f;
            SetRender();
        }
    } else {
        if ((u32)now <= 7) {
            *g_unk_7101a93290[now] = usable ? 0.0f : (f32)now + 1.0f;
            SetRender();
        }
        if ((u32)self->statePrev <= 7) {
            *g_unk_7101a93290[self->statePrev] = usable ? (f32)self->statePrev + 1.0f : 0.0f;
            SetRender();
        }
    }
}
