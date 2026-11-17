// CAttackPreset (unit 446): the preset's value for a target, by how far the team manager puts the target
// (fn_71011a76ec) against the attack's near and far distances, one set of three values or the other by
// the request's flag; zero without a target or ranges. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/CTeamManager/CTunableTeamManager.h"
#include "game/UnkBufferStringPool.h"
#include "types.h"

struct UnkTeamList;
f32 fn_71011a76ec(const UnkTeamList* self, const void* target);

enum UnkPresetRange_l3668e8 {
    kRangeNone = 0,
    kRangeNear = 1,
    kRangeMid = 2,
    kRangeFar = 3,
};

struct UnkPresetRanges_l3668e8 {
    u8 pad_0x0[0xbc];
    f32 near_distance;  // 0xbc
    f32 far_distance;   // 0xc0
};

struct UnkPresetAI_l3668e8 {
    u8 pad_0x0[0x1610];
    UnkPresetRanges_l3668e8* ranges;  // 0x1610
};

struct UnkPresetActor_l3668e8 {
    u8 pad_0x0[0x240];
    UnkPresetAI_l3668e8* ai;  // 0x240
};

struct UnkPresetTargetRef_l3668e8 {
    u8 pad_0x0[0x10];
    UnkPresetActor_l3668e8* actor;  // 0x10
};

struct UnkPresetRequest_l3668e8 {
    UnkPresetTargetRef_l3668e8* target;
    u8 pad_0x8[0x10];
    bool alt;  // 0x18
};

struct UnkAttackPreset_l3668e8 {
    u8 pad_0x0[0x2c];
    f32 near_value;      // 0x2c
    f32 mid_value;       // 0x30
    f32 far_value;       // 0x34
    f32 alt_near_value;  // 0x38
    f32 alt_mid_value;   // 0x3c
    f32 alt_far_value;   // 0x40
};

static inline char* TeamManager() {
    char* instance = g_unk_71726b2398;
    if (!instance) {
        instance = static_cast<char*>(fn_71000839a0(0x108, 0x3f));
        fn_71011a664c(instance);
        g_unk_71726b2398 = instance;
    }
    return instance;
}

f32 fn_71003668e8(const UnkAttackPreset_l3668e8* self, const UnkPresetRequest_l3668e8* request) {
    if (!request->target)
        return 0.0f;
    UnkPresetActor_l3668e8* actor = request->target->actor;
    if (!actor || !actor->ai)
        return 0.0f;
    const UnkPresetRanges_l3668e8* ranges = actor->ai->ranges;
    if (!ranges)
        return 0.0f;
    f32 distance = fn_71011a76ec(reinterpret_cast<const UnkTeamList*>(TeamManager()), actor);
    UnkPresetRange_l3668e8 range = kRangeNone;
    if (distance <= ranges->near_distance && ranges->near_distance > 0.0f)
        range = kRangeNear;
    else if (distance >= ranges->far_distance && ranges->far_distance > 0.0f)
        range = kRangeFar;
    else if (ranges->near_distance > 0.0f || ranges->far_distance > 0.0f)
        range = kRangeMid;
    if (request->alt) {
        switch (range) {
        case kRangeNear:
            return self->alt_near_value;
        case kRangeMid:
            return self->alt_mid_value;
        case kRangeFar:
            return self->alt_far_value;
        default:
            break;
        }
    } else {
        switch (range) {
        case kRangeNear:
            return self->near_value;
        case kRangeMid:
            return self->mid_value;
        case kRangeFar:
            return self->far_value;
        default:
            break;
        }
    }
    return 0.0f;
}
