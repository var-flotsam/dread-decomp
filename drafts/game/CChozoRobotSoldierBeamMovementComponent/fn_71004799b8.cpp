// CChozoRobotSoldierBeamMovementComponent's unit (tools/tu_map.py, unit 703): the beam's damage set-up: one
// of four tunable values and the matching unit-static id by the two flags at +0x5d8 and +0x5d9, and, when
// the owner's +0x40 is not zero, a push to the side it points. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CCharClassAttackComponent.h"
#include "game/UnkTU1370.h"
#include "game/UnkTU55.h"
#include "types.h"

using base::global::CStrId;

struct UnkRequest;

extern CStrId g_unk_71726cd568 __attribute__((visibility("hidden")));
extern CStrId g_unk_71726cd570 __attribute__((visibility("hidden")));
extern CStrId g_unk_71726cd578 __attribute__((visibility("hidden")));
extern CStrId g_unk_71726cd580 __attribute__((visibility("hidden")));
extern const UnkVec3 g_unk_7101649838 __attribute__((visibility("hidden")));  // pointing right
extern const UnkVec3 g_unk_7101649844 __attribute__((visibility("hidden")));  // pointing left

struct UnkBeamRequest_l4799b8 {
    u8 pad_0x0[0x49];
    u8 field_0x49;
    u8 pad_0x4a[0x2];
    u8 field_0x4c[0x68];
    u32 flags;  // 0xb4
    CStrId id;  // 0xb8
    u8 pad_0xc0[0x8];
    u32 field_0xc8;
};
static_assert(__builtin_offsetof(UnkBeamRequest_l4799b8, field_0xc8) == 0xc8, "");

struct UnkBeamMovement_l4799b8 {
    u8 pad_0x0[0x40];
    f32 field_0x40;
    u8 pad_0x44[0x594];
    bool field_0x5d8;
    bool field_0x5d9;
};
static_assert(__builtin_offsetof(UnkBeamMovement_l4799b8, field_0x5d8) == 0x5d8, "");

static inline f32 TunableValue(u32 offset) {
    return *reinterpret_cast<const f32*>(reinterpret_cast<const char*>(&g_unk_7172716160) + offset);
}

static inline bool IsNearZero(f32 x) {
    return x - 0.001f <= 0.0f && x + 0.001f >= 0.0f;
}

void fn_71004799b8(UnkBeamMovement_l4799b8* self, UnkBeamRequest_l4799b8* request) {
    request->field_0xc8 = 1;
    request->flags |= 0x4000000;
    UnkRequest* damage = reinterpret_cast<UnkRequest*>(request);
    if (self->field_0x5d9) {
        if (self->field_0x5d8) {
            fn_71008c1538(damage, TunableValue(0xec));
            request->id = g_unk_71726cd580;
        } else {
            fn_71008c1538(damage, TunableValue(0xe8));
            request->id = g_unk_71726cd578;
        }
    } else {
        if (self->field_0x5d8) {
            fn_71008c1538(damage, TunableValue(0xd0));
            request->id = g_unk_71726cd570;
        } else {
            fn_71008c1538(damage, TunableValue(0xcc));
            request->id = g_unk_71726cd568;
        }
    }

    if (IsNearZero(self->field_0x40))
        return;
    request->field_0x49 = 1;
    fn_710007b444(request->field_0x4c, self->field_0x40 > 0.0f ? &g_unk_7101649838 : &g_unk_7101649844);
}
