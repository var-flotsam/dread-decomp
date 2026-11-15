// CSamusMovement's unit (unit 1501): the movement's input sample, an x scale from the run info and a y copied
// from it, damped while slow motion or the speed booster is on, scaled while the morph-ball style flag (+0x7e4)
// is set, and zeroed under a few conditions of the aim component. Names and layouts are placeholders.
#include "base/reflection/UnkReflAttributes.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkPlayer;
struct UnkAbility;
struct UnkAimComponent;
struct UnkRunInfo_l9ecf34;

UnkAbility* fn_7100df5928(UnkPlayer* self);
bool fn_710033cf20(UnkAbility* ability);
f32 fn_7100796590(UnkRunInfo_l9ecf34* info);
f32 fn_71010d6858();
bool fn_710034f8dc(UnkAimComponent* self);
extern base::reflection::UnkTunable g_unk_71726b0fc0;
extern base::reflection::UnkTunable g_unk_7172a03d80;

// A single float that is copied as a word.
struct UnkScalar_l9ecf34 {
    f32 value;
    UnkScalar_l9ecf34& operator=(const UnkScalar_l9ecf34& o) {
        *reinterpret_cast<a_u32*>(this) = *reinterpret_cast<const a_u32*>(&o);
        return *this;
    }
};

struct UnkSample_l9ecf34 {
    f32 x;
    UnkScalar_l9ecf34 y;
};

struct UnkRunInfo_l9ecf34 {
    u8 pad_0x0[0xac];
    UnkScalar_l9ecf34 field_0xac;  // 0xac
};

struct UnkAim_l9ecf34 {
    u8 pad_0x0[0x360];
    bool field_0x360;  // 0x360
};

struct UnkActor_l9ecf34 {
    u8 pad_0x0[0x2b0];
    UnkAim_l9ecf34* aim;  // 0x2b0
};

struct UnkMovement_l9ecf34 {
    u8 pad_0x0[0x30];
    UnkActor_l9ecf34* actor;  // 0x30
    u8 pad_0x38[0x44];
    f32 field_0x7c;  // 0x7c
    u8 pad_0x80[0x764];
    bool field_0x7e4;  // 0x7e4
    u8 pad_0x7e5[0x4c73];
    UnkRunInfo_l9ecf34* info;  // 0x5458
};
static_assert(__builtin_offsetof(UnkMovement_l9ecf34, field_0x7e4) == 0x7e4, "layout");
static_assert(__builtin_offsetof(UnkMovement_l9ecf34, info) == 0x5458, "layout");

static inline f32 TunableAt(const base::reflection::UnkTunable& tunable, u32 offset) {
    return *reinterpret_cast<const f32*>(reinterpret_cast<const char*>(&tunable) + offset);
}

void fn_71009ecf34(UnkMovement_l9ecf34* self, UnkSample_l9ecf34* out, u32 flags) {
    out->y = self->info->field_0xac;
    out->x = fn_7100796590(self->info);
    if (UnkAbility* ability = fn_7100df5928(reinterpret_cast<UnkPlayer*>(g_unk_7101d47310))) {
        if (fn_710033cf20(ability))
            out->x = TunableAt(g_unk_71726b0fc0, 0x30) * out->x;
    }
    UnkGameManager* manager = g_unk_7101d47310;
    const u8* bytes = reinterpret_cast<const u8*>(manager);
    if ((manager->field_0xc8 != 0 || manager->field_0x10c) && bytes[0xc4] && !bytes[0xc6]) {
        out->y.value = TunableAt(g_unk_7172a03d80, 0x2c) * out->y.value;
        out->x = fn_71010d6858() * out->x;
    }
    if (self->field_0x7e4) {
        out->x = TunableAt(g_unk_7172a03d80, 0x34) * out->x;
        out->y.value = TunableAt(g_unk_7172a03d80, 0x30) * out->y.value;
    }
    UnkAim_l9ecf34* aim = self->actor->aim;
    if (aim && aim->field_0x360 && !fn_710034f8dc(reinterpret_cast<UnkAimComponent*>(aim)))
        out->x = 0.0f;
    if (self->field_0x7c == 0.0f && (flags & 1))
        out->x = 0.0f;
}
