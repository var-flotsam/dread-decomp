// Unit 1303 (tools/tu_map.py, no name hint; next to CGunComponent::CTunableGunComponent): returns the
// parameter record of a gun kind (1-3), refreshed from the gun tunable's pair of values for that kind,
// or null. Names are placeholders.
#include "base/reflection/UnkReflAttributes.h"
#include "types.h"

struct UnkGunParams_A5_b65 {
    u32 field_0x0;
    f32 field_0x4;
    f32 field_0x8;
};

struct UnkGunTunable_A5_b65 {
    u8 pad_0x0[0x28];
    f32 field_0x28[2];
    f32 field_0x30[2];
    f32 field_0x38[2];
};

extern base::reflection::UnkTunable g_unk_7172726228;
extern UnkGunParams_A5_b65 g_unk_7101a77b98 __attribute__((visibility("hidden")));
extern UnkGunParams_A5_b65 g_unk_7101a77ba4 __attribute__((visibility("hidden")));
extern UnkGunParams_A5_b65 g_unk_7101a77bb0 __attribute__((visibility("hidden")));

UnkGunParams_A5_b65* fn_7100837220(s32 kind) {
    UnkGunTunable_A5_b65* tunable = reinterpret_cast<UnkGunTunable_A5_b65*>(&g_unk_7172726228);
    UnkGunParams_A5_b65* params;
    const f32* a;
    f32* da;
    const f32* b;
    f32* db;
    switch (kind) {
    case 1:
        params = &g_unk_7101a77b98;
        da = &params->field_0x4;
        db = &params->field_0x8;
        a = &tunable->field_0x28[0];
        b = &tunable->field_0x28[1];
        break;
    case 2:
        params = &g_unk_7101a77ba4;
        da = &params->field_0x4;
        db = &params->field_0x8;
        a = &tunable->field_0x30[0];
        b = &tunable->field_0x30[1];
        break;
    case 3:
        params = &g_unk_7101a77bb0;
        da = &params->field_0x4;
        db = &params->field_0x8;
        a = &tunable->field_0x38[0];
        b = &tunable->field_0x38[1];
        break;
    default:
        return nullptr;
    }
    *da = *a;
    *db = *b;
    return params;
}
