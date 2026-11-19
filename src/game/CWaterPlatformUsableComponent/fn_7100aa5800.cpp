// CWaterPlatformUsableComponent unit (tools/tu_map.py): the update. After the base update, unless the
// flag at +0x5c5 is set: with the player inside the platform's area (x and y between +0x608 and
// +0x610), each Emmy is told about the block at +0x5c8 (fn_710110f97c) and, while it is busy and the
// component does not hold it back (fn_7100aa5980), stopped (fn_7100545560); outside, each Emmy gets
// the block through fn_710110fae4. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CComponent.h"
#include "game/CWaterPlatformUsableComponent.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2702.h"
#include "game/UnkTU39.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

char* fn_7100545560(char* p0);

struct UnkEmmy_laa5800 {
    u8 pad_0x0[0x29418];
    u8 timer[0xb0e];  // 0x29418
    bool field_0x29f26;
};
static_assert(__builtin_offsetof(UnkEmmy_laa5800, field_0x29f26) == 0x29f26, "layout");

struct UnkEmmyEntry_laa5800 {
    u8 pad_0x0[0x10];
    UnkEmmy_laa5800* emmy;
};

#pragma pack(push, 4)
struct UnkEmmyRegistry_laa5800 {
    u8 pad_0x0[0x8];
    u32 count;
    UnkEmmyEntry_laa5800* items[1];  // 0xc, 4-byte aligned
};
#pragma pack(pop)

struct UnkWaterPlatform_laa5800 {
    u8 pad_0x0[0x5c5];
    bool field_0x5c5;
    u8 pad_0x5c6[2];
    u8 block[0x40];  // 0x5c8
    CVector2D area_min;  // 0x608
    CVector2D area_max;  // 0x610
};
static_assert(__builtin_offsetof(UnkWaterPlatform_laa5800, area_min) == 0x608, "layout");

// NON_MATCHING(functional): Control flow differ from the original; behaves the same on every input tools/equiv_test.py --any-kind tried, with full coverage. Owes a match.
void fn_7100aa5800(UnkWaterPlatform_laa5800* self) {
    fn_71010c4234(reinterpret_cast<char*>(self));
    if (self->field_0x5c5)
        return;
    bool inside = false;
    if (auto* player = reinterpret_cast<UnkActor*>(fn_7100dec788(g_unk_7101d47310))) {
        CVector2D point;
        f32 x = player->field_0x44, y = player->field_0x54, z = player->field_0x64;
        CVector3D position = {x, y, z};
        fn_710007b430(&point, &position);
        if (point.x >= self->area_min.x && point.x <= self->area_max.x && point.y >= self->area_min.y &&
            point.y <= self->area_max.y)
            inside = true;
    }
    UnkEmmyRegistry_laa5800* registry = reinterpret_cast<UnkEmmyRegistry_laa5800*>(g_unk_71726af0a8);
    UnkEmmyEntry_laa5800** end = &registry->items[registry->count];
    for (UnkEmmyEntry_laa5800** it = registry->items; it != end; ++it) {
        UnkEmmyEntry_laa5800* entry = *it;
        UnkEmmy_laa5800* emmy = entry ? entry->emmy : nullptr;
        if (inside) {
            fn_710110f97c(emmy, self->block);
            if ((fn_710002fb70(emmy->timer) || emmy->field_0x29f26) &&
                !fn_7100aa5980(reinterpret_cast<UnkWaterPlatform_A5_b73*>(self)))
                fn_7100545560(reinterpret_cast<char*>(emmy));
        } else {
            fn_710110fae4(emmy, self->block);
        }
    }
}
